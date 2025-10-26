#include <DxLib.h>
#include "../../Application.h"
#include "../../Manager/InputManager.h"
#include "../../Scene/SceneManager.h"
#include "../../Manager/SystemManager.h"
#include "../../Manager/SoundManager.h"
#include "../../Utility/AsoUtility.h"
#include "Weapon/Stick.h"
#include "Player.h"

Player::Player(void)
{
	player_.modelId_ = -1;
	weapon_ = nullptr;
}

Player::~Player(void)
{
}

void Player::Load(void)
{
	//// モデルのロード
	//player_.modelId_ = MV1LoadModel((Application::PATH_MODEL + "Player/Player.mv1").c_str());

	// 杖を生成・ロード
	weapon_ = new Stick(this);
	weapon_->Load();
}

void Player::Init(void)
{

	auto& ins = SystemManager::GetInstance();

	// 座標の設定
	player_.pos_ = player_.prevPos_ = cameraPos_ = collisionPosTop_ = collisionPosUnder_ = DEFAULT_POS;
	cameraPos_.y += RELATIVE_POS_CAMERA;
	collisionPosTop_.y += COLLISION_OFFSET_TOP;
	collisionPosUnder_.y += COLLISION_OFFSET_UNDER;
	//MV1SetPosition(player_.modelId_, player_.pos_);

	// 向きの設定
	player_.angles_ = AsoUtility::VECTOR_ZERO;
	//MV1SetRotationXYZ(player_.modelId_, player_.angles_);

	// 大きさの設定
	player_.scales_ = SCALES;
	//MV1SetScale(player_.modelId_, player_.scales_);

	// HPの初期化
	player_.hp_ = ability_.hpMax_ = DEFAULT_HP;

	//  速度の初期化
	player_.moveSpeed_ = DEFAULT_MOVE_SPEED;

	// 生存フラグ初期化
	player_.isAlive_ = true;

	player_.isDamaged_ = false;

	// 衝突判定用半径
	player_.collisionRadius_ = COLLISION_RADIUS;

	ability_.stamina_ = ability_.staminaMax_ = DEFAULT_STAMINA;

	mouse_ = { 0,0 };
	SetMousePoint(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2);

	// 他の場所で設定された感度入れる
	MouseSensitivity_ = ins.GetMouseSensitivity();
	PadSensitivity_ = ins.GetPadSensitivity();

	staminaCounter_ = 0.0f;

	// アップグレードのタイプの初期化
	upgradeType_ = PLAYER_UPGRADE::NON;

	// MPとポーションの初期化
	magicNum_ = magicCapacity_ = MAGIC_CAPACITY;
	MPPotionNum_ = MP_POTION_NUM;

	// MP回復時間
	healMPTime_ = 0.0f;
	// MP回復中か
	isHealMP_ = false;

	// 杖の初期化
	weapon_->Init();

}

void Player::Update(void)
{
	// 感度の変更があったら変更を適用する
	Sensitivity();

	// 移動
	ProcessMove();

	// 視点移動
	ProcessAngle();

	// 攻撃
	ProcessAttack();

	// MP回復
	ProcessHealMP();

	// 杖の更新
	if (weapon_ != nullptr)
	{
		weapon_->Update();
	}

}

void Player::Draw(void)
{
	// 杖の描画
	if (weapon_ != nullptr)
	{
		weapon_->Draw();
	}

	int posY = Application::SCREEN_SIZE_Y;

	DrawFormatString(5, posY - 40, 0x7fff00, "HP：%.f", player_.hp_);
	DrawFormatString(5, posY - 20, 0xffd700, "スタミナ：%.f / %.f", ability_.stamina_, ability_.staminaMax_);

	// リロード中の表示
	if (isHealMP_)
	{
		int posX = Application::SCREEN_SIZE_X / 2;
		int posY = Application::SCREEN_SIZE_Y / 2;

		// 背景の枠
		DrawBox(posX - 50, posY - 30,
			posX + 50, posY - 40, 0x696969, true);
		// プログレスバー本体
		DrawBox(posX - 50, posY - 30,
			posX - 50 + static_cast<int>((50 * healMPTime_)), posY - 40, 0xff7f50, true);
		DrawString(posX - 70, posY - 60, "ポーション使用中", 0xffffff);
	}

	DrawFormatString(Application::SCREEN_SIZE_X - 380, Application::SCREEN_SIZE_Y - 25,
		0xffffff, "攻撃可能回数：%.f　/　残りのMPポーション：%.f", magicNum_, MPPotionNum_);

#ifdef _DEBUG

	//// 体 デバッグ用：衝突判定用カプセル
	DrawCapsule3D(collisionPosTop_, collisionPosUnder_,
		player_.collisionRadius_, 10, 0x00ff00, 0x00ff00, false);

	//DrawFormatString(0, 20, 0xffffff, "プレイヤー座標：%.2f,%.2f,%.2f", player_.pos_.x, player_.pos_.y, player_.pos_.z);
	// プレイヤー頭の位置目安
	//DrawSphere3D(cameraPos_, 30.0f, 10, 0x00ff00, 0x0000ff, false);
	//DrawSphere3D(player_.pos_, player_.collisionRadius_, 10, 0x00ff00, 0x0000ff, false);
#endif // _DEBUG

}

void Player::Release(void)
{
	MV1DeleteModel(player_.modelId_);

	if (weapon_ != nullptr)
	{
		weapon_->Release();
		delete weapon_;
		weapon_ = nullptr;
	}
}

void Player::Damage(float hp)
{
	player_.hp_ -= hp;

	if (player_.hp_ <= 0.0f)
	{
		player_.hp_ = 0.0f;
		// 攻撃を受けてHPが無くなったら死亡させる
		player_.isAlive_ = false;
	}
}

void Player::Extrusion(VECTOR overlap)
{
	// 押し出しを行う
	player_.pos_ = VAdd(player_.pos_, overlap);

	// 座標をモデルに設定
	MV1SetPosition(player_.modelId_, player_.pos_);
}

void Player::Upgrade(PLAYER_UPGRADE type, float upNum)
{
	// アップデートしたい能力の情報を得る
	upgradeType_ = type;

	// 種類によって変更する
	switch (upgradeType_)
	{
	case PLAYER_UPGRADE::NON:
		break;
	case PLAYER_UPGRADE::RESTOCK_POTION:

		// ポーションの補充を行う
		MPPotionNum_ += upNum;

		break;
	case PLAYER_UPGRADE::SPEED_UP:

		// スピードを上げる
		player_.moveSpeed_ += upNum;

		break;
	case PLAYER_UPGRADE::STAMINA_UP:

		// スタミナの最大値を上げる
		ability_.stamina_ += upNum;
		ability_.staminaMax_ += upNum;

		break;
	case PLAYER_UPGRADE::HP_UP:

		// HPの最大値を上げる
		player_.hp_ += upNum;
		ability_.hpMax_ += upNum;

		break;
	case PLAYER_UPGRADE::HEAL_HP:

		// HPを回復させる
		player_.hp_ += upNum;
		if (player_.hp_ > ability_.hpMax_)
		{
			player_.hp_ = ability_.hpMax_;
		}

		break;
	default:
		break;
	}

	// 強化し終わったらNONへ戻す
	upgradeType_ = PLAYER_UPGRADE::NON;
}


void Player::ProcessMove(void)
{

	InputManager& inputIns = InputManager::GetInstance();
	auto& sysIns = SystemManager::GetInstance();

	// 水平方向の forward ベクトル（y成分を0にする）
	VECTOR moveForward = VGet(
		sin(player_.angles_.y),
		0.0f,
		cos(player_.angles_.y)
	);

	// 水平方向の right ベクトル（y成分を0にする）
	VECTOR moveRight = VGet(
		cos(player_.angles_.y),
		0.0f,
		-sin(player_.angles_.y)
	);

	// 正規化
	moveForward = VNorm(moveForward);
	moveRight = VNorm(moveRight);

	float moveSpeed_;
	// ダッシュボタンが押されているかつ、スタミナが0ではなかったら入る
	if (inputIns.MoveDash() && ability_.stamina_ >= 0.1f)
	{
		moveSpeed_ = player_.moveSpeed_ + DASH_SPEED;

		// スタミナを減らす
		ability_.stamina_ -= 0.1f;
		if (ability_.stamina_ <= 0.0f)
		{
			// 0を超えないようにする
			ability_.stamina_ = 0.0f;
		}

		staminaCounter_ = 0.0f;
	}
	else
	{
		moveSpeed_ = player_.moveSpeed_;

		staminaCounter_ += SceneManager::GetInstance().GetDeltaTime();

		// スタミナ回復を行うまでの制限時間を超えたら入る
		if (staminaCounter_ >= RECOVERY_STAMINA_WAIT_TIME)
		{
			// スタミナ回復させる
			ability_.stamina_ += RECOVERY_STAMINA;

			if (ability_.stamina_ > ability_.staminaMax_)
			{
				// 最大スタミナを超えないようにする
				ability_.stamina_ = ability_.staminaMax_;
			}

		}
	}

	// 移動方向ベクトルを計算
	VECTOR moveVec = AsoUtility::VECTOR_ZERO;

	// 入力に応じてプレイヤーの位置を更新
	if (inputIns.MoveFront()) { moveVec = VAdd(moveVec, moveForward); }
	if (inputIns.MoveBack()) { moveVec = VSub(moveVec, moveForward); }
	if (inputIns.MoveLeft()) { moveVec = VSub(moveVec, moveRight); }
	if (inputIns.MoveRight()) { moveVec = VAdd(moveVec, moveRight); }

	if (VSize(moveVec) > 0.0001f) // ゼロ判定（VSizeSqはベクトルの長さの2乗）
	{
		// ベクトルを正規化し、移動速度を掛ける
		moveVec = VScale(VNorm(moveVec), moveSpeed_);

		// プレイヤーの位置を更新
		player_.pos_ = VAdd(player_.pos_, moveVec);
	}


	// カメラ位置の更新
	cameraPos_ = player_.pos_;
	cameraPos_.y += RELATIVE_POS_CAMERA;

	// 当たり判定用カプセルの座標更新
	collisionPosTop_ = collisionPosUnder_ = player_.pos_;
	collisionPosTop_.y += COLLISION_OFFSET_TOP;
	collisionPosUnder_.y += COLLISION_OFFSET_UNDER;

}

void Player::ProcessAngle(void)
{
	if (GetJoypadNum() == 0)
	{
		MouseAngle();
	}
	else
	{
		PadAngle();
	}

}

void Player::MouseAngle(void)
{
	// 現在のマウス座標を取得
	GetMousePoint(&mouse_.x, &mouse_.y);

	// 画面中央からのマウスの移動量を調べる
	int deltaX = mouse_.x - Application::SCREEN_SIZE_X / 2;
	int deltaY = mouse_.y - Application::SCREEN_SIZE_Y / 2;

	// マウスの移動量が一定のしきい値以下であれば処理をスキップ
	if ((float)std::abs(deltaX) < THRESHOLD && (float)std::abs(deltaY) < THRESHOLD)
	{
		// マウスカーソルを画面中央に戻す
		SetMousePoint(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2);
		return;
	}

	// マウスの移動量からカメラの回転量を更新する
	player_.angles_.y += deltaX * MouseSensitivity_;
	player_.angles_.x += deltaY * MouseSensitivity_;

	// ピッチ角の角度制限（真上や真下を向きすぎないようにする）
	if (player_.angles_.x > MAX_VIEW_ANGLE)
	{
		player_.angles_.x = MAX_VIEW_ANGLE;
	}
	if (player_.angles_.x < MIN_VIEW_ANGLE)
	{
		player_.angles_.x = MIN_VIEW_ANGLE;
	}

	// 杖の反動がある中視点移動があったら反動をなくす
	if (mouse_.y != Application::SCREEN_SIZE_Y / 2 && weapon_->GetIsRecoil())
	{
		weapon_->SetIsRecoil(false);
	}

	//// マウスカーソルを画面中央に戻す
	SetMousePoint(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2);
}

void Player::PadAngle(void)
{
	auto& ins = InputManager::GetInstance();

	// 接続されているゲームパッド１の情報を取得
	InputManager::JOYPAD_IN_STATE padState =
		ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1);
	// アナログキーの入力値から方向を取得
	VECTOR dir = ins.GetDirectionXZAKey(padState.AKeyRX, padState.AKeyRY);

	player_.angles_.x -= dir.z * PadSensitivity_;
	player_.angles_.y += dir.x * PadSensitivity_;

	// ピッチ角の角度制限（真上や真下を向きすぎないようにする）
	if (player_.angles_.x > MAX_VIEW_ANGLE)
	{
		player_.angles_.x = MAX_VIEW_ANGLE;
	}
	if (player_.angles_.x < MIN_VIEW_ANGLE)
	{
		player_.angles_.x = MIN_VIEW_ANGLE;
	}
}

void Player::ProcessAttack(void)
{
	if (weapon_ == nullptr || isHealMP_)
	{
		// 杖インスタンスの中身がないか、MP回復中だったら処理を行わない
		return;
	}

	if (StartHealMpTrg())
	{
		// MP回復処理が受け付けられたらこの先の処理を行わない
		return;
	}

	auto& ins = InputManager::GetInstance();

	// 杖が攻撃できる状態かつ、MPがあり、左クリックされたら入る
	if (weapon_->GetState() == WeaponBase::STATE::IDLE && magicNum_ > 0)
	{
		if (ins.IsTrgDownAttack())
		{
			// 魔法を生成し描画する
			weapon_->ChangeState(WeaponBase::STATE::GENERATE_MAGIC);
		}
	}

	// 魔法をチャージ状態だったら入る
	if (weapon_->GetState() == WeaponBase::STATE::CHARGE_MAGIC)
	{
		if (ins.IsTrgUpAttack())
		{
			// 攻撃を飛ばす
			weapon_->ChangeState(WeaponBase::STATE::ATTACK);

			magicNum_--;

			// 攻撃SEをながす
			SoundManager::GetInstance().Play(SoundManager::SE::ATTACK);
		}
	}

}

void Player::ProcessHealMP(void)
{
	if (!isHealMP_)
	{
		// リロードの指示がなければ処理を行わない
		return;
	}

	// リロード時間を進める
	healMPTime_ += SceneManager::GetInstance().GetDeltaTime();

	// リロード時間が既定の時間経ったらIDLE状態へ戻す
	if (healMPTime_ >= RELOAD_TIME)
	{
		healMPTime_ = 0.0f;

		// 残りのMPポーションを減らす
		MPPotionNum_--;
		// 攻撃可能回数を増やす
		magicNum_ = magicCapacity_;

		// MP回復を終了させる
		isHealMP_ = false;
	}
}

bool Player::StartHealMpTrg(void)
{
	auto& ins = InputManager::GetInstance();
	auto& insSound = SoundManager::GetInstance();

	// ポーションがない場合
	if ((ins.IsTrgDownAttack() || ins.HealMp()) && MPPotionNum_ <= 0)
	{
		// ポーションが無かったら、SE流して処理せず終わる

		return false;
	}

	// 左クリックされたら入る
	if (ins.IsTrgDownAttack())
	{
		if (magicNum_ <= 0)
		{
			// MPがなければ回復に進む
			isHealMP_ = true;
			// ポーション使用時SEをながす
			insSound.Play(SoundManager::SE::POTION);
			return true;
		}
	}
	// 回復ボタン押されたら入る
	else if (ins.HealMp())
	{
		if (magicNum_ < magicCapacity_)
		{
			// MPが満タンじゃなかったら回復に進む
			isHealMP_ = true;
			// ポーション使用時SEをながす
			insSound.Play(SoundManager::SE::POTION);
			return true;
		}
	}

	return false;
}

void Player::Sensitivity(void)
{
	float nowMouseSensitivity = SystemManager::GetInstance().GetMouseSensitivity();
	float nowPadSensitivity = SystemManager::GetInstance().GetPadSensitivity();
	if (nowMouseSensitivity != MouseSensitivity_)
	{
		// マウス感度に変更が加えてあったら適用する
		MouseSensitivity_ = nowMouseSensitivity;
	}
	if (nowPadSensitivity != PadSensitivity_)
	{
		// パッド感度に変更が加えてあったら適用する
		PadSensitivity_ = nowPadSensitivity;
	}
}

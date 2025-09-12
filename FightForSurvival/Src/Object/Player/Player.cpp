#include <DxLib.h>
#include "../../Application.h"
#include "../../Manager/InputManager.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/SystemManager.h"
#include "../../Utility/AsoUtility.h"
#include "Gun/Handgun.h"
#include "Player.h"

Player::Player(void)
{
	player_.modelId_ = -1;
	gun_ = nullptr;
}

Player::~Player(void)
{
}

void Player::Load(void)
{
	//// モデルのロード
	//player_.modelId_ = MV1LoadModel((Application::PATH_MODEL + "Player/Player.mv1").c_str());
	if (gun_ != nullptr)
	{
		gun_->Load();
	}
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

	yaw_ = pitch_ = 0.0f;

	// 他の場所で設定された感度入れる
	sensitivity_ = ins.GetSensitivity();

	staminaCounter_ = 0.0f;

	// 銃を生成
	gunType_ = ins.GetGunType();
	switch (gunType_)
	{
	case::GUN_TYPE::HANDGUN:
		gun_ = new Handgun(this);
		gun_->Init();
		break;
	case::GUN_TYPE::ASSAULT_RIFLE:
		break;
	case::GUN_TYPE::SHOTGUN:
		break;
	default:
		break;
	}

}

void Player::Update(void)
{
	
	// 移動
	ProcessMove();

	// 視点移動
	ProcessAngle();

	// 攻撃
	ProcessAttack();

	// 銃の更新
	if (gun_ != nullptr)
	{
		gun_->Update();
	}

}

void Player::Draw(void)
{
	// 銃の描画
	if (gun_ != nullptr)
	{
		gun_->Draw();
	}

#ifdef _DEBUG
	int posY = Application::SCREEN_SIZE_Y;

	DrawFormatString(5, posY - 40, 0x7fff00, "HP：%.2d", player_.hp_);
	DrawFormatString(5, posY - 20, 0xffd700, "スタミナ：%.f / %.f", ability_.stamina_, ability_.staminaMax_);

	// 体 デバッグ用：衝突判定用カプセル
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
	if (player_.modelId_ != -1)
	{
		MV1DeleteModel(player_.modelId_);
	}

	if (gun_ != nullptr)
	{
		gun_->Release();
		delete gun_;
		gun_ = nullptr;
	}
}

void Player::SetAbility(ABILITY_TYPE type, float i)
{
	int healHp, addHp;
	healHp = addHp = 0;

	switch (type)
	{
	case Player::ABILITY_TYPE::SPEED_UP:

		player_.moveSpeed_ += i;

		break;
	case Player::ABILITY_TYPE::HEAL_HP:

		healHp = static_cast<int>(i);

		player_.hp_ += healHp;
		if (player_.hp_ > ability_.hpMax_)
		{
			player_.hp_ = ability_.hpMax_;
		}

		break;
	case Player::ABILITY_TYPE::HP_UP:

		addHp = static_cast<int>(i);

		player_.hp_ += addHp;
		ability_.hpMax_ += addHp;

		break;
	case Player::ABILITY_TYPE::STAMINA_UP:

		ability_.stamina_ += i;
		ability_.staminaMax_ += i;

		break;
	default:
		break;
	}
}


void Player::SubHp(int hp)
{
	player_.hp_ -= hp;

	if (player_.hp_ <= 0)
	{
		player_.hp_ = 0;
		// 攻撃を受けてHPが無くなったら死亡させる
		player_.isAlive_ = false;
	}
}


void Player::ProcessMove(void)
{

	InputManager& inputIns = InputManager::GetInstance();
	auto& sysIns = SystemManager::GetInstance();

	// 水平方向の forward ベクトル（y成分を0にする）
	VECTOR moveForward = VGet(
		sin(yaw_),
		0.0f,
		cos(yaw_)
	);

	// 水平方向の right ベクトル（y成分を0にする）
	VECTOR moveRight = VGet(
		cos(yaw_),
		0.0f,
		-sin(yaw_)
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

	// 入力に応じてプレイヤーの位置を更新
	if (inputIns.MoveFront()) { player_.pos_ = VAdd(player_.pos_, VScale(moveForward, moveSpeed_)); }
	if (inputIns.MoveBack()) { player_.pos_ = VSub(player_.pos_, VScale(moveForward, moveSpeed_)); }
	if (inputIns.MoveLeft()) { player_.pos_ = VSub(player_.pos_, VScale(moveRight, moveSpeed_)); }
	if (inputIns.MoveRight()) { player_.pos_ = VAdd(player_.pos_, VScale(moveRight, moveSpeed_)); }

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
	float nowsensitivity = SystemManager::GetInstance().GetSensitivity();
	if (nowsensitivity != sensitivity_)
	{
		// 感度に変更が加えてあったら適用する
		sensitivity_ = nowsensitivity;
	}

	// 現在のマウス座標を取得
	GetMousePoint(&mouse_.x, &mouse_.y);

	// 画面中央からのマウスの移動量を調べる
	int deltaX = mouse_.x - Application::SCREEN_SIZE_X / 2;
	int deltaY = mouse_.y - Application::SCREEN_SIZE_Y / 2;

	// マウスの移動量からカメラの回転量を更新する
	yaw_ += deltaX * sensitivity_;
	pitch_ += deltaY * sensitivity_;

	// ピッチ角の角度制限（真上や真下を向きすぎないようにする）
	if (pitch_ > MAX_VIEW_ANGLE)
	{
		pitch_ = MAX_VIEW_ANGLE;
	}
	if (pitch_ < MIN_VIEW_ANGLE)
	{
		pitch_ = MIN_VIEW_ANGLE;
	}

	// 視点移動があったら反動をなくす
	if (mouse_.y != Application::SCREEN_SIZE_Y / 2)
	{
		gun_->SetIsRecoil(false);
	}

	//// マウスカーソルを画面中央に戻す
	SetMousePoint(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2);
}

void Player::ProcessAttack(void)
{
	if (gun_ == nullptr)
	{
		// 銃インスタンスの中身がなかったら処理を行わない
		return;
	}

	auto& ins = InputManager::GetInstance();

	// 左クリックされたかつ、銃が撃てる状態なら入る
	if (ins.Attack())
	{
		if (gun_->GetCanShot())
		{
			if (gun_->NowBulletNum() != 0)
			{
				// 弾があれば攻撃に進む
				gun_->ChangeState(GunBase::STATE::ATTACK);
			}
			else
			{
				// 弾がなければリロードに進む
				gun_->ChangeState(GunBase::STATE::RELOAD);
			}
		}
	}

	if (ins.Reload())
	{
		// リロードに進む
		gun_->ChangeState(GunBase::STATE::RELOAD);
	}
}

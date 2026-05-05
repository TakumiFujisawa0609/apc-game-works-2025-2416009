#include <DxLib.h>
#include "../../Manager/InputManager.h"
#include "../../Scene/SceneManager.h"
#include "../../Manager/SystemManager.h"
#include "../../Manager/SoundManager.h"
#include "../../Utility/AsoUtility.h"
#include "Weapon/Stick.h"
#include "Player.h"

Player::Player(void)
{
	weapon_ = nullptr;
}

Player::~Player(void)
{
}

void Player::Load(void)
{
	// 杖を生成・ロード
	weapon_ = new Stick(this);
	weapon_->Load();

	// フォントハンドルのロード
	font_[static_cast<int>(FONT_KIND::MAGIC_NUM)] = CreateFontToHandle("x12y12pxMaruMinya", 24, 1, DX_FONTTYPE_ANTIALIASING);;
	font_[static_cast<int>(FONT_KIND::CHANGE_MAGIC_INTERVAL)] = CreateFontToHandle("x12y12pxMaruMinya", 36, 1, DX_FONTTYPE_ANTIALIASING);;
	font_[static_cast<int>(FONT_KIND::MP_POTION_NUM)] = CreateFontToHandle("x12y12pxMaruMinya", 36, 16, DX_FONTTYPE_ANTIALIASING);;
	font_[static_cast<int>(FONT_KIND::USE_POTION)]= CreateFontToHandle("x12y12pxMaruMinya", 16, 1, DX_FONTTYPE_ANTIALIASING);;
}

void Player::Init(void)
{
	// 座標の設定
	player_.pos_ = player_.prevPos_ = cameraPos_ = collisionPosTop_ = collisionPosUnder_ = DEFAULT_POS;
	cameraPos_.y += RELATIVE_POS_CAMERA;
	collisionPosTop_.y += COLLISION_OFFSET_TOP;
	collisionPosUnder_.y += COLLISION_OFFSET_UNDER;

	// 向きの設定
	player_.angle_ = AsoUtility::VECTOR_ZERO;

	// HPの初期化
	player_.hp_ = ability_.hpMax_ = DEFAULT_HP;

	//  速度の初期化
	player_.moveSpeed_ = DEFAULT_MOVE_SPEED;

	// 生存フラグ初期化
	player_.isAlive_ = true;

	player_.isDamaged_ = false;

	// ジャンプ判定の初期化
	player_.isJump_ = false;

	// ジャンプ力の初期化
	player_.gravity_ = 0.0f;

	// 衝突判定用半径
	player_.collisionRadius_ = COLLISION_RADIUS;

	// スタミナ初期化
	ability_.stamina_ = ability_.staminaMax_ = DEFAULT_STAMINA;

	// スタミナ回復させるまでのカウンタ
	staminaCounter_ = 0.0f;

	// マウス座標の初期化
	mouse_ = { 0,0 };

	// マウスカーソルを真ん中に設定
	SetMousePoint(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2);

	// 他の場所で設定された感度入れる
	mouseSensitivity_ = SystemManager::GetInstance().GetMouseSensitivity();
	padSensitivity_ = SystemManager::GetInstance().GetPadSensitivity();

	// アップグレードのタイプの初期化
	upgradeType_ = PLAYER_UPGRADE::MAX;

	// MPとポーションの初期化
	magicNum_ = magicCapacity_ = MAGIC_CAPACITY;
	MPPotionNum_ = MP_POTION_NUM;

	// MP回復時間
	healMPTime_ = 0.0f;
	// MP回復中か
	isHealMP_ = false;

	// 杖の初期化
	weapon_->Init();

	// 魔法の種類を取得
	magicType_ = static_cast<int>(SystemManager::GetInstance().GetTypeMagic());
	// 魔法変更可能間隔
	changeMagicInterval_ = CHANGE_MAGIC_INTERVAL;

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

	// プレイヤーの攻撃の種類を変更する
	ChangeAttackType();

	// 重力処理
	Gravity();
}

void Player::Draw(void)
{
	// 杖の描画
	if (weapon_ != nullptr)
	{
		weapon_->Draw();
	}

#ifdef _DEBUG

	// デバッグ描画
	DebugDraw();

#endif // _DEBUG

}

void Player::Draw2D(void)
{
	// 魔法の残弾数
	DrawFormatStringToHandle(MAGIC_NUM_POS_X
		, MAGIC_NUM_POS_Y
		, FONT_COLOR
		, font_[static_cast<int>(FONT_KIND::MAGIC_NUM)]
		, "%d/%d"
		, magicNum_
		, MAGIC_CAPACITY);

	// MPポーションの残りの数
	DrawFormatStringToHandle(MP_POTION_NUM_POS_X
		, MP_POTION_NUM_POS_Y
		, FONT_COLOR
		, font_[static_cast<int>(FONT_KIND::MP_POTION_NUM)]
		, "x %d"
		, MPPotionNum_);

	// 攻撃変更可能時間
	if (changeMagicInterval_ > 0.0f)
	{
		DrawFormatStringToHandle(CHANGE_MAGIC_INTERVAL_POS_X
			, CHANGE_MAGIC_INTERVAL_POS_Y
			, FONT_COLOR
			, font_[static_cast<int>(FONT_KIND::CHANGE_MAGIC_INTERVAL)]
			, "%d秒"
			, static_cast<int>(changeMagicInterval_));
	}
	
	// リロード中の表示
	if (isHealMP_)
	{
		// 背景の枠
		DrawBox(MP_BAR_UPPER_LEFT_POS_X
			, MP_BAR_UPPER_LEFT_POS_Y,
			MP_BAR_LOWER_RIGHT_POS_X
			, MP_BAR_LOWER_RIGHT_POS_Y
			, MP_BAR_FRAME_COLOR, true);

		// プログレスバー本体
		DrawBox(MP_BAR_UPPER_LEFT_POS_X
			, MP_BAR_UPPER_LEFT_POS_Y,
			MP_BAR_UPPER_LEFT_POS_X + static_cast<int>((MP_BAR_MAX_WIDTH * healMPTime_))
			, MP_BAR_LOWER_RIGHT_POS_Y
			, MP_BAR_MAIN_COLOR, true);

		// なんのプログレスバーなのかの文字
		DrawStringToHandle(MP_BAR_STRING_POS_X
			, MP_BAR_STRING_POS_Y
			, "ポーション使用中"
			, FONT_COLOR
			, font_[static_cast<int>(FONT_KIND::USE_POTION)]);
	}
}

void Player::Release(void)
{
	// フォントハンドルの解放
	for (int i = 0; i < static_cast<int>(FONT_KIND::MAX); i++)
	{
		DeleteFontToHandle(font_[i]);
	}

	// 杖クラスの解放
	if (weapon_ != nullptr)
	{
		weapon_->Release();
		delete weapon_;
		weapon_ = nullptr;
	}
}

void Player::Damage(float damage)
{
	// 指定されたダメージ分HPを削る
	player_.hp_ -= damage;

	// HPが0以下になったら
	if (player_.hp_ <= 0.0f)
	{
		// HPを0初期化
		player_.hp_ = 0.0f;

		// 生存フラグを折る
		player_.isAlive_ = false;
	}
}

void Player::Extrusion(VECTOR overlap)
{
	// 指定量押し出しを行う
	player_.pos_ = VAdd(player_.pos_, overlap);
}

void Player::Upgrade(PLAYER_UPGRADE type, float upNum)
{
	// アップデートしたい能力の情報を得る
	upgradeType_ = type;

	// 種類によって変更する
	switch (upgradeType_)
	{
	case PLAYER_UPGRADE::RESTOCK_POTION:

		// ポーションの補充を行う
		MPPotionNum_ += static_cast<int>(upNum);

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
		// 最大HPを超えないようにする
		if (player_.hp_ > ability_.hpMax_)
		{
			player_.hp_ = ability_.hpMax_;
		}

		break;
	default:
		break;
	}

	// 強化し終わったらNONへ戻す
	upgradeType_ = PLAYER_UPGRADE::MAX;
}

void Player::CollisionStage(VECTOR pos)
{
	// 衝突したら指定座標に押し戻す
	player_.pos_ = VAdd(player_.pos_, pos);
}

void Player::CollisionStage(float posY)
{
	// 衝突したら指定座標に押し戻す
	player_.pos_.y = posY + (player_.collisionRadius_ - COLLISION_OFFSET_UNDER);

	// 重力を初期化
	player_.gravity_ = 0.0f;

	// ジャンプフラグを折る
	player_.isJump_ = false;
}

void Player::ProcessMove(void)
{
	// 水平方向の forward ベクトル（y成分を0にする）
	VECTOR moveForward = VGet(
		sin(player_.angle_.y),
		0.0f,
		cos(player_.angle_.y)
	);

	// 水平方向の right ベクトル（y成分を0にする）
	VECTOR moveRight = VGet(
		cos(player_.angle_.y),
		0.0f,
		-sin(player_.angle_.y)
	);

	// 正規化
	moveForward = VNorm(moveForward);
	moveRight = VNorm(moveRight);

	float moveSpeed_;
	// ダッシュボタンが押されているかつ、スタミナが0ではなかったら入る
	if (InputManager::GetInstance().MoveDash() && ability_.stamina_ >= 0.1f)
	{
		// ダッシュ分加算した移動速度を代入
		moveSpeed_ = player_.moveSpeed_ + DASH_SPEED;

		// スタミナを減らす
		ability_.stamina_ -= 0.1f;
		if (ability_.stamina_ <= 0.0f)
		{
			// 0を超えないようにする
			ability_.stamina_ = 0.0f;
		}

		// スタミナカウンター初期化
		staminaCounter_ = 0.0f;
	}
	else
	{
		// 普通の移動速度を代入
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
	if (InputManager::GetInstance().MoveFront()) { moveVec = VAdd(moveVec, moveForward); }
	if (InputManager::GetInstance().MoveBack()) { moveVec = VSub(moveVec, moveForward); }
	if (InputManager::GetInstance().MoveLeft()) { moveVec = VSub(moveVec, moveRight); }
	if (InputManager::GetInstance().MoveRight()) { moveVec = VAdd(moveVec, moveRight); }

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
	// 使用中のデバイスを見る
	if (SystemManager::GetInstance().GetIsDevice())
	{
		// マウスの視点操作
		MouseAngle();
	}
	else
	{
		// パッドの視点操作
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
	player_.angle_.y += deltaX * mouseSensitivity_;
	player_.angle_.x += deltaY * mouseSensitivity_;

	// ピッチ角の角度制限（真上や真下を向きすぎないようにする）
	if (player_.angle_.x > MAX_VIEW_ANGLE)
	{
		player_.angle_.x = MAX_VIEW_ANGLE;
	}
	if (player_.angle_.x < MIN_VIEW_ANGLE)
	{
		player_.angle_.x = MIN_VIEW_ANGLE;
	}

	// 杖の反動がある中視点移動があったら反動をなくす
	if (mouse_.y != Application::SCREEN_SIZE_Y / 2 && weapon_->GetIsRecoil())
	{
		weapon_->SetIsRecoil(false);
	}

	// マウスカーソルを画面中央に戻す
	SetMousePoint(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2);
}

void Player::PadAngle(void)
{
	// 接続されているゲームパッド１の情報を取得
	InputManager::JOYPAD_IN_STATE padState =
		InputManager::GetInstance().GetJPadInputState(InputManager::JOYPAD_NO::PAD1);
	// アナログキーの入力値から方向を取得
	VECTOR dir = InputManager::GetInstance().GetDirectionXZAKey(padState.AKeyRX, padState.AKeyRY);

	// プレイヤーの視点に入力値の方向×感度を反映
	player_.angle_.x -= dir.z * padSensitivity_;
	player_.angle_.y += dir.x * padSensitivity_;

	// ピッチ角の角度制限（真上や真下を向きすぎないようにする）
	if (player_.angle_.x > MAX_VIEW_ANGLE)
	{
		player_.angle_.x = MAX_VIEW_ANGLE;
	}
	if (player_.angle_.x < MIN_VIEW_ANGLE)
	{
		player_.angle_.x = MIN_VIEW_ANGLE;
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

	// 杖が攻撃できる状態かつ、MPがあったら
	if (weapon_->GetState() == WeaponBase::STATE::IDLE && magicNum_ > 0)
	{
		// 攻撃ボタン押されたら
		if (InputManager::GetInstance().IsTrgDownAttack())
		{
			// 魔法を生成し描画する
			weapon_->ChangeState(WeaponBase::STATE::CREATE_MAGIC);
		}
	}

	// 魔法をチャージ状態だったら入る
	if (weapon_->GetState() == WeaponBase::STATE::CHARGE_MAGIC)
	{
		// 攻撃ボタンを離したら
		if (InputManager::GetInstance().IsTrgUpAttack())
		{
			// 攻撃を飛ばす
			weapon_->ChangeState(WeaponBase::STATE::ATTACK);

			// 魔法の残弾数を減らす
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
		// 0初期化
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
	// ポーションがない場合
	if ((InputManager::GetInstance().IsTrgDownAttack() || InputManager::GetInstance().HealMp()) && MPPotionNum_ <= 0)
	{
		// ポーションが無かったら、SE流して処理せず終わる

		return false;
	}

	// 左クリックされたら入る
	if (InputManager::GetInstance().IsTrgDownAttack())
	{
		if (magicNum_ <= 0)
		{
			// MPがなければ回復に進む
			isHealMP_ = true;
			// ポーション使用時SEをながす
			SoundManager::GetInstance().Play(SoundManager::SE::POTION);
			return true;
		}
	}
	// 回復ボタン押されたら入る
	else if (InputManager::GetInstance().HealMp())
	{
		if (magicNum_ < magicCapacity_)
		{
			// MPが満タンじゃなかったら回復に進む
			isHealMP_ = true;
			// ポーション使用時SEをながす
			SoundManager::GetInstance().Play(SoundManager::SE::POTION);
			return true;
		}
	}

	return false;
}

void Player::Sensitivity(void)
{
	// 今現在の感度を確認
	float nowMouseSensitivity = SystemManager::GetInstance().GetMouseSensitivity();
	float nowPadSensitivity = SystemManager::GetInstance().GetPadSensitivity();

	// 取得している感度と確認した感度が違ったら
	if (nowMouseSensitivity != mouseSensitivity_)
	{
		// マウス感度に変更が加えてあったら適用する
		mouseSensitivity_ = nowMouseSensitivity;
	}

	// 取得している感度と確認した感度が違ったら
	if (nowPadSensitivity != padSensitivity_)
	{
		// パッド感度に変更が加えてあったら適用する
		padSensitivity_ = nowPadSensitivity;
	}
}

void Player::ChangeAttackType(void)
{
	// 0より大きかったら(待ち時間がまだあったら)入る
	if (changeMagicInterval_ > 0.0f)
	{
		// 攻撃変更待ち時間を減らす
		changeMagicInterval_ -= SceneManager::GetInstance().GetDeltaTime();

		// 攻撃変更待ち時間が0より小さくなったら
		if (changeMagicInterval_ < 0.0f)
		{
			// 0初期化
			changeMagicInterval_ = 0.0f;
		}

		return;
	}

	// 前の魔法の種類を保持しておく
	int prevMagicType = magicType_;

	// 魔法の種類を変更するボタンが押されたら
	if (InputManager::GetInstance().AttackSelectLeft())
	{
		// 魔法の種類を変更
		magicType_ --;

		// 範囲外の魔法の種類を指していたら
		if (magicType_ < static_cast<int>(TYPE_MAGIC::STRAIGHT_MAGIC))
		{
			// 爆発魔法を設定する
			magicType_ = static_cast<int>(TYPE_MAGIC::EXPLOSION_MAGIC);
		}
	}
	// 魔法の種類を変更するボタンが押されたら
	else if (InputManager::GetInstance().AttackSelectRight())
	{
		// 魔法の種類を変更
		magicType_ ++;

		// 範囲外の魔法の種類を指していたら
		if (magicType_ > static_cast<int>(TYPE_MAGIC::EXPLOSION_MAGIC))
		{
			// 直線魔法を設定する
			magicType_ = static_cast<int>(TYPE_MAGIC::STRAIGHT_MAGIC);
		}
	}

	// 魔法の種類に変更があったら
	if (prevMagicType != magicType_)
	{
		// 魔法の種類を保存
		SystemManager::GetInstance().SetTypeMagic(static_cast<TYPE_MAGIC>(magicType_));
		
		// すぐに魔法を変えられないように時間をもうける
		changeMagicInterval_ = CHANGE_MAGIC_INTERVAL;
	}

}

void Player::Gravity(void)
{
	// 重力を増やす
	player_.gravity_ += GRAVITATION;

	// 重力が最大値を超えないようにする
	if (player_.gravity_ <= MAX_GRAVITATION)
	{
		player_.gravity_ = MAX_GRAVITATION;
	}

	// Y軸移動量を計算
	float movePowY = player_.gravity_ * player_.moveSpeed_;

	// 重力をプレイヤー座標に反映する
	player_.pos_.y += movePowY;
}

void Player::DebugDraw(void)
{
	//// 体 デバッグ用：衝突判定用カプセル
	//DrawCapsule3D(collisionPosTop_, collisionPosUnder_,
	//	player_.collisionRadius_, 10, 0x00ff00, 0x00ff00, false);

	// プレイヤー座標
	DrawFormatString(0, 100,
		0xffffff, "プレイヤー座標：%.f, %.f, %.f", player_.pos_.x, player_.pos_.y, player_.pos_.z);

	// プレイヤー頭の位置目安
	//DrawSphere3D(cameraPos_, 30.0f, 10, 0x00ff00, 0x0000ff, false);
	//DrawSphere3D(player_.pos_, player_.collisionRadius_, 10, 0x00ff00, 0x0000ff, false);
}
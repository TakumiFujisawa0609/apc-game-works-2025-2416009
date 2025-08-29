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
	gun_->Load();
}

void Player::Init(void)
{

	auto& ins = SystemManager::GetInstance();

	// 座標の設定
	player_.pos_ = player_.prevPos_ = DEFAULT_POS;
	//MV1SetPosition(player_.modelId_, player_.pos_);

	// 向きの設定
	player_.angles_ = AsoUtility::VECTOR_ZERO;
	//MV1SetRotationXYZ(player_.modelId_, player_.angles_);

	// 大きさの設定
	player_.scales_ = SCALES;
	//MV1SetScale(player_.modelId_, player_.scales_);

	// HPの初期化
	player_.hp_ = ability_.hpMax_ = DEFAULT_HP;

	player_.moveSpeed_ = DEFAULT_MOVE_SPEED;

	ability_.stamina_ = ability_.staminaMax_ = DEFAULT_STAMINA;

	mouse_ = { 0,0 }; 
	SetMousePoint(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2);

	yaw_ = pitch_ = 0.0f;

	// 他の場所で設定された感度入れる
	sensitivity_ = ins.GetSensitivity();

	staminaCounter_ = 0.0f;

	// 銃を生成
	gun_ = new Handgun(this);
	gun_->Init();
}

void Player::Update(void)
{
	
	// 移動
	ProcessMove();

	// 視点移動
	ProcessAngle();

	// 銃の更新
	gun_->Update();

}

void Player::Draw(void)
{
	// 銃の描画
	gun_->Draw();

#ifdef _DEBUG
	DrawFormatString(0, 20, 0xffffff, "プレイヤー座標：%.2f,%.2f,%.2f", player_.pos_.x, player_.pos_.y, player_.pos_.z);
	DrawFormatString(0, 70, 0x7fff00, "HP：%.2d", player_.hp_);
	DrawFormatString(0, 90, 0xffd700, "スタミナ：%.f / %.f", ability_.stamina_, ability_.staminaMax_);
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
	if (inputIns.MoveDash() && ability_.stamina_ > 0.0f)
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

}

void Player::ProcessAngle(void)
{
	// 現在のマウス座標を取得
	GetMousePoint(&mouse_.x, &mouse_.y);

	// 画面中央からのマウスの移動量を調べる
	int deltaX = mouse_.x - Application::SCREEN_SIZE_X / 2;
	int deltaY = mouse_.y - Application::SCREEN_SIZE_Y / 2;

	// マウスの移動量からカメラの回転量を更新する
	yaw_ += deltaX * sensitivity_;
	pitch_ += deltaY * sensitivity_;

	// ピッチ角の角度制限（真上や真下を向きすぎないようにする）
	if (pitch_ > 1.5f)
	{
		pitch_ = 1.5f;
	}
	if (pitch_ < -1.5f)
	{
		pitch_ = -1.5f;
	}

	//// マウスカーソルを画面中央に戻す
	SetMousePoint(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2);
}

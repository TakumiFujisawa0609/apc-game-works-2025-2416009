#include <DxLib.h>
#include "../../Application.h"
#include "../../Manager/InputManager.h"
#include "../../Manager/SystemManager.h"
#include "../../Utility/AsoUtility.h"
#include "Player.h"

Player::Player(void)
{
}

Player::~Player(void)
{
}

void Player::Load(void)
{
	//// モデルのロード
	//player_.modelId_ = MV1LoadModel((Application::PATH_MODEL + "Player/Player.mv1").c_str());
}

void Player::Init(void)
{
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
	player_.hp_ = MAX_HP;

	player_.moveSpeed_ = SPEED_MOVE;

	mouse_ = { 0,0 };

	yaw_ = pitch_ = 0.0f;

	// 他の場所で設定された感度入れる
	sensitivity_ = SystemManager::GetInstance().GetSensitivity();
}

void Player::Update(void)
{

	// 視点移動
	ProcessAngle();
	
	// 移動
	ProcessMove();

}

void Player::Draw(void)
{
#ifdef _DEBUG
	DrawFormatString(0, 20, 0xffffff, "プレイヤー座標：%.2f,%.2f,%.2f", player_.pos_.x, player_.pos_.y, player_.pos_.z);
#endif // _DEBUG

}

void Player::Release(void)
{
}

void Player::ProcessMove(void)
{

	InputManager& ins = InputManager::GetInstance();

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

	// 入力に応じてプレイヤーの位置を更新
	if (ins.MoveFront()) { player_.pos_ = VAdd(player_.pos_, VScale(moveForward, player_.moveSpeed_)); }
	if (ins.MoveBack()) { player_.pos_ = VSub(player_.pos_, VScale(moveForward, player_.moveSpeed_)); }
	if (ins.MoveLeft()) { player_.pos_ = VSub(player_.pos_, VScale(moveRight, player_.moveSpeed_)); }
	if (ins.MoveRight()) { player_.pos_ = VAdd(player_.pos_, VScale(moveRight, player_.moveSpeed_)); }

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

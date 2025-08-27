#include <EffekseerForDXLib.h>
#include "InputManager.h"
#include "../Utility/AsoUtility.h"
#include "Camera.h"

Camera::Camera(void):player_(nullptr)
{
}

Camera::Camera(const Player* player) :player_(player)
{
	// カメラの位置が x = 320.0f, y = 240.0f, z = (画面のサイズによって変化)、
	// 注視点の位置は x = 320.0f, y = 240.0f, z = 1.0f
	// カメラの上方向は x = 0.0f, y = 1.0f, z = 0.0f
	// 右上位置からZ軸のプラス方向を見るようなカメラ
}

Camera::~Camera(void)
{
}

void Camera::Init(void)
{
	pos_ = DEFAULT_POS;
	angles_ = AsoUtility::VECTOR_ZERO;
}

void Camera::Update(void)
{
	// カメラの移動
	MoveCamera();

	// カメラの振動
	ShakeCamera();
}

void Camera::SetBeforeDraw(void)
{
	// クリップ距離を設定する(SetDrawScreenでリセットされる)
	SetCameraNearFar(CAMERA_NEAR, CAMERA_FAR);

	switch (mode_)
	{
	case Camera::MODE::FIXED_POINT:

		SetBeforeDrawFixedPoint();

		break;
	case Camera::MODE::FREE:

		SetBeforeDrawFree();

		break;
	}

	// カメラの設定(位置と角度による制御)
	SetCameraPositionAndAngle(
		pos_,
		angles_.x,
		angles_.y,
		angles_.z
	);

	// DXライブラリのカメラとEffekseerのカメラを同期する
	Effekseer_Sync3DSetting();
}

void Camera::DrawDebug(void)
{

	//DrawFormatString(
	//	0, 0, 0x000000, "カメラ座標：(%.2f, %.2f, %.2f)",
	//	pos_.x, pos_.y, pos_.z);
	//DrawFormatString(
	//	0, 20, 0x000000, "カメラ角度：(%.2f, %.2f, %.2f)",
	//	angles_.x * 180.0f / DX_PI_F,
	//	angles_.y * 180.0f / DX_PI_F,
	//	angles_.z * 180.0f / DX_PI_F);

}

void Camera::Release(void)
{
}

void Camera::ChangeMode(MODE mode)
{

	// カメラモードの変更
	mode_ = mode;

	// 変更時の初期化処理
	switch (mode_)
	{
	case Camera::MODE::FIXED_POINT:
		break;
	case Camera::MODE::FREE:
		break;
	}

}

void Camera::SetBeforeDrawFixedPoint(void)
{
}

void Camera::SetBeforeDrawFree(void)
{

	//auto& ins = InputManager::GetInstance();
	//// WASDでカメラの位置を変える
	//float movePow = 3.0f;
	//if (ins.IsNew(KEY_INPUT_W)) { pos_.z += movePow; }
	//if (ins.IsNew(KEY_INPUT_A)) { pos_.x -= movePow; }
	//if (ins.IsNew(KEY_INPUT_S)) { pos_.z -= movePow; }
	//if (ins.IsNew(KEY_INPUT_D)) { pos_.x += movePow; }
	//if (ins.IsNew(KEY_INPUT_Q)) { pos_.y += movePow; }
	//if (ins.IsNew(KEY_INPUT_E)) { pos_.y -= movePow; }

	//// 矢印キーでカメラの角度を変える
	//float rotPow = 1.0f * DX_PI_F / 180.0f;
	//if (ins.IsNew(KEY_INPUT_DOWN)) { angles_.x += rotPow; }
	//if (ins.IsNew(KEY_INPUT_UP)) { angles_.x -= rotPow; }
	//if (ins.IsNew(KEY_INPUT_RIGHT)) { angles_.y += rotPow; }
	//if (ins.IsNew(KEY_INPUT_LEFT)) { angles_.y -= rotPow; }

}

void Camera::MoveCamera(void)
{
	if (player_ == nullptr)
	{
		// 例外スロー対策
		return;
	}

	pos_ = player_->GetPlayer().pos_;

	// プレイヤーの角度をカメラの角度として設定
	angles_.y = player_->GetYaw();
	angles_.x = player_->GetPitch();

}

void Camera::ShakeCamera(void)
{
	// 現在のカメラ座標を取得
	if (hitStopCounter_ > 0) {

		// 振動先をカウンターから計算する----------
		// 0 or 1
		int shake = (hitStopCounter_ / 5) % 2;
		// 0 or 2　中心を作る
		shake *= 2;
		// -1 or 1　0を中心にする
		shake -= 1;
		// -3 or 3　振れ幅を付ける
		shake *= 2;
		// ----------------------------------------

		pos_.x -= shake;
		pos_.y += shake;

		hitStopCounter_--;
	}
}

#include <DxLib.h>
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/EndManager.h"
#include "../Manager/Camera.h"
#include "../Manager/CollisionManager.h"
#include "TitleScene.h"

TitleScene::TitleScene(void)
{
	camera_ = nullptr;
}

TitleScene::~TitleScene(void)
{
}

void TitleScene::Load(void)
{
}

void TitleScene::Init(void)
{
	// カメラの初期化
	camera_ = new Camera();
	camera_->Init();

	// ポーズモードの種類
	state_ = STATE::NON;

	// 座標初期化
	pos_[STATE::GAMESTART] = { GAMESTART_POS_X ,GAMESTART_POS_Y };
	pos_[STATE::EXIT] = { EXIT_POS_X ,EXIT_POS_Y };

	// マウス座標
	mousePos_ = { 0,0 };

	isPrevStart_ = isNowStart_ =  false;
}

void TitleScene::Update(void)
{
	isPrevStart_ = isNowStart_;
	if (InputManager::GetInstance().PushStartKey() && !isNowStart_)
	{
		isNowStart_ = true;
	}

	if (isPrevStart_ && isNowStart_)
	{
		// マウスの位置を取得
		GetMousePoint(&mousePos_.x, &mousePos_.y);

		// 引数の座標によって選択中のものを変化させる
		InputDevisUpdate(mousePos_);

		// 確定処理
		Confirm();
	}

	// カメラの更新
	camera_->Update();
}

void TitleScene::Draw(void)
{
	// タイトル名
	DrawString(Application::SCREEN_SIZE_X / 2 - 80, 200, "Fight For Survival", 0xffffff);

	if (isPrevStart_ && isNowStart_)
	{
		switch (state_)
		{
		case STATE::GAMESTART:

			DrawString(GAMESTART_POS_X, GAMESTART_POS_Y, "GameStart", 0x00ff00);
			DrawString(EXIT_POS_X, EXIT_POS_Y, "Exit", 0xffffff);

			break;
		case STATE::EXIT:

			DrawString(GAMESTART_POS_X, GAMESTART_POS_Y, "GameStart", 0xffffff);
			DrawString(EXIT_POS_X, EXIT_POS_Y, "Exit", 0x00ff00);

			break;
		case STATE::NON:

			DrawString(GAMESTART_POS_X, GAMESTART_POS_Y, "GameStart", 0xffffff);
			DrawString(EXIT_POS_X, EXIT_POS_Y, "Exit", 0xffffff);

			break;
		}
	}
	else
	{
		DrawString(Application::SCREEN_SIZE_X / 2 - 60, Application::SCREEN_SIZE_Y - 190, "Check to Start", 0xffffff);
	}

	// カメラの設定
	camera_->SetBeforeDraw();

#ifdef _DEBUG

	// カメラのデバック描画
	camera_->DrawDebug();

	if (isPrevStart_ && isNowStart_)
	{
		DrawBox(pos_[STATE::GAMESTART].x,
			pos_[STATE::GAMESTART].y,
			pos_[STATE::GAMESTART].x + COL_SIZE_X,
			pos_[STATE::GAMESTART].y + COL_SIZE_Y, 0xff0000, false);

		DrawBox(pos_[STATE::EXIT].x,
			pos_[STATE::EXIT].y,
			pos_[STATE::EXIT].x + COL_SIZE_X,
			pos_[STATE::EXIT].y + COL_SIZE_Y, 0xff0000, false);
	}
#endif // _DEBUG

}

void TitleScene::Release(void)
{
	if (camera_ != nullptr)
	{
		// カメラの解放
		camera_->Release();
		delete camera_;
	}
}

void TitleScene::Confirm(void)
{

	InputManager& ins = InputManager::GetInstance();

	switch (state_)
	{
	case STATE::GAMESTART:

		if (ins.Confirm())
		{
			//SoundManager::GetInstance()->Play(SoundManager::SE::DONE);
			// スタートキーが押されたらゲームシーンへ移る
			SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
		}

		break;
	case STATE::EXIT:

		if (ins.Confirm())
		{
			// ゲームを終了させる
			EndManager::GetInstance().SetIsEnd(true);
		}

		break;
	}
}

void TitleScene::InputDevisUpdate(Vector2 pos)
{
	// カーソルとの当たり判定を行う
	if (CollisionManager::RectangleAndPoint(pos_[STATE::GAMESTART], COL_SIZE_X, COL_SIZE_Y, pos))
	{
		state_ = STATE::GAMESTART;
	}
	else if (CollisionManager::RectangleAndPoint(pos_[STATE::EXIT], COL_SIZE_X, COL_SIZE_Y, pos))
	{
		state_ = STATE::EXIT;
	}
	else
	{
		state_ = STATE::NON;
	}

}

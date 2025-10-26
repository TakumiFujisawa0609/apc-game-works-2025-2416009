#include <DxLib.h>
#include "../../Scene/SceneManager.h"
#include "../../Manager/InputManager.h"
#include "../../Manager/EndManager.h"
#include "../../Manager/Camera.h"
#include "../../Manager/CollisionManager.h"
#include "../../Manager/SoundManager.h"
#include "TitleScene.h"

TitleScene::TitleScene(void)
{
	camera_ = nullptr;

	// マウスカーソルを表示
	SetMouseDispFlag(true);
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
	ChangeState(STATE::NON);

	// 座標初期化
	pos_[STATE::GAMESTART] = { GAMESTART_POS_X ,GAMESTART_POS_Y };
	pos_[STATE::EXIT] = { EXIT_POS_X ,EXIT_POS_Y };

	isPrevStart_ = isNowStart_ =  false;

	// BGMをかける
	SoundManager::GetInstance().Play(SoundManager::BGM::TITLE);
}

void TitleScene::Update(void)
{
	isPrevStart_ = isNowStart_;
	if (InputManager::GetInstance().PushStartKey() && !isNowStart_)
	{
		SoundManager::GetInstance().Play(SoundManager::SE::DECIDE);
		isNowStart_ = true;
	}

	if (isPrevStart_ && isNowStart_)
	{
		if (GetJoypadNum() == 0)
		{
			// 引数の座標によって選択中のものを変化させる
			Collision();
		}
		else
		{
			// 選択処理
			PadSelect();
		}

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

	// BGMを止める
	SoundManager::GetInstance().Stop(SoundManager::BGM::TITLE);
}

void TitleScene::Confirm(void)
{

	InputManager& ins = InputManager::GetInstance();

	if (ins.Confirm())
	{
		switch (state_)
		{
		case STATE::GAMESTART:

			//SoundManager::GetInstance()->Play(SoundManager::SE::DONE);
			// スタートキーが押されたらゲームシーンへ移る
			SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);

			break;
		case STATE::EXIT:

			// ゲームを終了させる
			EndManager::GetInstance().SetIsEnd(true);

			break;
		}

		// 決定SEをながす
		SoundManager::GetInstance().Play(SoundManager::SE::DECIDE);

	}
}

void TitleScene::Collision(void)
{
	auto prevState = state_;

	// 当たり判定取る
	for (int i = 0; i < static_cast<int>(STATE::NON); i++)
	{
		if (CollisionManager::RectangleAndMouse(pos_[i], COL_SIZE_X, COL_SIZE_Y))
		{
			ChangeState(static_cast<STATE>(i));

			break;
		}
		else
		{
			ChangeState(STATE::NON);
		}
	}

	if (state_ != prevState && state_ != STATE::NON)
	{
		// 何も選択されていない状態から選択されたらSEを流す
		SoundManager::GetInstance().Play(SoundManager::SE::SELECT);
	}

}

void TitleScene::PadSelect(void)
{
	auto& ins = InputManager::GetInstance();
	auto prevState = state_;

	switch (state_)
	{
	case TitleScene::GAMESTART:

		if (ins.SelectDown())
		{
			ChangeState(STATE::EXIT);
		}

		break;
	case TitleScene::EXIT:

		if (ins.SelectUp())
		{
			ChangeState(STATE::GAMESTART);
		}

		break;
	case TitleScene::NON:

		ChangeState(STATE::GAMESTART);

		break;
	default:
		break;
	}

	if (state_ != prevState && state_ != STATE::NON)
	{
		// 何も選択されていない状態から選択されたらSEを流す
		SoundManager::GetInstance().Play(SoundManager::SE::SELECT);
	}
}

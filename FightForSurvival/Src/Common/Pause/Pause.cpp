#include <DxLib.h>
#include "../../Manager/InputManager.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/CollisionManager.h"
//#include "../Manager/SoundManager/SoundManager.h"
#include "Pause.h"

// コンストラクタ
Pause::Pause(void)
{
}

// デストラクタ
Pause::~Pause(void)
{
}

// ロード関連
void Pause::Load(void)
{
	//LoadDivGraph("Data/Image/Pause/pause.png", DRAW_NUM, DRAW_NUM, 1, IMAGE_SIZE_X, IMAGE_SIZE_Y, images_,true);
}

// 初期化
void Pause::Init(void)
{
	// ポーズモードの種類
	pause_ = PAUSE::NON;

	// 座標初期化
	pos_[static_cast<int>(PAUSE::CONTINUE)] = { CONTINUE_POS_X ,CONTINUE_POS_Y };
	pos_[static_cast<int>(PAUSE::TITLE)] = { TITLE_POS_X ,TITLE_POS_Y };

	// マウス座標
	mousePos_ = { 0,0 };

	// ポーズモード中か確認
	pauseMode_ = false;
}

// 更新
void Pause::Update(void)
{
	// ポーズモード中だったら選択処理できる
	if (pauseMode_)
	{
		// マウスの位置を取得
		GetMousePoint(&mousePos_.x, &mousePos_.y);

		// 引数の座標によって選択中のものを変化させる
		InputDevisUpdate(mousePos_);

		// 確定処理
		Confirm();
	}

	// ポーズモードのON・OFF
	StartPause();
}

// 描画
void Pause::Draw(void)
{
	if (pauseMode_)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
		DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		switch (pause_)
		{
		case Pause::PAUSE::CONTINUE:
			//DrawRotaGraph(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2, 1.0, 0.0, images_[static_cast<int>(PAUSE::CONTINUE)], true);

			DrawString(CONTINUE_POS_X, CONTINUE_POS_Y, "Continue", 0x00ff00);
			DrawString(TITLE_POS_X, TITLE_POS_Y, "Title", 0xffffff);

			break;
		case Pause::PAUSE::TITLE:

			//DrawRotaGraph(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2, 1.0, 0.0, images_[static_cast<int>(PAUSE::TITLE)], true);

			DrawString(CONTINUE_POS_X, CONTINUE_POS_Y, "Continue", 0xffffff);
			DrawString(TITLE_POS_X, TITLE_POS_Y, "Title", 0x00ff00);

			break;
		case Pause::PAUSE::NON:

			//DrawRotaGraph(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2, 1.0, 0.0, images_[static_cast<int>(PAUSE::NON)], true);

			DrawString(CONTINUE_POS_X, CONTINUE_POS_Y, "Continue", 0xffffff);
			DrawString(TITLE_POS_X, TITLE_POS_Y, "Title", 0xffffff);

			break;
		}

#ifdef _DEBUG

		DrawBox(pos_[static_cast<int>(PAUSE::CONTINUE)].x,
			pos_[static_cast<int>(PAUSE::CONTINUE)].y,
			pos_[static_cast<int>(PAUSE::CONTINUE)].x + COL_SIZE_X,
			pos_[static_cast<int>(PAUSE::CONTINUE)].y + COL_SIZE_Y, 0xff0000, false);

		DrawBox(pos_[static_cast<int>(PAUSE::TITLE)].x,
			pos_[static_cast<int>(PAUSE::TITLE)].y,
			pos_[static_cast<int>(PAUSE::TITLE)].x + COL_SIZE_X,
			pos_[static_cast<int>(PAUSE::TITLE)].y + COL_SIZE_Y, 0xff0000, false);

#endif // _DEBUG
	}
}

// 解放
void Pause::Release(void)
{
}

void Pause::Confirm(void)
{

	InputManager& ins = InputManager::GetInstance();

	switch (pause_)
	{
	case Pause::PAUSE::CONTINUE:

		if (ins.Confirm())
		{
			//SoundManager::GetInstance()->Play(SoundManager::SE::DONE);

			pauseMode_ = false;
		}

		break;
	case Pause::PAUSE::TITLE:

		if (ins.Confirm())
		{
			//SoundManager::GetInstance()->Play(SoundManager::SE::DONE);
			//SoundManager::GetInstance()->Stop(SoundManager::BGM::GAME);
			SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
		}

		break;
	}

}

void Pause::StartPause(void)
{
	InputManager& ins = InputManager::GetInstance();

	if (ins.PauseKeys())
	{
		//SoundManager::GetInstance()->Play(SoundManager::SE::CANCEL);
		pauseMode_ = !pauseMode_;
		// マウスカーソルを表示しない
		SetMouseDispFlag(pauseMode_);
	}
}

void Pause::InputDevisUpdate(Vector2 pos)
{

	// カーソルとの当たり判定を行う
	if (CollisionManager::RectangleAndPoint(pos_[static_cast<int>(PAUSE::CONTINUE)], COL_SIZE_X, COL_SIZE_Y, pos))
	{
		pause_ = PAUSE::CONTINUE;
	}
	else if (CollisionManager::RectangleAndPoint(pos_[static_cast<int>(PAUSE::TITLE)], COL_SIZE_X, COL_SIZE_Y, pos))
	{
		pause_ = PAUSE::TITLE;
	}
	else
	{
		pause_ = PAUSE::NON;
	}

}

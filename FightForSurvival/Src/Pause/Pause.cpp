#include <DxLib.h>
#include "../Manager/InputManager/InputManager.h"
#include "../Manager/SceneManager/SceneManager.h"
#include "../Manager/SoundManager/SoundManager.h"
#include "../GameConfig.h"
#include "Pause.h"

// コンストラクタ
Pause::Pause(void)
{
}

// デストラクタ
Pause::~Pause(void)
{
}

// 初期化
void Pause::Init(void)
{
	// 現在の操作タイプを確認
	operationType_ = GameConfig::GetOperationType();

	// ポーズモードの種類
	pause_ = PAUSE::NON;

	// 座標初期化
	pos_[static_cast<int>(PAUSE::CONTINUE)] = { CONTINUE_POS_X ,CONTINUE_POS_Y };
	pos_[static_cast<int>(PAUSE::TITLE)] = { TITLE_POS_X ,TITLE_POS_Y };

	// マウス座標
	mousePos_ = { 0,0 };

	// ジョイスティック座標
	joystickPos_ = { Application::SCREEN_SIZE_X / 2,Application::SCREEN_SIZE_Y / 2 };

	// ポーズモード中か確認
	pauseMode_ = false;
}

// ロード関連
void Pause::Load(void)
{
	LoadDivGraph("Data/Image/Pause/pause.png", DRAW_NUM, DRAW_NUM, 1, IMAGE_SIZE_X, IMAGE_SIZE_Y, images_,true);

	cursorImage_ = LoadGraph("Data/Image/cursor.png");
}

// 更新
void Pause::Update(void)
{
	// 現在の操作タイプを確認
	operationType_ = GameConfig::GetOperationType();

	if (pauseMode_)
	{
		SoundManager::GetInstance()->Stop(SoundManager::BGM::GAME);

		// ポーズモード中だったら選択処理できる
		switch (operationType_)
		{
		case OPERATION_TYPE::MOUSE:

			GetMousePoint(&mousePos_.x, &mousePos_.y);

			InputDevisUpdate(mousePos_);

			break;
		case OPERATION_TYPE::JOYPAD:

			joystickPos_ = JoyMove::JoystickMove(joystickPos_);

			InputDevisUpdate(joystickPos_);

			break;
		}

		Confirm();

	}

	// ポーズモードになるトリガー
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
			DrawRotaGraph(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2, 1.0, 0.0, images_[static_cast<int>(PAUSE::CONTINUE)], true);

			break;
		case Pause::PAUSE::TITLE:

			DrawRotaGraph(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2, 1.0, 0.0, images_[static_cast<int>(PAUSE::TITLE)], true);

			break;
		case Pause::PAUSE::NON:

			DrawRotaGraph(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2, 1.0, 0.0, images_[static_cast<int>(PAUSE::NON)], true);

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

		// カーソルの描画
		switch (operationType_)
		{
		case OPERATION_TYPE::MOUSE:
			DrawCursor(mousePos_);
			break;
		case OPERATION_TYPE::JOYPAD:
			DrawCursor(joystickPos_);
			break;
		}
	}

}

// 解放
void Pause::Release(void)
{
	DeleteGraph(cursorImage_);

	for (int i = 0; i < DRAW_NUM; i++)
	{
		DeleteGraph(images_[i]);
	}
}

// 矩形と点の当たり判定
bool Pause::RectangleAndPoint(Vector2F pos1, int wid1, int hig1, Vector2 pos2)
{

	if (pos1.x + wid1 > pos2.x &&
		pos1.x < pos2.x &&
		pos1.y + hig1 >pos2.y &&
		pos1.y < pos2.y)
	{
		return true;
	}

	return false;

}

void Pause::Confirm(void)
{

	InputManager& ins = InputManager::GetInstance();

	switch (pause_)
	{
	case Pause::PAUSE::CONTINUE:

		if (ins.ConfirmButtons())
		{
			SoundManager::GetInstance()->Play(SoundManager::SE::DONE);

			pauseMode_ = false;
		}

		break;
	case Pause::PAUSE::TITLE:

		if (ins.ConfirmButtons())
		{
			SoundManager::GetInstance()->Play(SoundManager::SE::DONE);
			SoundManager::GetInstance()->Stop(SoundManager::BGM::GAME);
			SceneManager::GetInstance()->ChangeScene(SceneManager::SCENE_TYPE::TITLE);
		}

		break;
	}

}

void Pause::StartPause(void)
{

	InputManager& ins = InputManager::GetInstance();

	if (ins.PauseKeys())
	{
		SoundManager::GetInstance()->Play(SoundManager::SE::CANCEL);
		pauseMode_ = !pauseMode_;
	}

}

void Pause::InputDevisUpdate(Vector2 pos)
{

	// カーソルとの当たり判定を行う
	if (RectangleAndPoint(pos_[static_cast<int>(PAUSE::CONTINUE)], COL_SIZE_X, COL_SIZE_Y, pos))
	{
		pause_ = PAUSE::CONTINUE;
	}
	else if (RectangleAndPoint(pos_[static_cast<int>(PAUSE::TITLE)], COL_SIZE_X, COL_SIZE_Y, pos))
	{
		pause_ = PAUSE::TITLE;
	}
	else
	{
		pause_ = PAUSE::NON;
	}

}

void Pause::DrawCursor(Vector2 pos)
{
	DrawRotaGraph(pos.x, pos.y, 0.2, 0.0, cursorImage_, true);
}

#include <DxLib.h>
#include "../../Manager/InputManager.h"
#include "../../Scene/SceneManager.h"
#include "../../Manager/SoundManager.h"
#include "../../Utility/Collision/CollisionUtility.h"
#include "Setting/Setting.h"
#include "Pause.h"

// コンストラクタ
Pause::Pause(void)
	:
	setting_(nullptr)
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

	// 設定のインスタンスを生成
	setting_ = new Setting();
	setting_->Load();
}

// 初期化
void Pause::Init(void)
{
	// ポーズモードの種類
	ChangePause(PAUSE::NON);

	// 座標初期化
	pos_[static_cast<int>(PAUSE::CONTINUE)] = { CONTINUE_POS_X ,CONTINUE_POS_Y };
	pos_[static_cast<int>(PAUSE::SETTING)] = { SETTING_POS_X ,SETTING_POS_Y };
	pos_[static_cast<int>(PAUSE::TITLE)] = { TITLE_POS_X ,TITLE_POS_Y };

	// ポーズモード中か確認
	pauseMode_ = false;

	// 設定中か
	isSetting_ = false;

	// 設定の初期化
	setting_->Init();
}

// 更新
void Pause::Update(void)
{
	// ポーズモード中だったら選択処理できる
	if (pauseMode_)
	{

		isSetting_ = setting_->GetIsSetting();
		if (isSetting_)
		{
			// 設定時は設定の処理のみ受け付ける
			setting_->Update();
			return;
		}

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

	// ポーズモードのON・OFF
	StartPause();
}

// 描画
void Pause::Draw(void)
{

	if (pauseMode_)
	{

		if (isSetting_)
		{
			// 設定の描画
			setting_->Draw();
			return;
		}

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
		DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		switch (pause_)
		{
		case Pause::PAUSE::CONTINUE:
			//DrawRotaGraph(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2, 1.0, 0.0, images_[static_cast<int>(PAUSE::CONTINUE)], true);

			DrawString(CONTINUE_POS_X, CONTINUE_POS_Y, "Continue", 0x00ff00);
			DrawString(SETTING_POS_X, SETTING_POS_Y, "Setting", 0xffffff);
			DrawString(TITLE_POS_X, TITLE_POS_Y, "Title", 0xffffff);

			break;
		case Pause::PAUSE::SETTING:

			//DrawRotaGraph(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2, 1.0, 0.0, images_[static_cast<int>(PAUSE::TITLE)], true);

			DrawString(CONTINUE_POS_X, CONTINUE_POS_Y, "Continue", 0xffffff);
			DrawString(SETTING_POS_X, SETTING_POS_Y, "Setting", 0x00ff00);
			DrawString(TITLE_POS_X, TITLE_POS_Y, "Title", 0xffffff);

			break;
		case Pause::PAUSE::TITLE:

			//DrawRotaGraph(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2, 1.0, 0.0, images_[static_cast<int>(PAUSE::TITLE)], true);

			DrawString(CONTINUE_POS_X, CONTINUE_POS_Y, "Continue", 0xffffff);
			DrawString(SETTING_POS_X, SETTING_POS_Y, "Setting", 0xffffff);
			DrawString(TITLE_POS_X, TITLE_POS_Y, "Title", 0x00ff00);

			break;
		case Pause::PAUSE::NON:

			//DrawRotaGraph(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2, 1.0, 0.0, images_[static_cast<int>(PAUSE::NON)], true);

			DrawString(CONTINUE_POS_X, CONTINUE_POS_Y, "Continue", 0xffffff);
			DrawString(SETTING_POS_X, SETTING_POS_Y, "Setting", 0xffffff);
			DrawString(TITLE_POS_X, TITLE_POS_Y, "Title", 0xffffff);

			break;
		}

#ifdef _DEBUG

		DrawBox(pos_[static_cast<int>(PAUSE::CONTINUE)].x,
			pos_[static_cast<int>(PAUSE::CONTINUE)].y,
			pos_[static_cast<int>(PAUSE::CONTINUE)].x + COL_SIZE_X,
			pos_[static_cast<int>(PAUSE::CONTINUE)].y + COL_SIZE_Y, 0xff0000, false);

		DrawBox(pos_[static_cast<int>(PAUSE::SETTING)].x,
			pos_[static_cast<int>(PAUSE::SETTING)].y,
			pos_[static_cast<int>(PAUSE::SETTING)].x + COL_SIZE_X,
			pos_[static_cast<int>(PAUSE::SETTING)].y + COL_SIZE_Y, 0xff0000, false);

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
	if (setting_ != nullptr)
	{
		setting_->Release();
		delete setting_;
		setting_ = nullptr;
	}
}

void Pause::Confirm(void)
{

	InputManager& ins = InputManager::GetInstance();

	if (ins.Confirm())
	{
		switch (pause_)
		{
		case Pause::PAUSE::CONTINUE:

			pauseMode_ = false;

			// マウスの位置を真ん中に初期化する
			SetMousePoint(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2);
			SetMouseDispFlag(false);

			break;
		case Pause::PAUSE::SETTING:

			// 設定モードに入る
			setting_->SetIsSetting(true);

			break;
		case Pause::PAUSE::TITLE:

			SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);

			break;
		}

		// 決定SEをながす
		SoundManager::GetInstance().Play(SoundManager::SE::DECIDE);

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

		// マウスの位置を真ん中に初期化する
		SetMousePoint(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2);

		// SEをながす
		SoundManager::GetInstance().Play(SoundManager::SE::PAUSE);
	}
}

void Pause::Collision(void)
{
	auto prevPause = pause_;

	for (int i = 0; i < static_cast<int>(PAUSE::NON); i++)
	{
		if (CollisionUtility::RectangleAndMouse(pos_[i], COL_SIZE_X, COL_SIZE_Y))
		{
			ChangePause(static_cast<PAUSE>(i));
			break;
		}
		else
		{
			ChangePause(PAUSE::NON);
		}
	}

	if (pause_ != prevPause && pause_ != PAUSE::NON)
	{
		// 何も選択されていない状態から選択されたらSEを流す
		SoundManager::GetInstance().Play(SoundManager::SE::SELECT);
	}

}

void Pause::PadSelect(void)
{
	auto& ins = InputManager::GetInstance();
	auto prevPause = pause_;

	switch (pause_)
	{
	case Pause::PAUSE::CONTINUE:

		if (ins.SelectDown())
		{
			ChangePause(PAUSE::SETTING);
		}

		break;
	case Pause::PAUSE::SETTING:

		if (ins.SelectUp())
		{
			ChangePause(PAUSE::CONTINUE);
		}

		if (ins.SelectDown())
		{
			ChangePause(PAUSE::TITLE);
		}

		break;
	case Pause::PAUSE::TITLE:

		if (ins.SelectUp())
		{
			ChangePause(PAUSE::SETTING);
		}

		break;
	case Pause::PAUSE::NON:

		ChangePause(PAUSE::CONTINUE);

		break;
	default:
		break;
	}

	if (pause_ != prevPause && pause_ != PAUSE::NON)
	{
		// 何も選択されていない状態から選択されたらSEを流す
		SoundManager::GetInstance().Play(SoundManager::SE::SELECT);
	}
}

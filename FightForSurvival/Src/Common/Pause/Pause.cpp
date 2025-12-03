#include <DxLib.h>
#include "../../Manager/InputManager.h"
#include "../../Scene/SceneManager.h"
#include "../../Manager/SoundManager.h"
#include "../../Manager/SystemManager.h"
#include "../../Utility/Collision/CollisionUtility.h"
#include "Setting/Setting.h"
#include "../../UI/UIManager.h"
#include "../../UI/TextrueManager/TextureManager.h"
#include "../../UI/Object/Button/PauseButton/PauseButton.h"
#include "Pause.h"

// コンストラクタ
Pause::Pause(void)
	:
	setting_(nullptr)
{
	for (int i = 0; i < static_cast<int>(PAUSE::NON); i++)
	{
		isTrgDown_[i] = false;
	}
}

// デストラクタ
Pause::~Pause(void)
{
}

// ロード関連
void Pause::Load(void)
{
	// 設定のインスタンスを生成
	setting_ = new Setting();
	setting_->Load();

	// UI管理の生成処理
	uiMgr = new UIManager();
	texMgr = new TextureManager();

	// UIを生成
	UIBase* pauseContinue = UIFactory::GetInstance()->CreateUI(UI_KIND::PAUSE_CONTINUE, texMgr);
	UIBase* pauseSetting = UIFactory::GetInstance()->CreateUI(UI_KIND::PAUSE_SETTING, texMgr);
	UIBase* pauseTitle = UIFactory::GetInstance()->CreateUI(UI_KIND::PAUSE_TITLE, texMgr);

	// ポインタを渡す
	PauseButton* continueButton = dynamic_cast<PauseButton*>(pauseContinue);
	continueButton->SetNowPause(&pause_);
	PauseButton* settingButton = dynamic_cast<PauseButton*>(pauseSetting);
	settingButton->SetNowPause(&pause_);
	PauseButton* titleButton = dynamic_cast<PauseButton*>(pauseTitle);
	titleButton->SetNowPause(&pause_);


	// 生成したUIを追加
	uiMgr->AddUI(pauseContinue);
	uiMgr->AddUI(pauseSetting);
	uiMgr->AddUI(pauseTitle);
}

// 初期化
void Pause::Init(void)
{
	// ポーズモードの種類
	ChangePause(PAUSE::NON);

	// 座標初期化
	int i = static_cast<int>(PAUSE::CONTINUE);
	for (int y = 0; y < DRAW_NUM; y++)
	{
		pos_[i] = { POS_X, POS_Y + y * SPACE_Y };
		i++;
	}

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

		if (SystemManager::GetInstance().GetIsDevice())
		{
			// 引数の座標によって選択中のものを変化させる
			MouseSelect();
		}
		else
		{
			// 選択処理
			PadSelect();
		}

		// UIの更新
		uiMgr->Update();
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

		// 背景(透明)
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ALPHA);
		DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		// UIの描画
		uiMgr->Draw();


#ifdef _DEBUG

		// カメラのデバック描画

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
	// UIの解放
	delete uiMgr;
	delete texMgr;

	if (setting_ != nullptr)
	{
		setting_->Release();
		delete setting_;
		setting_ = nullptr;
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

void Pause::MouseSelect(void)
{
	auto prevPause = pause_;

	for (int i = 0; i < static_cast<int>(PAUSE::NON); i++)
	{

		if (CollisionUtility::RectangleAndMouse(pos_[i], COL_SIZE_X, COL_SIZE_Y))
		{
			ChangePause(static_cast<PAUSE>(i));

			if (InputManager::GetInstance().ConfirmUp() && isTrgDown_[i])
			{
				// 確定時の遷移処理
				Confirm();
			}
			if (InputManager::GetInstance().Confirm() && !isTrgDown_[i])
			{
				isTrgDown_[i] = true;
			}

			break;
		}
		else
		{
			ChangePause(PAUSE::NON);
			isTrgDown_[i] = false;
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

	if (ins.Confirm())
	{
		// 確定時の遷移処理
		Confirm();
	}
}

void Pause::Confirm(void)
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

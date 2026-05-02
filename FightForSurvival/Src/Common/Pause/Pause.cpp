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
	:setting_(nullptr)
	,uiMgr(nullptr)
	,texMgr(nullptr)
	,isSetting_(false)
	,pauseMode_(false)
	,pause_(PAUSE::NON)
{
	// ダウントリガー判定用
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
	// 感度設定のインスタンスを生成
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

	// ダウントリガー判定用
	for (int i = 0; i < static_cast<int>(PAUSE::NON); i++)
	{
		isTrgDown_[i] = false;
	}

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
		// 感度設定中か確認
		isSetting_ = setting_->GetIsSetting();

		// 感度設定中だった場合
		if (isSetting_)
		{
			// 設定時は設定の処理のみ受け付ける
			setting_->Update();
			return;
		}

		// 使用しているデバイスを確認
		if (SystemManager::GetInstance().GetIsDevice())
		{
			// マウスの選択処理
			MouseSelect();
		}
		else
		{
			// パッドの選択処理
			PadSelect();
		}

		// UIの更新
		uiMgr->Update();
	}

	// ポーズモードのON・OFF
	ChangePauseMode();
}

// 描画
void Pause::Draw(void)
{
	// ポーズモード中だったら
	if (pauseMode_)
	{

		// 感度設定中だったら
		if (isSetting_)
		{
			// 感度設定用の描画
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
		DebugDraw();
#endif // _DEBUG
	}

}

// 解放
void Pause::Release(void)
{
	// UIの解放
	delete uiMgr;
	delete texMgr;

	// 解放
	if (setting_ != nullptr)
	{
		setting_->Release();
		delete setting_;
		setting_ = nullptr;
	}
}

void Pause::ChangePauseMode(void)
{
	// ポーズモードボタンを押されたら
	if (InputManager::GetInstance().PauseKeys())
	{
		// trueだったらfalseへ　falseだったらtrueへ
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
	// 前の状態を保持しておく
	PAUSE prevPause = pause_;

	for (int i = 0; i < static_cast<int>(PAUSE::NON); i++)
	{

		// マウスと当たっていたら
		if (CollisionUtility::RectangleAndMouse(pos_[i], COL_SIZE_X, COL_SIZE_Y))
		{
			// 選択中のものの種類を設定
			ChangePause(static_cast<PAUSE>(i));

			// 確定ボタンを離した時かつ押下判定されていたら
			if (InputManager::GetInstance().ConfirmUp() && isTrgDown_[i])
			{
				// 確定時の遷移処理
				Confirm();
			}
			// 確定ボタンを押されたとき押下判定されていなかったら
			if (InputManager::GetInstance().Confirm() && !isTrgDown_[i])
			{
				isTrgDown_[i] = true;
			}

			break;
		}
		// 当たっていなかったら
		else
		{
			// NON状態にする
			ChangePause(PAUSE::NON);

			// 押下判定を初期化
			isTrgDown_[i] = false;
		}
	}

	// 選択中のものの変更が行われているかつ何かを選択していたら処理を行う
	CompPause(prevPause);

}

void Pause::PadSelect(void)
{
	// 前の状態を保持しておく
	PAUSE prevPause = pause_;

	switch (pause_)
	{
	case Pause::PAUSE::CONTINUE:

		// パッドの十字キーやスティックの下入力を検知したら
		if (InputManager::GetInstance().SelectDown())
		{
			// 選択中のものを変更
			ChangePause(PAUSE::SETTING);
		}

		break;
	case Pause::PAUSE::SETTING:

		// パッドの十字キーやスティックの上入力を検知したら
		if (InputManager::GetInstance().SelectUp())
		{
			// 選択中のものを変更
			ChangePause(PAUSE::CONTINUE);
		}

		// パッドの十字キーやスティックの下入力を検知したら
		if (InputManager::GetInstance().SelectDown())
		{
			// 選択中のものを変更
			ChangePause(PAUSE::TITLE);
		}

		break;
	case Pause::PAUSE::TITLE:

		// パッドの十字キーやスティックの上入力を検知したら
		if (InputManager::GetInstance().SelectUp())
		{
			// 選択中のものを変更
			ChangePause(PAUSE::SETTING);
		}

		break;
	case Pause::PAUSE::NON:

		// 最初はCONTINUEに設定される
		ChangePause(PAUSE::CONTINUE);

		break;
	default:
		break;
	}

	// 選択中のものの変更が行われているかつ何かを選択していたら処理を行う
	CompPause(prevPause);

	// 確定ボタンを押されたら
	if (InputManager::GetInstance().Confirm())
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

		// ポーズモードを終了させる
		pauseMode_ = false;

		// マウスの位置を真ん中に初期化する
		SetMousePoint(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2);

		// マウスの表示を消す
		SetMouseDispFlag(false);

		break;
	case Pause::PAUSE::SETTING:

		// 設定モードに入る
		setting_->SetIsSetting(true);

		break;
	case Pause::PAUSE::TITLE:

		// タイトルシーンへ遷移
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);

		break;
	}

	// 決定SEをながす
	SoundManager::GetInstance().Play(SoundManager::SE::DECIDE);
}

void Pause::CompPause(const PAUSE prevPause)
{
	// 何も選択されていない状態から選択されたら
	if (pause_ != prevPause && pause_ != PAUSE::NON)
	{
		// SEを流す
		SoundManager::GetInstance().Play(SoundManager::SE::SELECT);
	}
}

void Pause::DebugDraw(void)
{
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
}

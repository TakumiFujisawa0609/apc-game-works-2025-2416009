#include <DxLib.h>
#include "../../Scene/SceneManager.h"
#include "../../Manager/InputManager.h"
#include "../../Manager/EndManager.h"
#include "../../Manager/Camera.h"
#include "../../Utility/Collision/CollisionUtility.h"
#include "../../Manager/SoundManager.h"
#include "../../Manager/SystemManager.h"
#include "TitleScene.h"

#include "../../UI/UIManager.h"
#include "../../UI/TextrueManager/TextureManager.h"
#include "../../UI/Object/Bar/HPBar.h"
#include "../../UI/Object/Button/TitleButton/TitleButton.h"

TitleScene::TitleScene(void)
{
	camera_ = nullptr;

	state_ = CLICK;

	// マウスカーソルを表示
	SetMouseDispFlag(true);
}

TitleScene::~TitleScene(void)
{
}

void TitleScene::Load(void)
{
	// UI管理の生成処理
	uiMgr_ = new UIManager();
	texMgr_ = new TextureManager();

	// UIを生成
	UIBase* bg = UIFactory::GetInstance()->CreateUI(UI_KIND::TITLE_BG, texMgr_);
	UIBase* text = UIFactory::GetInstance()->CreateUI(UI_KIND::TITLE_TEXT, texMgr_);
	UIBase* cloud = UIFactory::GetInstance()->CreateUI(UI_KIND::CLOUD, texMgr_);
	UIBase* start = UIFactory::GetInstance()->CreateUI(UI_KIND::CLICK_TO_START, texMgr_);
	UIBase* gameStart = UIFactory::GetInstance()->CreateUI(UI_KIND::GAME_START, texMgr_);
	UIBase* end = UIFactory::GetInstance()->CreateUI(UI_KIND::END, texMgr_);

	// ポインタを渡す
	TitleButton* startButton = dynamic_cast<TitleButton*>(gameStart);
	startButton->SetNowState(&state_);
	TitleButton* endButton = dynamic_cast<TitleButton*>(end);
	endButton->SetNowState(&state_);


	// 生成したUIを追加
	uiMgr_->AddUI(bg);
	uiMgr_->AddUI(cloud);
	uiMgr_->AddUI(text);
	uiMgr_->AddUI(start);
	uiMgr_->AddUI(gameStart);
	uiMgr_->AddUI(end);
}

void TitleScene::Init(void)
{
	// カメラの初期化
	camera_ = new Camera();
	camera_->Init();

	// 座標初期化
	pos_[STATE::GAMESTART] = { GAMESTART_POS_X ,GAMESTART_POS_Y };
	pos_[STATE::END] = { EXIT_POS_X ,EXIT_POS_Y };

	// 動画再生タイミング用フレーム
	idleFrameCount_ = 0;

	// BGMをかける
	SoundManager::GetInstance().Play(SoundManager::BGM::TITLE);
}

void TitleScene::Update(void)
{
	if (state_ != CLICK)
	{
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

		// 確定処理
		Confirm();
	}

	if (InputManager::GetInstance().PushStartKey())
	{
		SoundManager::GetInstance().Play(SoundManager::SE::DECIDE);
		ChangeState(STATE::NON);
		for (auto ui : uiMgr_->GetUIList())
		{
			if (ui->GetUIKind() == UI_KIND::CLICK_TO_START)
			{
				// 描画を消す
				ui->SetIsDraw(false);
			}
			else if (ui->GetUIKind() == UI_KIND::GAME_START)
			{
				// 描画をさせる
				ui->SetIsDraw(true);
			}
			else if (ui->GetUIKind() == UI_KIND::END)
			{
				// 描画をさせる
				ui->SetIsDraw(true);
			}
		}
	}

	bool hasInput = false;
	if (InputManager::GetInstance().Confirm() ||
		CheckHitKeyAll() != 0)
	{
		// 操作があったらフラグを立てる
		hasInput = true;
	}

	if (hasInput)
	{
		// 何か操作があったらフレームカウントを0に初期化
		idleFrameCount_ = 0;
	}
	else
	{
		// フレームを進める
		idleFrameCount_++;
	}

	if (idleFrameCount_ > 60 * MOVIE_START_TIME) {
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::MOVIE);
		return; // ここで処理を抜けて動画専用の更新にする
	}

	// UIの更新
	uiMgr_->Update();

	// カメラの更新
	camera_->Update();
}

void TitleScene::Draw(void)
{
	// UIの描画
	uiMgr_->Draw();

	// カメラの設定
	camera_->SetBeforeDraw();

#ifdef _DEBUG

	// カメラのデバック描画
	camera_->DrawDebug();

	if (state_ != CLICK)
	{

		DrawBox(pos_[STATE::GAMESTART].x,
			pos_[STATE::GAMESTART].y,
			pos_[STATE::GAMESTART].x + COL_SIZE_X,
			pos_[STATE::GAMESTART].y + COL_SIZE_Y, 0xff0000, false);

		DrawBox(pos_[STATE::END].x,
			pos_[STATE::END].y,
			pos_[STATE::END].x + COL_SIZE_X,
			pos_[STATE::END].y + COL_SIZE_Y, 0xff0000, false);

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

	// UIの解放
	delete uiMgr_;
	delete texMgr_;

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
		case STATE::END:

			// ゲームを終了させる
			EndManager::GetInstance().SetIsEnd(true);

			break;
		}

		// 決定SEをながす
		SoundManager::GetInstance().Play(SoundManager::SE::DECIDE);

	}
}

void TitleScene::MouseSelect(void)
{
	auto prevState = state_;

	// 当たり判定取る
	for (int i = 0; i < static_cast<int>(STATE::NON); i++)
	{
		if (CollisionUtility::RectangleAndMouse(pos_[i], COL_SIZE_X, COL_SIZE_Y))
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
			ChangeState(STATE::END);
		}

		break;
	case TitleScene::END:

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

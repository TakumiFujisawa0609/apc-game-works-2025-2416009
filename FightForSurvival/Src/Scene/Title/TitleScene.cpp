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
	// UI管理の生成処理
	uiMgr = new UIManager();
	texMgr = new TextureManager();

	// UIを生成
	UIBase* bg = UIFactory::GetInstance()->CreateUI(UI_KIND::TITLE_BG, texMgr);
	UIBase* text = UIFactory::GetInstance()->CreateUI(UI_KIND::TITLE_TEXT, texMgr);
	UIBase* cloud = UIFactory::GetInstance()->CreateUI(UI_KIND::CLOUD, texMgr);
	UIBase* start = UIFactory::GetInstance()->CreateUI(UI_KIND::CLICK_TO_START, texMgr);
	UIBase* gameStart = UIFactory::GetInstance()->CreateUI(UI_KIND::GAME_START, texMgr);
	UIBase* end = UIFactory::GetInstance()->CreateUI(UI_KIND::END, texMgr);

	// 生成したUIを追加
	uiMgr->AddUI(bg);
	uiMgr->AddUI(cloud);
	uiMgr->AddUI(text);
	uiMgr->AddUI(start);
	uiMgr->AddUI(gameStart);
	uiMgr->AddUI(end);
}

void TitleScene::Init(void)
{
	// カメラの初期化
	camera_ = new Camera();
	camera_->Init();

	// ポーズモードの種類
	ChangeState(STATE::CLICK);

	// 座標初期化
	pos_[STATE::GAMESTART] = { GAMESTART_POS_X ,GAMESTART_POS_Y };
	pos_[STATE::EXIT] = { EXIT_POS_X ,EXIT_POS_Y };


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
		for (auto ui : uiMgr->GetUIList())
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

	// UIの更新
	uiMgr->Update();

	// カメラの更新
	camera_->Update();
}

void TitleScene::Draw(void)
{
	// UIの描画
	uiMgr->Draw();

	// カメラの設定
	camera_->SetBeforeDraw();

#ifdef _DEBUG

	// カメラのデバック描画
	camera_->DrawDebug();

	DrawBox(pos_[STATE::GAMESTART].x,
		pos_[STATE::GAMESTART].y,
		pos_[STATE::GAMESTART].x + COL_SIZE_X,
		pos_[STATE::GAMESTART].y + COL_SIZE_Y, 0xff0000, false);

	DrawBox(pos_[STATE::EXIT].x,
		pos_[STATE::EXIT].y,
		pos_[STATE::EXIT].x + COL_SIZE_X,
		pos_[STATE::EXIT].y + COL_SIZE_Y, 0xff0000, false);
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
	delete uiMgr;
	delete texMgr;

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

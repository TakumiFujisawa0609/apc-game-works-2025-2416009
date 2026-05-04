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
	// ステートがPushStartKeyの画面じゃなかったら
	if (state_ != CLICK)
	{

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

		// 確定処理
		Confirm();
	}

	// ステートによって描画物の表示の有無を変更
	StateDraw();

	if (StartMovie())
	{
		return; // ここで処理を抜けて動画専用の更新にする
	}

	// UIの更新
	uiMgr_->Update();
}

void TitleScene::Draw(void)
{
	// UIの描画
	uiMgr_->Draw();

#ifdef _DEBUG

	// デバッグ用描画
	DebugDraw();

#endif // _DEBUG

}

void TitleScene::Release(void)
{
	// UIの解放
	delete uiMgr_;
	delete texMgr_;

	// BGMを止める
	SoundManager::GetInstance().Stop(SoundManager::BGM::TITLE);
}

void TitleScene::Confirm(void)
{
	// 指定のキーを押されたら
	if (InputManager::GetInstance().Confirm())
	{
		switch (state_)
		{
		case STATE::GAMESTART:

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
	// 前の状態を保持しておく
	STATE prevState = state_;

	// 当たり判定取る
	for (int i = 0; i < static_cast<int>(STATE::NON); i++)
	{
		// マウスとGameStart・Endとの当たり判定を取る
		if (CollisionUtility::RectangleAndMouse(pos_[i], COL_SIZE_X, COL_SIZE_Y))
		{
			// 当たっているものにステートを変更
			ChangeState(static_cast<STATE>(i));

			break;
		}
		else
		{
			// 当たっていない場合はNONへ
			ChangeState(STATE::NON);
		}
	}

	// 何も選択されていない状態から選択されたらSEを流す
	PlaySelectSE(prevState);
}

void TitleScene::PadSelect(void)
{
	// 前の状態を保持しておく
	STATE prevState = state_;

	switch (state_)
	{
	case TitleScene::GAMESTART:

		// 指定のキーを押されたら
		if (InputManager::GetInstance().SelectDown())
		{
			// 状態をENDに変更
			ChangeState(STATE::END);
		}

		break;
	case TitleScene::END:

		// 指定のキーを押されたら
		if (InputManager::GetInstance().SelectUp())
		{
			// 状態をGAMESTARTに変更
			ChangeState(STATE::GAMESTART);
		}

		break;
	case TitleScene::NON:

		// パッドの場合NONだったら強制的にGameStartを選択するようにする
		ChangeState(STATE::GAMESTART);

		break;
	default:
		break;
	}

	// 何も選択されていない状態から選択されたらSEを流す
	PlaySelectSE(prevState);
}

void TitleScene::PlaySelectSE(STATE prevState)
{
	// 何も選択されていない状態から選択されたら
	if (state_ != prevState && state_ != STATE::NON)
	{
		// SEを流す
		SoundManager::GetInstance().Play(SoundManager::SE::SELECT);
	}
}

bool TitleScene::StartMovie(void)
{
	bool hasInput = false;

	// 操作しているかどうかを見る
	if (InputManager::GetInstance().Confirm() ||
		CheckHitKeyAll() != 0)
	{
		// 操作があったらフラグを立てる
		hasInput = true;
	}

	// 何か操作があったら
	if (hasInput)
	{
		// フレームカウントを0に初期化
		idleFrameCount_ = 0;
	}
	// 何も操作がが無かったら
	else
	{
		// フレームを進める
		idleFrameCount_++;
	}

	// カウントが指定の値よりも大きくなったら
	if (idleFrameCount_ > MOVIE_START_TIME) 
	{
		// シーンを動画シーンに変更
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::MOVIE);

		return true;
	}

	return false;
}

void TitleScene::StateDraw(void)
{
	// 指定のキーを押されたら
	if (InputManager::GetInstance().PushStartKey())
	{
		// SEを流す
		SoundManager::GetInstance().Play(SoundManager::SE::DECIDE);
		// NONにする
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
}

void TitleScene::DebugDraw(void)
{
	// GameStartとEndが選択できる時だったら
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
}

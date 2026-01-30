#include "../../Manager/InputManager.h"
#include "../../Scene/SceneManager.h"
#include "../../Manager/SystemManager.h"
#include "../../Manager/SoundManager.h"
#include "../../Application.h"
#include "../../UI/Object/Sprite/PendulumSprite/PendulumSprite.h"
#include "ClearScene.h"

#include "../../UI/UIManager.h"
#include "../../UI/TextrueManager/TextureManager.h"

ClearScene::ClearScene(void)
{

}

ClearScene::~ClearScene(void)
{
}

void ClearScene::Load(void)
{
	// UI管理の生成処理
	uiMgr_ = new UIManager();
	texMgr_ = new TextureManager();

	// UIを生成
	UIBase* gameClear = UIFactory::GetInstance()->CreateUI(UI_KIND::CLEAR_SPRITE, texMgr_);
	UIBase* bg = UIFactory::GetInstance()->CreateUI(UI_KIND::CLEAR_BG, texMgr_);
	UIBase* scoreBoard = UIFactory::GetInstance()->CreateUI(UI_KIND::SCORE_BOARD, texMgr_);
	UIBase* rankS = UIFactory::GetInstance()->CreateUI(UI_KIND::RANK_S, texMgr_);
	UIBase* rankA = UIFactory::GetInstance()->CreateUI(UI_KIND::RANK_A, texMgr_);
	UIBase* rankB = UIFactory::GetInstance()->CreateUI(UI_KIND::RANK_B, texMgr_);
	UIBase* rankC = UIFactory::GetInstance()->CreateUI(UI_KIND::RANK_C, texMgr_);
	UIBase* zonbieSprite = UIFactory::GetInstance()->CreateUI(UI_KIND::CLEAR_ZOMBIE, texMgr_);
	UIBase* batSprite = UIFactory::GetInstance()->CreateUI(UI_KIND::CLEAR_BAT, texMgr_);
	UIBase* dragonSprite = UIFactory::GetInstance()->CreateUI(UI_KIND::CLEAR_DRAGON, texMgr_);

	// ダウンキャストして PendulumSprite固有の関数を呼べるようにする
	PendulumSprite* zonbie = dynamic_cast<PendulumSprite*>(zonbieSprite);
	zonbie->SetIsLeft(false);

	score_ = SystemManager::GetInstance().GetScore();

	if (score_ < 5000)
	{
		// ダウンキャストして Sprite固有の関数を呼べるようにする
		Sprite* rankCSprite = dynamic_cast<Sprite*>(rankC);
		rankCSprite->SetIsDraw(true);
	}
	else if (score_ < 10000)
	{
		// ダウンキャストして Sprite固有の関数を呼べるようにする
		Sprite* rankBSprite = dynamic_cast<Sprite*>(rankB);
		rankBSprite->SetIsDraw(true);
	}
	else if (score_ < 18000)
	{
		// ダウンキャストして Sprite固有の関数を呼べるようにする
		Sprite* rankASprite = dynamic_cast<Sprite*>(rankA);
		rankASprite->SetIsDraw(true);
	}
	else
	{
		// ダウンキャストして Sprite固有の関数を呼べるようにする
		Sprite* rankSSprite = dynamic_cast<Sprite*>(rankS);
		rankSSprite->SetIsDraw(true);
	}

	// 生成したUIを追加
	uiMgr_->AddUI(bg);
	uiMgr_->AddUI(gameClear);
	uiMgr_->AddUI(scoreBoard);
	uiMgr_->AddUI(rankS);
	uiMgr_->AddUI(rankA);
	uiMgr_->AddUI(rankB);
	uiMgr_->AddUI(rankC);
	uiMgr_->AddUI(zonbieSprite);
	uiMgr_->AddUI(batSprite);
	uiMgr_->AddUI(dragonSprite);

	// 設定付きフォントデータロード
	font_[static_cast<int>(Font::BIG)] = CreateFontToHandle("x12y12pxMaruMinya", 60, 20, DX_FONTTYPE_ANTIALIASING);
	font_[static_cast<int>(Font::SMALL)] = CreateFontToHandle("x12y12pxMaruMinya", 50, 20, DX_FONTTYPE_ANTIALIASING);

}

void ClearScene::Init(void)
{
	// BGMをかける
	SoundManager::GetInstance().Play(SoundManager::BGM::CLEAR);
}

void ClearScene::Update(void)
{
	if (InputManager::GetInstance().PushStartKey())
	{
		// タイトルに遷移させる
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
		// 決定SE流す
		SoundManager::GetInstance().Play(SoundManager::SE::DECIDE);
	}

	// UIの更新
	uiMgr_->Update();
}

void ClearScene::Draw(void)
{
	// UIの描画
	uiMgr_->Draw();

	int POS_X = Application::SCREEN_SIZE_X / 2;
	int POS_Y = Application::SCREEN_SIZE_Y / 2;
	DrawStringToHandle(POS_X - 150, POS_Y - 50, "最終スコア", 0x000000, font_[static_cast<int>(Font::BIG)]);

	int score = score_;
	int count = 0;
	while (score / 10 !=0)
	{
		score /= 10;
		count++;
	}

	DrawFormatStringToHandle(POS_X + 60 - (count * 20), POS_Y + 90, 0x000000, font_[static_cast<int>(Font::SMALL)], "%d", score_);
}

void ClearScene::Release(void)
{
	// UIの解放
	delete uiMgr_;
	delete texMgr_;

	// BGMを止める
	SoundManager::GetInstance().Stop(SoundManager::BGM::CLEAR);

	DeleteFontToHandle(font_[static_cast<int>(Font::BIG)]);
	DeleteFontToHandle(font_[static_cast<int>(Font::SMALL)]);
}

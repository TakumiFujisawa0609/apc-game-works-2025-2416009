#include "../../Manager/InputManager.h"
#include "../../Scene/SceneManager.h"
#include "../../Manager/SystemManager.h"
#include "../../Manager/SoundManager.h"
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

	// スコアを受け取る
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
	// 指定のキーを押されたら
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

	// スコアを桁ごとに分ける
	int score = score_;
	int count = 0;
	while (score / DIGIT !=0)
	{
		score /= DIGIT;
		count++;
	}

	DrawStringToHandle(FONT_BIG_POS_X, FONT_BIG_POS_Y, "最終スコア", FONT_COLOR, font_[static_cast<int>(Font::BIG)]);
	// スコア表示
	DrawFormatStringToHandle(FONT_SMALL_POS_X - (count * DISTANCE), FONT_SMALL_POS_Y, FONT_COLOR, font_[static_cast<int>(Font::SMALL)], "%d", score_);
}

void ClearScene::Release(void)
{
	// UIの解放
	delete uiMgr_;
	delete texMgr_;

	// BGMを止める
	SoundManager::GetInstance().Stop(SoundManager::BGM::CLEAR);

	// フォントハンドルを解放
	DeleteFontToHandle(font_[static_cast<int>(Font::BIG)]);
	DeleteFontToHandle(font_[static_cast<int>(Font::SMALL)]);
}

#include "../../Manager/InputManager.h"
#include "../../Scene/SceneManager.h"
#include "../../Manager/SystemManager.h"
#include "../../Manager/SoundManager.h"
#include "../../Application.h"
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
	UIBase* bg = UIFactory::GetInstance()->CreateUI(UI_KIND::CLEAR_BG, texMgr_);
	UIBase* scoreBoard = UIFactory::GetInstance()->CreateUI(UI_KIND::SCORE_BOARD, texMgr_);
	UIBase* rankS = UIFactory::GetInstance()->CreateUI(UI_KIND::RANK_S, texMgr_);
	UIBase* rankA = UIFactory::GetInstance()->CreateUI(UI_KIND::RANK_A, texMgr_);
	UIBase* rankB = UIFactory::GetInstance()->CreateUI(UI_KIND::RANK_B, texMgr_);
	UIBase* rankC = UIFactory::GetInstance()->CreateUI(UI_KIND::RANK_C, texMgr_);
	UIBase* zonbie = UIFactory::GetInstance()->CreateUI(UI_KIND::CLEAR_ZOMBIE, texMgr_);
	UIBase* bat = UIFactory::GetInstance()->CreateUI(UI_KIND::CLEAR_BAT, texMgr_);
	UIBase* dragon = UIFactory::GetInstance()->CreateUI(UI_KIND::CLEAR_DRAGON, texMgr_);

	// 生成したUIを追加
	uiMgr_->AddUI(bg);
	uiMgr_->AddUI(scoreBoard);
	uiMgr_->AddUI(rankS);
	uiMgr_->AddUI(rankA);
	uiMgr_->AddUI(rankB);
	uiMgr_->AddUI(rankC);
	uiMgr_->AddUI(zonbie);
	uiMgr_->AddUI(bat);
	uiMgr_->AddUI(dragon);

	// 設定付きフォントデータロード
	font_[static_cast<int>(Font::BIG)] = CreateFontToHandle("x12y12pxMaruMinya", 32, 20, DX_FONTTYPE_ANTIALIASING);
	font_[static_cast<int>(Font::SMALL)] = CreateFontToHandle("x12y12pxMaruMinya", 20, 20, DX_FONTTYPE_ANTIALIASING);

}

void ClearScene::Init(void)
{
	score_ = SystemManager::GetInstance().GetScore();

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
	DrawStringToHandle(POS_X - 50, POS_Y - 80, "GameClear!", 0x000000, font_[static_cast<int>(Font::BIG)]);
	DrawStringToHandle(POS_X - 50, POS_Y - 50, "最終スコア", 0x000000, font_[static_cast<int>(Font::SMALL)]);
	DrawFormatStringToHandle(POS_X, POS_Y, 0x000000, font_[static_cast<int>(Font::SMALL)], "%d", score_);
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

#include "../../Manager/InputManager.h"
#include "../../Scene/SceneManager.h"
#include "../../Manager/SystemManager.h"
#include "../../Manager/SoundManager.h"
#include "../../Application.h"
#include "OverScene.h"

#include "../../UI/UIManager.h"
#include "../../UI/TextrueManager/TextureManager.h"

OverScene::OverScene(void)
{
}

OverScene::~OverScene(void)
{
}

void OverScene::Load(void)
{
	// UI管理の生成処理
	uiMgr_ = new UIManager();
	texMgr_ = new TextureManager();

	UIBase* gameOver = UIFactory::GetInstance()->CreateUI(UI_KIND::OVER_SPRITE, texMgr_);
	UIBase* bg = UIFactory::GetInstance()->CreateUI(UI_KIND::OVER_BG, texMgr_);

	// 生成したUIを追加
	uiMgr_->AddUI(bg);
	uiMgr_->AddUI(gameOver);

	// 設定付きフォントデータロード
	font_[static_cast<int>(Font::BIG)] = CreateFontToHandle("x12y12pxMaruMinya", 50, 20, DX_FONTTYPE_ANTIALIASING);
	font_[static_cast<int>(Font::SMALL)] = CreateFontToHandle("x12y12pxMaruMinya", 40, 20, DX_FONTTYPE_ANTIALIASING);
}

void OverScene::Init(void)
{
	// スコアを受け取る
	score_ = SystemManager::GetInstance().GetScore();

	// BGMをかける
	SoundManager::GetInstance().Play(SoundManager::BGM::OVER);
}

void OverScene::Update(void)
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

void OverScene::Draw(void)
{
	// UIの描画
	uiMgr_->Draw();

	// スコアを桁ごとに分ける
	int score = score_;
	int count = 0;
	while (score / DIGIT != 0)
	{
		score /= DIGIT;
		count++;
	}

	DrawStringToHandle(FONT_BIG_POS_X, FONT_BIG_POS_Y, "最終スコア", 0xffffff, font_[static_cast<int>(Font::BIG)]);
	// スコア表示
	DrawFormatStringToHandle(FONT_SMALL_POS_X - (count * DISTANCE), FONT_SMALL_POS_Y, 0xffffff, font_[static_cast<int>(Font::SMALL)], "%d", score_);
}

void OverScene::Release(void)
{
	// UIの解放
	delete uiMgr_;
	delete texMgr_;

	// BGMを止める
	SoundManager::GetInstance().Stop(SoundManager::BGM::OVER);

	// フォントハンドルの解放
	DeleteFontToHandle(font_[static_cast<int>(Font::BIG)]);
	DeleteFontToHandle(font_[static_cast<int>(Font::SMALL)]);
}

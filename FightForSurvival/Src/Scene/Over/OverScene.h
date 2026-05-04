#pragma once

#include "../Base/SceneBase.h"

class UIManager;
class TextureManager;

class OverScene : public SceneBase
{
public:

	// BIGフォントの時の座標
	static constexpr int FONT_BIG_POS_X = Application::SCREEN_SIZE_X / 2 - 150;
	static constexpr int FONT_BIG_POS_Y = Application::SCREEN_SIZE_Y / 2;

	// SMALLフォントの時の座標
	static constexpr int FONT_SMALL_POS_X = Application::SCREEN_SIZE_X / 2 + 60;
	static constexpr int FONT_SMALL_POS_Y = Application::SCREEN_SIZE_Y / 2 + 90;

	// 桁ごとの間の距離
	static constexpr int DISTANCE = 20;

	// 桁数を分ける
	static constexpr int DIGIT = 10;

	// フォントの大きさ
	enum class Font
	{
		BIG,
		SMALL,

		MAX
	};

	// コンストラクタ
	OverScene(void);
	// デストラクタ
	~OverScene(void)override;

	// 読み込み処理
	void Load(void) override;
	// 初期化処理
	void Init(void) override;
	// 更新処理
	void Update(void) override;
	// 描画処理
	void Draw(void) override;
	// 解放処理
	void Release(void) override;

private:

	// UI
	UIManager* uiMgr_;
	TextureManager* texMgr_;

	// フォントハンドル
	int font_[static_cast<int>(Font::MAX)];

	// スコア
	int score_;

};


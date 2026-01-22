#pragma once

#include "../Base/SceneBase.h"

class UIManager;
class TextureManager;

class OverScene : public SceneBase
{
public:

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

	void Load(void) override;
	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
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


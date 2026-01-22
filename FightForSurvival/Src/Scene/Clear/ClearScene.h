#pragma once

#include "../Base/SceneBase.h"

class UIManager;
class TextureManager;

class ClearScene : public SceneBase
{
public:

	enum class Font
	{
		BIG,
		SMALL,

		MAX
	};

	// コンストラクタ
	ClearScene(void);

	// デストラクタ
	~ClearScene(void)override;

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

	// スコアを格納するための変数
	int score_;

};

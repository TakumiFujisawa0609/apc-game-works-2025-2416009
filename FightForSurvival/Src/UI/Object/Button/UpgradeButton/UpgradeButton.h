#pragma once

#include "../Button.h"

class UpgradeButton:public Button
{
public:

	enum TextKind
	{
		HP_UP,
		HP_HEAL,
		SPEED_UP,
		STAMINA_UP,
		POTION_RESTOC,
	};

	// コンストラクタ
	UpgradeButton(const UIResourceInfo& info, TextureManager* texMgr);

	// デストラクタ
	~UpgradeButton();

	void Update(void) override;		// 更新
	void Draw(void) override;		// 描画

	// ボタンの状態を返す
	BUTTON_STATE GetButtonState(void) const { return buttonState_; }

private:

	TextureManager* texMgr_;

	BUTTON_STATE buttonState_;		// ボタンの状態

	int handle_[MAX];	// 画像ハンドル
};


#pragma once

#include "../UIBase.h"
#include "../../UIInfo.h"

// クラスの前方宣言
class TextureManager;

// ベースを継承
class Button : public UIBase
{
public:
	// ボタンの状態
	enum BUTTON_STATE
	{
		DEFAULE,		// 通常
		HOVER,			// 選択状態
		TRIGGER_DOWN,	// ボタン押下
		TRIGGER_UP,		// ボタン離し

		MAX,		// 最大数
	};

public:
	// コンストラクタ
	Button(const UIResourceInfo& info, TextureManager* texMgr);

	// デストラクタ
	~Button();

	void Update(void) override;		// 更新
	void Draw(void) override;		// 描画

	// ボタンの状態を返す
	BUTTON_STATE GetButtonState(void) const { return buttonState_; }

private:
	
	BUTTON_STATE buttonState_;		// ボタンの状態

	int handle_[MAX];	// 画像ハンドル

};

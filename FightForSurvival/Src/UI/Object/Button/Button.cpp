#include "Button.h"

#include <DxLib.h>

#include "../../TextrueManager/TextureManager.h"

Button::Button(const UIResourceInfo& info, TextureManager* texMgr)
	:
	buttonState_(DEFAULE)
{
	x_ = info.x;
	y_ = info.y;
	w_ = info.width;
	h_ = info.height;

	// ロード
	handle_[DEFAULE] = texMgr->LoadTexture(info.basePath);
	handle_[HOVER] = texMgr->LoadTexture(info.mainPath);
	handle_[TRIGGER_DOWN] = texMgr->LoadTexture(info.framePath);
	handle_[TRIGGER_UP] = texMgr->LoadTexture(info.framePath);
}

Button::~Button()
{
}

void Button::Update(void)
{
	// マウスの座標取得用変数
	int mousePosX = 0;
	int mousePosY = 0;

	// マウス座標を取得
	GetMousePoint(&mousePosX, &mousePosY);

	// ボタンの上にマウスポインタがあるか？
	if (x_< mousePosX &&
		x_+ w_ > mousePosX &&
		y_< mousePosY &&
		y_+ h_ > mousePosY)
	{
		// ボタンの上にマウスポインタがある
		buttonState_ = HOVER;

		// クリックされているか？
		if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0)
		{
			// クリックされている
			buttonState_ = TRIGGER_DOWN;
		}
		else
		{
			// 前フレームでボタン内でクリックした
			if (buttonState_ == TRIGGER_DOWN)
			{
				// ボタン内で離した
				buttonState_ = TRIGGER_UP;
			}
		}
	}
	// ボタンの外にマウスポインタがある
	else
	{
		// デフォルト状態
		buttonState_ = DEFAULE;
	}
}

void Button::Draw(void)
{
	// ボタンの描画が各状態によって変化
	DrawGraph(x_, y_, handle_[static_cast<int>(buttonState_)], true);
}

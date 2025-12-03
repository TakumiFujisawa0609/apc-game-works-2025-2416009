#include "Button.h"

#include <DxLib.h>

#include "../../TextrueManager/TextureManager.h"
#include "../../../Manager/InputManager.h"

Button::Button(const UIResourceInfo& info, TextureManager* texMgr)
	:
	buttonState_(DEFAULE)
{
	uiKind_ = info.uiKind;
	isDraw_ = info.isDraw;
	x_ = info.x;
	y_ = info.y;
	w_ = info.width;
	h_ = info.height;

	isTrgDown_ = false;

	// ロード
	handle_[DEFAULE] = texMgr->LoadTexture(info.basePath);
	handle_[HOVER] = texMgr->LoadTexture(info.mainPath);
	handle_[TRIGGER_DOWN] = texMgr->LoadTexture(info.framePath);
	handle_[TRIGGER_UP] = texMgr->LoadTexture(info.framePath);
	textHandle_ = texMgr->LoadTexture(info.textPath);
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
		x_ + w_ > mousePosX &&
		y_< mousePosY &&
		y_ + h_ > mousePosY)
	{

		// クリックされているか？
		if (InputManager::GetInstance().Confirm() && !isTrgDown_)
		{
			isTrgDown_ = true;
		}

		if (isTrgDown_)
		{
			buttonState_ = TRIGGER_DOWN;
		}
		else
		{
			buttonState_ = HOVER;
		}
		
	}
	else
	{
		buttonState_ = DEFAULE;
		isTrgDown_ = false;
	}

}

void Button::Draw(void)
{
	if (!isDraw_)
	{
		return;
	}

	// ボタンの描画が各状態によって変化
	DrawGraph(x_, y_, handle_[static_cast<int>(buttonState_)], true);
	DrawGraph(x_, y_, textHandle_, true);
}

#include "Sprite.h"

#include <DxLib.h>

#include "../../TextrueManager/TextureManager.h"

Sprite::Sprite(const UIResourceInfo& info, TextureManager* texMgr)
{
	uiKind_ = info.uiKind;
	isDraw_ = info.isDraw;
	x_ = info.x;
	y_ = info.y;
	w_ = info.width;
	h_ = info.height;


	// ƒ[ƒh
	handle_= texMgr->LoadTexture(info.basePath);
}

Sprite::~Sprite()
{
}

void Sprite::Update(void)
{
	if (!isDraw_)
	{
		return;
	}
}

void Sprite::Draw(void)
{
	if (!isDraw_)
	{
		return;
	}

	DrawExtendGraph(x_, y_, x_ + w_, y_ + h_ ,handle_, true);
}

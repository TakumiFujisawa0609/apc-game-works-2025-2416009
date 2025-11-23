#include "Sprite.h"

#include <DxLib.h>

#include "../../TextrueManager/TextureManager.h"

Sprite::Sprite(const UIResourceInfo& info, TextureManager* texMgr)
{
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
}

void Sprite::Draw(void)
{
	DrawGraph(x_, y_, handle_, true);
}

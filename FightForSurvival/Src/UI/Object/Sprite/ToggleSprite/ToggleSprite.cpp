#include "ToggleSprite.h"

ToggleSprite::ToggleSprite(const UIResourceInfo& info, TextureManager* texMgr)
	:Sprite(info, texMgr),
	draw1Flg_(true)
{
}

ToggleSprite::~ToggleSprite()
{
}

void ToggleSprite::Update(void)
{
}

void ToggleSprite::Draw(void)
{
	if (draw1Flg_)
	{

	}
	else
	{

	}
}

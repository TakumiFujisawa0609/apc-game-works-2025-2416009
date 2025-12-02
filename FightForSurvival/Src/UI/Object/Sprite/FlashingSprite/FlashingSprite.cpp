#include "FlashingSprite.h"

#include <DxLib.h>

#include "../../../TextrueManager/TextureManager.h"

FlashingSprite::FlashingSprite(const UIResourceInfo& info, TextureManager* texMgr)
	:Sprite(info,texMgr)
{
	alpha_ = ALPHA_MAX;
	isAlpha_ = true;
}

FlashingSprite::~FlashingSprite()
{
}

void FlashingSprite::Update(void)
{
	Sprite::Update();

	// アルファ値を変える
	if (isAlpha_)
	{

		alpha_ += ALPHA_SPEED;

		if (alpha_ > ALPHA_MAX)
		{
			alpha_ = ALPHA_MAX;
			isAlpha_ = false;
		}
	}
	else
	{
		// アルファ値を変える
		alpha_ -= ALPHA_SPEED;

		if (alpha_ < 0.0f)
		{
			alpha_ = 0.0f;
			isAlpha_ = true;
		}

	}

}

void FlashingSprite::Draw(void)
{
	if (!isDraw_)
	{
		return;
	}

	// アルファ値を変える
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(alpha_));
	DrawExtendGraph(x_, y_, x_ + w_, y_ + h_, handle_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

}

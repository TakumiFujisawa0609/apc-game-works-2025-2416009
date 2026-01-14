#include "PendulumSprite.h"


#include "../../../TextrueManager/TextureManager.h"

PendulumSprite::PendulumSprite(const UIResourceInfo& info, TextureManager* texMgr)
	:Sprite(info, texMgr)
{
	angle_ = 0.0f;
	isLeft_ = true;
}

PendulumSprite::~PendulumSprite()
{
}

void PendulumSprite::Update(void)
{
	Sprite::Update();

	// 回転の左右を変更する
	if (isLeft_)
	{

		angle_ -= ROTATION_SPEED;

		if (angle_ < ANGLE_MIN)
		{
			angle_ = ANGLE_MIN;
			isLeft_ = false;
		}
	}
	else
	{
		// アルファ値を変える
		angle_ += ROTATION_SPEED;

		if (angle_ > ANGLE_MAX)
		{
			angle_ = ANGLE_MAX;
			isLeft_ = true;
		}

	}
}

void PendulumSprite::Draw(void)
{
	if (!isDraw_)
	{
		return;
	}

	DrawRotaGraph(x_, y_, 1.0, angle_,handle_, true);
}

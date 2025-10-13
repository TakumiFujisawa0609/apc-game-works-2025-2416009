#include <DxLib.h>
#include "../../Application.h"
#include "RedDamageEffect.h"

RedDamageEffect::RedDamageEffect(void)
{
}

RedDamageEffect::~RedDamageEffect(void)
{
}

void RedDamageEffect::Load(void)
{
}

void RedDamageEffect::Init(void)
{
	alpha_ = 0;
}

void RedDamageEffect::Update(void)
{
	if (alpha_ > 0)
	{
		alpha_--;
		if (alpha_ <= 0)
		{
			alpha_ = 0;
		}
	}
}

void RedDamageEffect::Draw(void)
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha_);
	DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, 0xff0000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void RedDamageEffect::SetRedEffect(void)
{
	alpha_ = 128;
}

#include <DxLib.h>
#include "../../Application.h"
#include "Reticle.h"

Reticle::Reticle(void)
	:img_(-1)
{
}

Reticle::~Reticle(void)
{
}

void Reticle::Load(void)
{
	// ‰æ‘œ‚Ì“Ç‚İ‚İ
	img_ = LoadGraph((Application::PATH_IMAGE + "UI/cursor.png").c_str());
}

void Reticle::Init(void)
{
	// À•Wİ’è
	pos_.x = Application::SCREEN_SIZE_X / 2;
	pos_.y = Application::SCREEN_SIZE_Y / 2;
}

void Reticle::Draw(void)
{
	// •`‰æ
	DrawRotaGraph(pos_.x, pos_.y, 0.7, 0.0, img_, true);
}

void Reticle::Release(void)
{
	if (img_ != -1)
	{
		// ’†g‚ª“ü‚Á‚Ä‚¢‚½‚çíœ
		DeleteGraph(img_);
	}
}

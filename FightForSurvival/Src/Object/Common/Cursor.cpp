#include <DxLib.h>
#include "../../Application.h"
#include "Cursor.h"

Cursor::Cursor(void)
{
	img_ = -1;
}

Cursor::~Cursor(void)
{
}

void Cursor::Load(void)
{
	img_ = LoadGraph((Application::PATH_IMAGE + "cursor.png").c_str());
}

void Cursor::Init(void)
{
	// À•Wİ’è
	pos_.x = Application::SCREEN_SIZE_X / 2;
	pos_.y = Application::SCREEN_SIZE_Y / 2;
}

void Cursor::Draw(void)
{
	// •`‰æ
	DrawRotaGraph(pos_.x, pos_.y, 0.7, 0.0, img_, true);
}

void Cursor::Release(void)
{
	if (img_ != -1)
	{
		// ’†g‚ª“ü‚Á‚Ä‚¢‚½‚çíœ
		DeleteGraph(img_);
	}
}

#include <DxLib.h>
#include "../../Manager/SystemManager.h"
#include "Score.h"

Score::Score(void)
{
}

Score::~Score(void)
{
}

void Score::Load(void)
{
	font_= CreateFontToHandle("x12y12pxMaruMinya", 32, 1, DX_FONTTYPE_ANTIALIASING);;
}

void Score::Init(void)
{
	pos_.x = POS_X;
	pos_.y = POS_Y;
}

void Score::Draw(void)
{
	DrawFormatStringToHandle(pos_.x, pos_.y, 0xffffff, font_, "Score : %d", SystemManager::GetInstance().GetScore());
}

void Score::Release(void)
{
	DeleteFontToHandle(font_);
}

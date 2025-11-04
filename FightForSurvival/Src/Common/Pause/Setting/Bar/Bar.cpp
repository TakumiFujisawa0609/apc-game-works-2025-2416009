#include <DxLib.h>
#include "Bar.h"

Bar::Bar(void)
{
}

Bar::~Bar(void)
{
}

void Bar::Init(void)
{
	SetParam();

	dis_ = max_ - min_;
	value_ = size_.x / dis_;
}

void Bar::Draw(void)
{
	auto j = value_ * now_;

	DrawBox(pos_.x, pos_.y, j, size_.y, 0xffffff, true);
}

void Bar::Release(void)
{
}

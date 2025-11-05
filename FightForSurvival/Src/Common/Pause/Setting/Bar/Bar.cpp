#include <DxLib.h>
#include "Bar.h"

Bar::Bar(Vector2 pos, Vector2 size, unsigned int color, float max, float min, float now)
	:
	pos_(pos),
	size_(size),
	color_(color),
	max_(max),
	min_(min),
	now_(now)
{
}

Bar::~Bar(void)
{
}

void Bar::Init(void)
{
	range_ = max_ - min_;
	pixelBerUnit_ = range_ / size_.x;
}

void Bar::Draw(void)
{
	auto j = pixelBerUnit_ * now_;

	DrawBox(pos_.x, pos_.y, pos_.x + j, pos_.y + size_.y, color_, true);
}

void Bar::Release(void)
{
}

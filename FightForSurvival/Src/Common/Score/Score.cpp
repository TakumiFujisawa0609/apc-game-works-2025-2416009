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
}

void Score::Init(void)
{
	pos_.x = POS_X;
	pos_.y = POS_Y;
}

void Score::Draw(void)
{
	DrawFormatString(pos_.x, pos_.y, 0xffffff, "Score : %d", SystemManager::GetInstance().GetScore());
}

void Score::Release(void)
{

}

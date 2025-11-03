#include "FireMagic.h"

FireMagic::FireMagic(int baseModelId) :MagicBase(baseModelId)
{
}

FireMagic::~FireMagic(void)
{
}

void FireMagic::SetParam(void)
{
	magic_.scale_ = SCALE;
	magic_.rotate_ = ROTATE;

	magic_.speed_ = SPEED;
	magic_.cntAlive_ = CNT_ALIVE;
	magic_.collisionRadius_ = COLLISION_RAD;
	magic_.headDamage_ = HEAD_DAMAGE;
	magic_.bodyDamage_ = BODY_DAMAGE;
}

#include "ExplosionMagic.h"

ExplosionMagic::ExplosionMagic(int baseModelId)
	:
	MagicBase(baseModelId)
{
}

ExplosionMagic::~ExplosionMagic(void)
{
}

void ExplosionMagic::SetParam(void)
{
	magic_.scale_ = SCALE;
	magic_.rotate_ = ROTATE;

	magic_.speed_ = SPEED;
	magic_.cntAlive_ = CNT_ALIVE;
	magic_.collisionRadius_ = COLLISION_RAD;
	magic_.headDamage_ = HEAD_DAMAGE;
	magic_.bodyDamage_ = BODY_DAMAGE;
}

void ExplosionMagic::ChangeShot(void)
{
}

void ExplosionMagic::ChangeBlast(void)
{
}

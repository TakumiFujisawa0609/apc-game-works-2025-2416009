#include "BatMagic.h"

BatMagic::BatMagic(TYPE_MAGIC typeMagic, int baseModelId) :MagicBase(typeMagic, baseModelId)
{
}

BatMagic::~BatMagic(void)
{
}

void BatMagic::SetParam(void)
{
	magic_.scale_ = SCALE;
	magic_.rotate_ = ROTATE;

	magic_.speed_ = SPEED;
	magic_.cntAlive_ = CNT_ALIVE;
	magic_.collisionRadius_ = COLLISION_RAD;
	magic_.bodyDamage_ = DAMAGE;
}

void BatMagic::ChangeShot(void)
{
}

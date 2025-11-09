#include "DragonMagic.h"

DragonMagic::DragonMagic(TYPE_MAGIC typeMagic, int baseModelId) :MagicBase(typeMagic, baseModelId)
{
}

DragonMagic::~DragonMagic(void)
{
}

void DragonMagic::SetParam(void)
{
	magic_.scale_ = SCALE;
	magic_.rotate_ = ROTATE;

	magic_.speed_ = SPEED;
	magic_.cntAlive_ = CNT_ALIVE;
	magic_.collisionRadius_ = COLLISION_RAD;
	magic_.bodyDamage_ = DAMAGE;
}

void DragonMagic::ChangeShot(void)
{
}

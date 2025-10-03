#include "../../../../Utility/AsoUtility.h"
#include "FireMagic.h"

FireMagic::FireMagic(int baseModelId) :MagicBase(baseModelId)
{
}

FireMagic::~FireMagic(void)
{
}

void FireMagic::SetParam(void)
{
	Magic_.scale_ = AsoUtility::VECTOR_ONE;
	Magic_.rotate_ = AsoUtility::VECTOR_ZERO;

	Magic_.speed_ = SPEED;
	Magic_.cntAlive_ = CNT_ALIVE;
	Magic_.collisionRadius_ = COLLISION_RAD;
	Magic_.headDamage_ = HEAD_DAMAGE;
	Magic_.bodyDamage_ = BODY_DAMAGE;
}

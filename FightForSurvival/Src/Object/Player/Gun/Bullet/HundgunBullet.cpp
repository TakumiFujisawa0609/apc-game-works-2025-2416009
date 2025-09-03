#include "../../../../Utility/AsoUtility.h"
#include "HundgunBullet.h"

HundgunBullet::HundgunBullet(int baseModelId) :BulletBase(baseModelId)
{
}

HundgunBullet::~HundgunBullet(void)
{
}

void HundgunBullet::SetParam(void)
{
	bullet_.scale_ = AsoUtility::VECTOR_ONE;
	bullet_.rotate_ = AsoUtility::VECTOR_ZERO;

	bullet_.speed_ = SPEED;
	bullet_.cntAlive_ = CNT_ALIVE;
	bullet_.collisionRadius_ = COLLISION_RAD;
	bullet_.headDamage_ = HEAD_DAMAGE;
	bullet_.bodyDamage_ = BODY_DAMAGE;
}

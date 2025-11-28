#include "StraightMagic.h"

StraightMagic::StraightMagic(TYPE_MAGIC typeMagic,int baseModelId)
	:
	MagicBase(typeMagic, baseModelId)
{
}

StraightMagic::~StraightMagic(void)
{
}

void StraightMagic::SetParam(void)
{
	magic_.scale_ = SCALE;
	magic_.rotate_ = ROTATE;

	magic_.speed_ = SPEED;
	magic_.cntAlive_ = CNT_ALIVE;
	magic_.collisionRadius_ = COLLISION_RAD;
	magic_.headDamage_ = HEAD_DAMAGE;
	magic_.bodyDamage_ = BODY_DAMAGE;
}

void StraightMagic::ChangeShot(void)
{
	if (magic_.collisionRadius_ >= CHARGE_MAX)
	{
		// チャージが最大だったら、攻撃力を5上げる
		magic_.headDamage_ += ADD_DAMEGE;
		magic_.bodyDamage_ += ADD_DAMEGE;
	}
	else
	{
		// チャージが最大でなければ、通常時の攻撃力とする
		magic_.headDamage_ = HEAD_DAMAGE;
		magic_.bodyDamage_ = BODY_DAMAGE;
	}
}

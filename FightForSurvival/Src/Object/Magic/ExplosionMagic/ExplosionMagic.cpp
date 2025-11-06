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

void ExplosionMagic::ChangeBlast(void)
{
	// 着弾後当たり判定を大きくする
	magic_.collisionRadius_ += HIT_COLLISION_RAD;
}

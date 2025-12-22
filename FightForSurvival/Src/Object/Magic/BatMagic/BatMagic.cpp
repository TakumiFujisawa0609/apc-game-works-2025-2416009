#include "../../../Manager/EffectResManager/EffectResManager.h"
#include "BatMagic.h"

BatMagic::BatMagic(TYPE_MAGIC typeMagic, int baseModelId, VECTOR* pos) :MagicBase(typeMagic, baseModelId,pos)
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

	effectScale_ = 100.0f;
}

void BatMagic::ChangeCharge(void)
{
}

void BatMagic::ChangeShot(void)
{
	// チャージ状態のエフェクト再生
	effectPlayId_ = EffectResManager::GetInstance().PlayEffect(
		effectScale_, magic_.dir_, magic_.pos_, EffectResManager::TYPE::BAT_MAGIC);
}

void BatMagic::ChangeBlast(void)
{
}

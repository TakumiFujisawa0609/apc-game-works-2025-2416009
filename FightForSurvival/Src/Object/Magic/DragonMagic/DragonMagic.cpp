#include "../../../Manager/EffectResManager/EffectResManager.h"
#include "DragonMagic.h"

DragonMagic::DragonMagic(TYPE_MAGIC typeMagic, int baseModelId, VECTOR* pos) :MagicBase(typeMagic, baseModelId, pos)
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
	effectScale_ = 10.0f;
}

void DragonMagic::ChangeCharge(void)
{
	// チャージ状態のエフェクト再生
	effectPlayId_ = EffectResManager::GetInstance().PlayEffect(
		effectScale_, magic_.dir_, magic_.pos_, EffectResManager::TYPE::DRAGON_MAGIC);
}

void DragonMagic::ChangeShot(void)
{
	// チャージ状態のエフェクト再生
	effectPlayId_ = EffectResManager::GetInstance().PlayEffect(
		effectScale_, magic_.dir_, magic_.pos_, EffectResManager::TYPE::DRAGON_MAGIC);
}

void DragonMagic::ChangeBlast(void)
{
}

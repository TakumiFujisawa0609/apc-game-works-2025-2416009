#include "../../../Manager/EffectResManager/EffectResManager.h"
#include "ExplosionMagic.h"

ExplosionMagic::ExplosionMagic(TYPE_MAGIC typeMagic, int baseModelId, VECTOR* weponPos)
	:
	MagicBase(typeMagic, baseModelId, weponPos)
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

	effectScale_ = 10.0f;
}

void ExplosionMagic::ChangeCharge(void)
{
	// チャージ状態のエフェクト再生
	effectPlayId_ = EffectResManager::GetInstance().PlayEffect(
		effectScale_, magic_.dir_, magic_.pos_, EffectResManager::TYPE::PLAYER_MAGIC_CHARGE);
}

void ExplosionMagic::ChangeShot(void)
{


	if (magic_.collisionRadius_ >= chargeMax_)
	{
		// チャージが最大だったら、攻撃力を5上げる
		magic_.headDamage_ += addDamage_;
		magic_.bodyDamage_ += addDamage_;


		// ショット状態のエフェクト再生
		effectPlayId_ = EffectResManager::GetInstance().PlayEffect(
			effectScale_, magic_.dir_, magic_.pos_, EffectResManager::TYPE::PLAYER_MAGIC_SHOT_MAX);
	}
	else
	{
		// チャージが最大でなければ、通常時の攻撃力とする
		magic_.headDamage_ = HEAD_DAMAGE;
		magic_.bodyDamage_ = BODY_DAMAGE;

		// ショット状態のエフェクト再生
		effectPlayId_ = EffectResManager::GetInstance().PlayEffect(
			effectScale_, magic_.dir_, magic_.pos_, EffectResManager::TYPE::PLAYER_MAGIC_SHOT);
	}
}

void ExplosionMagic::ChangeBlast(void)
{
	// 着弾後当たり判定を大きくする
	magic_.collisionRadius_ += HIT_COLLISION_RAD;

	// 爆発状態のエフェクト再生
	effectPlayId_ = EffectResManager::GetInstance().PlayEffect(
		50.0f, magic_.dir_, magic_.pos_, EffectResManager::TYPE::PLAYER_MAGIC_BLAST);
}

#include "../../../Manager/EffectResManager/EffectResManager.h"
#include "StraightMagic.h"

StraightMagic::StraightMagic(TYPE_MAGIC typeMagic, VECTOR* weponPos)
	:
	MagicBase(typeMagic,weponPos)
{
}

StraightMagic::~StraightMagic(void)
{
}

void StraightMagic::SetParam(void)
{
	// 移動速度初期化
	magic_.speed_ = SPEED;
	// 存在可能時間の初期化
	magic_.cntAlive_ = CNT_ALIVE;
	// 当たり判定用半径の初期化
	magic_.collisionRadius_ = COLLISION_RAD;

	// ヘッドショット時のダメージ
	magic_.headDamage_ = HEAD_DAMAGE;

	// ヘッドショット以外の体のダメージ
	magic_.bodyDamage_ = BODY_DAMAGE;

	// エフェクトの大きさ初期化
	magic_.effectScale_ = EFFECT_SCALE;
}

void StraightMagic::ChangeCharge(void)
{
	// チャージ状態のエフェクト再生
	magic_.effectPlayId_ = EffectResManager::GetInstance().PlayEffect(
		magic_.effectScale_, magic_.dir_, magic_.pos_, EffectResManager::TYPE::PLAYER_MAGIC_CHARGE);
}

void StraightMagic::ChangeShot(void)
{

	if (magic_.collisionRadius_ >= chargeMax_)
	{
		// チャージが最大だったら、攻撃力を5上げる
		magic_.headDamage_ += addDamage_;
		magic_.bodyDamage_ += addDamage_;
		
		// ショット状態のエフェクト再生
		magic_.effectPlayId_ = EffectResManager::GetInstance().PlayEffect(
			magic_.effectScale_, magic_.dir_, magic_.pos_, EffectResManager::TYPE::PLAYER_MAGIC_SHOT_MAX);

	}
	else
	{
		// チャージが最大でなければ、通常時の攻撃力とする
		magic_.headDamage_ = HEAD_DAMAGE;
		magic_.bodyDamage_ = BODY_DAMAGE;

		// ショット状態のエフェクト再生
		magic_.effectPlayId_ = EffectResManager::GetInstance().PlayEffect(
			magic_.effectScale_, magic_.dir_, magic_.pos_, EffectResManager::TYPE::PLAYER_MAGIC_SHOT);

	}
}

void StraightMagic::ChangeBlast(void)
{
	// 爆発状態のエフェクト再生
	magic_.effectPlayId_ = EffectResManager::GetInstance().PlayEffect(
		magic_.effectScale_, magic_.dir_, magic_.pos_, EffectResManager::TYPE::PLAYER_MAGIC_BLAST);
}

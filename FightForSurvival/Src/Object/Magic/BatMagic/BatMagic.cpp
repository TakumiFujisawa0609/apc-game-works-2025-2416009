#include "../../../Manager/EffectResManager/EffectResManager.h"
#include "BatMagic.h"

BatMagic::BatMagic(TYPE_MAGIC typeMagic, VECTOR* pos) :MagicBase(typeMagic,pos)
{
}

BatMagic::~BatMagic(void)
{
}

void BatMagic::SetParam(void)
{
	// 移動速度初期化
	magic_.speed_ = SPEED;

	// 存在可能時間の初期化
	magic_.cntAlive_ = CNT_ALIVE;

	// 当たり判定用半径の初期化
	magic_.collisionRadius_ = COLLISION_RAD;

	// 魔法のダメージの初期化
	magic_.bodyDamage_ = DAMAGE;

	// エフェクトの大きさ初期化
	magic_.effectScale_ = EFFECT_SCALE;
}

void BatMagic::ChangeCharge(void)
{
}

void BatMagic::ChangeShot(void)
{
	// コウモリの魔法エフェクト再生
	magic_.effectPlayId_ = EffectResManager::GetInstance().PlayEffect(
		magic_.effectScale_, magic_.dir_, magic_.pos_, EffectResManager::TYPE::BAT_MAGIC);
}

void BatMagic::ChangeBlast(void)
{
}

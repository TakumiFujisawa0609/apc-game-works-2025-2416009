#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "../../Enemy/EnemyInfo.h"
#include "../../Enemy/Base/EnemyBase.h"
#include "../../Enemy/EnemyManager.h"
#include "../../../Utility/Collision/CollisionUtility.h"
#include "../../../Manager/EffectResManager/EffectResManager.h"
#include "ChaseMagic.h"

ChaseMagic::ChaseMagic(TYPE_MAGIC typeMagic, int baseModelId, VECTOR* weponPos)
	:
	MagicBase(typeMagic, baseModelId, weponPos)
{
}

ChaseMagic::~ChaseMagic(void)
{
}

void ChaseMagic::SetParam(void)
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

void ChaseMagic::UpdateShot(void)
{
	// ターゲットが分かっていたら向きを計算し更新
	if (targetFound_)
	{
		LookTargetEnemy();

		// エフェクトの回転
	// 方向から角度を出す
		VECTOR angle;
		angle.y = atan2(magic_.dir_.x, magic_.dir_.z);

		// XZのベクトルの長さを計算する
		float XZLength = sqrtf(magic_.dir_.x * magic_.dir_.x + magic_.dir_.z * magic_.dir_.z);

		// X軸の角度を計算する
		angle.x = atan2(magic_.dir_.y, XZLength);

		// 回転はXY軸のみとする
		angle.z = 0.0f;

		SetRotationPlayingEffekseer3DEffect(
			effectPlayId_, -angle.x, angle.y, angle.z);
	}

	// 移動処理
	MagicBase::UpdateShot();
}

void ChaseMagic::ChangeCharge(void)
{
	// チャージ状態のエフェクト再生
	effectPlayId_ = EffectResManager::GetInstance().PlayEffect(
		effectScale_, magic_.dir_, magic_.pos_, EffectResManager::TYPE::PLAYER_MAGIC_CHARGE);
}

void ChaseMagic::ChangeShot(void)
{

	if (magic_.collisionRadius_ >= chargeMax_)
	{
		// チャージが最大だったら、攻撃力を5上げる
		magic_.headDamage_ += addDamage_;
		magic_.bodyDamage_ += addDamage_;

		// ショット状態のエフェクト再生
		effectPlayId_ = EffectResManager::GetInstance().PlayEffect(
			effectScale_, magic_.dir_, magic_.pos_, EffectResManager::TYPE::PLAYER_MAGIC_CHASE);
	}
	else
	{
		// チャージが最大でなければ、通常時の攻撃力とする
		magic_.headDamage_ = HEAD_DAMAGE;
		magic_.bodyDamage_ = BODY_DAMAGE;
		// ショット状態のエフェクト再生
		effectPlayId_ = EffectResManager::GetInstance().PlayEffect(
			1000.0f, magic_.dir_, magic_.pos_, EffectResManager::TYPE::PLAYER_MAGIC_CHASE);
	}

	// 初期化
	targetPos_ = { 0.0f,0.0f,0.0f };

	// まだターゲットが見つかっていない（初期化前）
	targetFound_ = false;

	// 現在の最短距離を保持する変数 (最初は非常に大きな値を設定)
	float minDistance = FLT_MAX;

	auto& enemies = EnemyManager::GetInstance().GetEnemy();

	for (auto& enemy : enemies)
	{
		if (!enemy->IsCollisionState() || !enemy->GetEnemy().isAlive_)
		{
			// 生きていないなら処理を行わない
			continue;
		}

		VECTOR newEnemyPos = enemy->GetColPos().colPos_[COLLISION_POS::BODY_UNDER];
		newEnemyPos.y += 40.0f;

		if (!CollisionUtility::CollisionSecter(magic_.pos_, magic_.dir_, newEnemyPos, enemy->GetEnemy().collisionRadiusBody_, VIEW_RANGE, VIEW_ANGLE))
		{
			// 視野角範囲内に入っていなかったら追跡対象にしない
			continue;
		}

		// 相手へのベクトルを計算(引き算)
		// 魔法と新しいエネミーの距離を計算
		VECTOR vec = VSub(magic_.pos_, newEnemyPos);

		// ベクトルの正規化で単位ベクトル(方向)を取得する
		float length = sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);

		// 今現在の最短距離のエネミーの距離より小さければ新しいエネミーを最短距離者とする
		if (length < minDistance)
		{
			// 最短距離を更新する
			minDistance = length;

			// 最短距離の敵の座標を更新
			targetPos_ = newEnemyPos;

			// ターゲットが見つかった
			targetFound_ = true;
		}
	}
}

void ChaseMagic::ChangeBlast(void)
{
	// 爆発状態のエフェクト再生
	effectPlayId_ = EffectResManager::GetInstance().PlayEffect(
		effectScale_, magic_.dir_, magic_.pos_, EffectResManager::TYPE::PLAYER_MAGIC_BLAST);
}

void ChaseMagic::LookTargetEnemy(void)
{
	// 相手へのベクトルを計算(引き算)
	VECTOR vec;
	vec.x = targetPos_.x - magic_.pos_.x;
	vec.y = targetPos_.y - magic_.pos_.y;
	vec.z = targetPos_.z - magic_.pos_.z;

	// ベクトルの正規化で単位ベクトル(方向)を取得する
	float length = sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);

	if (length == 0.0f)
	{
		vec.x = vec.z = vec.y = 0.0f;
		return;
	}

	// 大きさで割って単位ベクトルにする
	magic_.dir_.x = vec.x / length;
	magic_.dir_.y = vec.y / length;
	magic_.dir_.z = vec.z / length;

	// 方向から角度を出す
	magic_.rotate_.y = atan2f(magic_.dir_.x, magic_.dir_.z);

	// 回転はY軸のみとする
	magic_.rotate_.x = magic_.rotate_.z = 0.0f;

	// モデルに向きを設定
	MV1SetRotationXYZ(magic_.modelId_, magic_.rotate_);
}

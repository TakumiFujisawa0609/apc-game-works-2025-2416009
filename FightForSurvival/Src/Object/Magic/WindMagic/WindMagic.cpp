#include <DxLib.h>
#include "../../Enemy/EnemyInfo.h"
#include "../../Enemy/Base/EnemyBase.h"
#include "../../Enemy/EnemyManager.h"
#include "WindMagic.h"

WindMagic::WindMagic(int baseModelId)
	:
	MagicBase(baseModelId)
{
}

WindMagic::~WindMagic(void)
{
}

void WindMagic::SetParam(void)
{
	magic_.scale_ = SCALE;
	magic_.rotate_ = ROTATE;

	magic_.speed_ = SPEED;
	magic_.cntAlive_ = CNT_ALIVE;
	magic_.collisionRadius_ = COLLISION_RAD;
	magic_.headDamage_ = HEAD_DAMAGE;
	magic_.bodyDamage_ = BODY_DAMAGE;
}

void WindMagic::UpdateShot(void)
{
	// ターゲットが分かっていたら向きを計算し更新
	if (targetFound_)
	{
		LookTargetEnemy();
	}

	// 移動処理
	MagicBase::UpdateShot();
}

void WindMagic::ChangeShot(void)
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

void WindMagic::LookTargetEnemy(void)
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
	magic_.rotate_.y = atan2(magic_.dir_.x, magic_.dir_.z);

	// 回転はY軸のみとする
	magic_.rotate_.x = magic_.rotate_.z = 0.0f;

	// モデルに向きを設定
	MV1SetRotationXYZ(magic_.modelId_, magic_.rotate_);
}

#include <DxLib.h>
#include "../../Common/AnimationController.h"
#include "../../../Application.h"
#include "Zombie.h"

Zombie::Zombie(ENEMY_TYPE type, int baseModelId, Player* player)
	: EnemyBase(type, baseModelId, player)
{
	state_.stateTable_[STATE_IDLE] = Idle;
	state_.stateTable_[STATE_ATTACK] = Attack;

	for (int i = 0; i < static_cast<int>(ANIM_TYPE::MAX); i++)
	{
		animationController_->AddInFbx(i, 25.0f, i);
	}

	// フレーム登録
	AddFrames();
}

Zombie::~Zombie(void)
{
}

void Zombie::SetParam(void)
{
	enemy_.angle_ = ANGLE;
	enemy_.scales_ = SCALE;
	enemy_.hp_ = HP;
	enemy_.moveSpeed_ = SPEED;
	enemy_.isAlive_ = true;

	score_ = SCORE;


	// 当たり判定用の半径
	enemy_.collisionRadius_ = COLLISION_RADIUS;
	// 頭
	enemy_.collisionRadiusHead_ = COLLISION_RADIUS_HEAD;
	// 体
	enemy_.collisionRadiusBody_ = COLLISION_RADIUS_BODY;
	// 腕
	enemy_.collisionRadiusArm_ = COLLISION_RADIUS_ARM;
	// 手
	enemy_.collisionRadiusHand_ = COLLISION_RADIUS_HAND;

	// 実際のボーンからの座標微調整用の相対座標
	// 頭
	collision_.offsetHead_ = OFFSET_POS_HEAD;
	// 体
	collision_.offsetBodyTop_ = OFFSET_POS_BODY_TOP;
	collision_.offsetBodyUnder_ = OFFSET_POS_BODY_UNDER;
	// 腕
	collision_.offsetArmTop_ = OFFSET_POS_ARM_TOP;
	collision_.offsetArmUnder_ = OFFSET_POS_ARM_UNDER;
	// 手
	collision_.offsetHand_ = OFFSET_POS_HAND;

	// 攻撃可能範囲
	attack_.cooldown_ = ATTACK_COOLDOWN;
	attack_.range_ = ATTACK_RANGE;

}

void Zombie::AddFrames(void)
{
	// 頭のボーンフレーム取得
	collision_.headBone_ = MV1SearchFrame(enemy_.modelId_, "Head");
	// 体のボーンフレーム取得
	collision_.bodyBoneTop_ = MV1SearchFrame(enemy_.modelId_, "Head");
	collision_.bodyBoneUnder_ = MV1SearchFrame(enemy_.modelId_, "Root");
	// 右腕のボーンフレーム取得
	collision_.armBoneTopR_ = MV1SearchFrame(enemy_.modelId_, "UpperArm.R");
	// 右手のボーンフレーム取得
	collision_.handBoneR_ = MV1SearchFrame(enemy_.modelId_, "Middle2.R");
	// 左腕のボーンフレーム取得
	collision_.armBoneTopL_ = MV1SearchFrame(enemy_.modelId_, "UpperArm.L");
	// 左手のボーンフレーム取得
	collision_.handBoneL_ = MV1SearchFrame(enemy_.modelId_, "Pinky1.L");
}

void Zombie::Idle(EnemyBase& enemy)
{
	if (!enemy.SearchAttackRange())
	{
		// 範囲内に入っていなかったら追跡
		enemy.ChangeState(ENEMY_STATE::STATE_CHASE);
	}
	else if(enemy.GetAttackCooldown() == 0.0f)
	{
		// 攻撃範囲に入っていて、攻撃待ち時間が0だったら攻撃へ移行
		enemy.ChangeState(ENEMY_STATE::STATE_ATTACK);
		// 攻撃当たり判定用目印
		enemy.SetIsAttack(true);
	}
}

void Zombie::Attack(EnemyBase& enemy)
{
	// ゲッター経由でアクセス
	AnimationController* animController = enemy.GetAnimationController();
	// ポインタが有効かチェックする
	if (animController != nullptr)
	{
		if (animController->IsEnd())
		{
			// 攻撃が終わったら後退させる
			enemy.ChangeState(ENEMY_STATE::STATE_RETREAT);
			// 攻撃待ち時間をセット
			enemy.SetAttackCooldown(ATTACK_COOLDOWN);
			// 攻撃当たり判定用目印
			enemy.SetIsAttack(false);
		}
	}
}

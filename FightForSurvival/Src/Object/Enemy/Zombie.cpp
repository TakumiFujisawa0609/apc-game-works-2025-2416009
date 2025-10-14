#include <DxLib.h>
#include "../Common/AnimationController.h"
#include "../../Application.h"
#include "Zombie.h"

Zombie::Zombie(ENEMY_TYPE type, int baseModelId, int baseAttackEffectModelId, Player* player)
	: EnemyBase(type, baseModelId, baseAttackEffectModelId, player)
{
	stateTable_[STATE_IDLE] = Idle;
	stateTable_[STATE_ATTACK] = Attack;

	// アニメーション登録
	AddAnimation();

	// フレーム登録
	AddFrames();
}

Zombie::~Zombie(void)
{
}

void Zombie::SetParam(void)
{
	enemy_.angles_ = ANGLE;
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
	// 脚
	enemy_.collisionRadiusLeg_ = COLLISION_RADIUS_LEG;

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
	// 脚
	collision_.offsetLegTop_ = OFFSET_POS_LEG_TOP;
	collision_.offsetLegUnder_ = OFFSET_POS_LEG_UNDER;

	// 攻撃可能範囲
	attackRange_ = ATTACK_RANGE;

}

void Zombie::AddAnimation(void)
{
	std::string enePas = Application::PATH_MODEL + "Enemy/";
	// 待機モーション
	std::string pas = enePas + "Zombie Idle.mv1";
	animationController_->Add(static_cast<int>(ENEMY_STATE::STATE_IDLE), 60.0f, pas);
	// 追跡モーション
	pas = enePas + "Zombie Running.mv1";
	animationController_->Add(static_cast<int>(ENEMY_STATE::STATE_CHASE), 60.0f, pas);
	// 攻撃モーション
	pas = enePas + "Standing Melee Attack Downward.mv1";
	animationController_->Add(static_cast<int>(ENEMY_STATE::STATE_ATTACK), 60.0f, pas);
	//// 後退モーション
	pas = enePas + "Standing Walk Back.mv1";
	animationController_->Add(static_cast<int>(ENEMY_STATE::STATE_RETREAT), 60.0f, pas);
	// ダメージ時モーション
	pas = enePas + "Zombie HitIdle.mv1";
	animationController_->Add(static_cast<int>(ENEMY_STATE::STATE_HIT), 150.0f, pas);
	// 死亡モーション
	pas = enePas + "Zombie Dying.mv1";
	animationController_->Add(static_cast<int>(ENEMY_STATE::STATE_DEAD), 75.0f, pas);
}

void Zombie::AddFrames(void)
{
	// 頭のボーンフレーム取得
	collision_.headBone_ = SearchFrame("5:Head");
	// 体のボーンフレーム取得
	collision_.bodyBoneTop_ = SearchFrame("5:Spine2");
	collision_.bodyBoneUnder_ = SearchFrame("5:Hips");
	// 右腕のボーンフレーム取得
	collision_.armBoneTopR_ = SearchFrame("5:RightArm");
	// 右手のボーンフレーム取得
	collision_.handBoneR_ = SearchFrame("5:RightHand");
	// 左腕のボーンフレーム取得
	collision_.armBoneTopL_ = SearchFrame("5:LeftArm");
	// 左手のボーンフレーム取得
	collision_.handBoneL_ = SearchFrame("5:LeftHand");
	// 右脚のボーンフレーム取得
	collision_.legBoneTopR_ = SearchFrame("5:RightUpLeg");
	collision_.legBoneUnderR_ = SearchFrame("5:RightFoot");
	// 左脚のボーンフレーム取得
	collision_.legBoneTopL_ = SearchFrame("5:LeftUpLeg");
	collision_.legBoneUnderL_ = SearchFrame("5:LeftFoot");
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
		}
	}
}

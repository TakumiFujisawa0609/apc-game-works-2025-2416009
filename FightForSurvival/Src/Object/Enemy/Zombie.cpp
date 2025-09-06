#include <DxLib.h>
#include "../Common/AnimationController.h"
#include "../../Application.h"
#include "Zombie.h"

Zombie::Zombie(void)
{
	stateTable_[STATE_IDLE] = Idle;
	stateTable_[STATE_ATTACK] = Attack;
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

	// 当たり判定用の半径
	// 頭
	enemy_.collisionRadius_ = COLLISION_RADIUS;
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

	// 頭のボーンフレーム取得
	collision_.headBone_ = SearchFrame("Head");
	// 体のボーンフレーム取得
	collision_.bodyBoneTop_ = SearchFrame("Spine2");
	collision_.bodyBoneUnder_ = SearchFrame("Hips");
	// 右腕のボーンフレーム取得
	collision_.armBoneTopR_ = SearchFrame("RightArm");
	// 右手のボーンフレーム取得
	collision_.handBoneR_ = SearchFrame("RightHand");
	// 左腕のボーンフレーム取得
	collision_.armBoneTopL_ = SearchFrame("LeftArm");
	// 左手のボーンフレーム取得
	collision_.handBoneL_ = SearchFrame("LeftHand");
	// 右脚のボーンフレーム取得
	collision_.legBoneTopR_ = SearchFrame("RightUpLeg");
	collision_.legBoneUnderR_ = SearchFrame("RightFoot");
	// 左脚のボーンフレーム取得
	collision_.legBoneTopL_ = SearchFrame("LeftUpLeg");
	collision_.legBoneUnderL_ = SearchFrame("LeftFoot");
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
}

void Zombie::Idle(EnemyBase& enemy)
{
	//if (範囲内に入ってなかったら移動)
	//{
	//	// 範囲内に入っていなかったら追跡
		enemy.ChangeState(ENEMY_STATE::STATE_CHASE);
	//}
	//else
	//{
		
	//	if (攻撃制限時間を超えたら入る)
	//	{
	//		// 攻撃制限時間を超えているかつ、範囲内に入っていたら攻撃を行う
	//		ChangeState(ENEMY_STATE::PUNCH);
	//	}
	//}
}

void Zombie::Attack(EnemyBase& enemy)
{
	//if (攻撃終わったらIDLEに戻す)
	//{
	//	// 攻撃が終わったら後退させる
	//	enemy.ChangeState(ENEMY_STATE::STATE_RETREAT);
	//}
}

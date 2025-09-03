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
	enemy_.collisionRadius_ = COLLISION_RADIUS;
	enemy_.collisionRadiusBody_ = COLLISION_RADIUS_BODY;
	colPos_.relativePosHead_ = RELATIVE_POS_HEAD;
	colPos_.relativePosBodyTop_ = RELATIVE_POS_BODY_TOP;
	colPos_.relativePosBodyUnder_ = RELATIVE_POS_BODY_UNDER;
}

void Zombie::AddAnimation(void)
{
	// ダンステスト
//std::string pas = "Data/Model/Enemy/Idle.mv1";
//animationController_->Add(static_cast<int>(ANIM_TYPE::IDLE), 60.0f, pas);
}

void Zombie::Idle(EnemyBase& enemy)
{
	//if (範囲内に入ってなかったら移動)
	//{
	//	// 範囲内に入っていなかったら追跡
		//enemy.ChangeState(ENEMY_STATE::STATE_CHASE);
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

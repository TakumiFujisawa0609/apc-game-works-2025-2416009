#include "../../Common/AnimationController.h"
#include "Bat.h"

Bat::Bat(ENEMY_TYPE type, int baseModelId,Player* player)
	:EnemyBase(type, baseModelId, player)
{
	// ステートテーブルの初期化
	state_.stateTable_[STATE_IDLE] = Idle;
	state_.stateTable_[STATE_ATTACK] = Attack;

	// アニメーションの読み込み
	for (int i = 0; i < static_cast<int>(ANIM_TYPE_FLY::MAX); i++)
	{
		animationController_->AddInFbx(i, 25.0f, i);
	}

	// フレーム登録
	AddFrames();
}

Bat::~Bat(void)
{
}

void Bat::SetParam(void)
{
	// 向き初期化
	enemy_.angle_ = ANGLE;

	// モデルの大きさ初期化
	enemy_.scales_ = SCALE;

	// HPの初期化
	enemy_.hp_ = HP;

	// 移動速度の初期化
	enemy_.moveSpeed_ = SPEED;

	// 生存判定の初期化
	enemy_.isAlive_ = true;

	// 敵撃破時のスコア
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

	// 攻撃クールダウンの初期化
	attack_.cooldown_ = ATTACK_COOLDOWN;

	// 攻撃可能範囲の初期化
	attack_.range_ = ATTACK_RANGE;

	// 魔法攻撃の座標初期化
	relativeMagicPos_ = RELATIVE_MAGIC_POS;
}

void Bat::AddFrames(void)
{
	// 頭のボーンフレーム取得
	collision_.headBone_ = MV1SearchFrame(enemy_.modelId_, "Neck");
	// 体のボーンフレーム取得
	collision_.bodyBoneTop_ = MV1SearchFrame(enemy_.modelId_,"Neck");
	collision_.bodyBoneUnder_ = MV1SearchFrame(enemy_.modelId_, "BodyRoot");
	// 右腕のボーンフレーム取得
	collision_.armBoneTopR_ = MV1SearchFrame(enemy_.modelId_, "Wing1.R");
	// 右手のボーンフレーム取得
	collision_.handBoneR_ = MV1SearchFrame(enemy_.modelId_, "Wing4.R_end");
	// 左腕のボーンフレーム取得
	collision_.armBoneTopL_ = MV1SearchFrame(enemy_.modelId_, "Wing1.L");
	// 左手のボーンフレーム取得
	collision_.handBoneL_ = MV1SearchFrame(enemy_.modelId_, "Wing4.L_end");
}

void Bat::PlayAnim(void)
{
	if (animationController_ == nullptr)
	{
		// アニメーションコントローラーの中身が空だったら処理を行わない
		return;
	}

	// ステートによってアニメーションを変更
	switch (state_.state_)
	{
	case ENEMY_STATE::STATE_IDLE:
		animationController_->Play(static_cast<int>(ANIM_TYPE_FLY::FLYING));
		break;
	case ENEMY_STATE::STATE_CHASE:
		animationController_->BlendAnimPlay(static_cast<int>(ANIM_TYPE_FLY::FLYING), AnimationController::BLEND_LATIO);
		break;
	case ENEMY_STATE::STATE_ATTACK:
		animationController_->BlendAnimPlay(static_cast<int>(ANIM_TYPE_FLY::ATTACK_2), AnimationController::BLEND_LATIO, false);
		break;
	case ENEMY_STATE::STATE_RETREAT:
		animationController_->BlendAnimPlay(static_cast<int>(ANIM_TYPE_FLY::FLYING), AnimationController::BLEND_LATIO, false);
		break;
	case ENEMY_STATE::STATE_HIT:
		animationController_->BlendAnimPlay(static_cast<int>(ANIM_TYPE_FLY::HIT), AnimationController::BLEND_LATIO, false);
		break;
	case ENEMY_STATE::STATE_DEAD:
		animationController_->BlendAnimPlay(static_cast<int>(ANIM_TYPE_FLY::DEATH), AnimationController::BLEND_LATIO, false);
		break;
	default:
		break;
	}
}

void Bat::Idle(EnemyBase& enemy)
{
	// 攻撃範囲に入っていたら
	if (enemy.SearchAttackRange())
	{
		// プレイヤーのほうへ向く
		enemy.LookPlayer();

		// 攻撃クール時間が終わっていたら
		if (enemy.GetAttackCooldown() <= 0.0f)
		{
			// 攻撃を生成
			enemy.CreateMagicForward();

			// 攻撃範囲に入っていて、攻撃待ち時間が0だったら攻撃へ移行
			enemy.ChangeState(ENEMY_STATE::STATE_ATTACK);
		}
	}
}

void Bat::Attack(EnemyBase& enemy)
{
	// ゲッター経由でアクセス
	AnimationController* animController = enemy.GetAnimationController();

	// ポインタが有効かチェックする
	if (animController != nullptr)
	{
		// アニメーションが終わっていたら
		if (animController->IsEnd())
		{
			// IDLE状態へ戻す
			enemy.ChangeState(ENEMY_STATE::STATE_IDLE);

			// 攻撃待ち時間をセット
			enemy.SetAttackCooldown(ATTACK_COOLDOWN);
		}
	}
}
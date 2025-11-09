#include "../../Common/AnimationController.h"
#include "../../Magic/Base/MagicBase.h"
#include "../../../Utility/MatrixUtility.h"
#include "../../../Utility/Collision/CollisionUtility.h"
#include "../../../Scene/SceneManager.h"
#include "../../Enemy/EnemyManager.h"
#include "Dragon.h"

Dragon::Dragon(ENEMY_TYPE type, int baseModelId, std::vector<int> animModelIds, Player* player)
	: EnemyBase(type, baseModelId, animModelIds, player)
{
	// 関数ポインタの設定
	state_.stateTable_[STATE_IDLE] = Idle;
	state_.stateTable_[STATE_ATTACK] = Attack;

	attackStateTable_[SELECT] = AttackSelect;
	attackStateTable_[RANGE_ATTACK] = RangeAttack;
	attackStateTable_[FORWARD_ATTACK] = ForwardAttack;
	attackStateTable_[RUSH_ATTACK] = RushAttack;
	attackStateTable_[ATTACK_END] = AttackEnd;

	for (int i = 0; i < static_cast<int>(ANIM_TYPE_FLY::MAX); i++)
	{
		animationController_->AddInFbx(i, 25.0f, i);
	}

	// フレーム登録
	AddFrames();
}

Dragon::~Dragon(void)
{
}

void Dragon::Release(void)
{
	magicsRange_.clear();
	EnemyBase::Release();
}

void Dragon::ChangeAttackState(DRAGON_ATTACK_STATE state)
{
	attackState_ = state;

	// ステートに合った初期化
	switch (attackState_)
	{
	case Dragon::SELECT:
		break;
	case Dragon::RANGE_ATTACK:
		animationController_->BlendAnimPlay(static_cast<int>(ANIM_TYPE_FLY::ATTACK), AnimationController::BLEND_LATIO, false);
		break;
	case Dragon::FORWARD_ATTACK:
		forwardAttackStartTime_ = STARTING_TIME;
		break;
	case Dragon::RUSH_ATTACK:
		animationController_->BlendAnimPlay(static_cast<int>(ANIM_TYPE_FLY::FLYING), AnimationController::BLEND_LATIO, false);
		break;
	case Dragon::ATTACK_END:
		animationController_->BlendAnimPlay(static_cast<int>(ANIM_TYPE_FLY::FLYING), AnimationController::BLEND_LATIO);
		break;
	default:
		break;
	}
}

void Dragon::SetParam(void)
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
	attack_.range_ = ATTACK_RANGE;

	enemy_.dir_.x = sinf(enemy_.angle_.y);
	enemy_.dir_.y = 0.0f; // 飛行タイプの場合、通常は0
	enemy_.dir_.z = cosf(enemy_.angle_.y);
	enemy_.dir_ = VNorm(enemy_.dir_);

	// 最初のステートを攻撃選択モードにする
	ChangeAttackState(SELECT);

	// 突撃時の速度
	rushSpeed_ = RUSH_SPEED;

	relativeMagicPos_ = RELATIVE_MAGIC_POS;
}

void Dragon::AddFrames(void)
{
	// 頭のボーンフレーム取得
	collision_.headBone_ = MV1SearchFrame(enemy_.modelId_, "Neck");
	// 体のボーンフレーム取得
	collision_.bodyBoneTop_ = MV1SearchFrame(enemy_.modelId_, "Neck");
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

void Dragon::PlayAnim(void)
{
	if (animationController_ == nullptr)
	{
		// アニメーションコントローラーの中身が空だったら処理を行わない
		return;
	}

	switch (state_.state_)
	{
	case ENEMY_STATE::STATE_IDLE:
		animationController_->BlendAnimPlay(static_cast<int>(ANIM_TYPE_FLY::FLYING), AnimationController::BLEND_LATIO);
		break;
	case ENEMY_STATE::STATE_CHASE:
		animationController_->BlendAnimPlay(static_cast<int>(ANIM_TYPE_FLY::FLYING), AnimationController::BLEND_LATIO);
		break;
	case ENEMY_STATE::STATE_ATTACK:
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

void Dragon::Idle(EnemyBase& enemy)
{
	// 攻撃のクールタイムが終了したら入る
	if (enemy.GetAttackCooldown() <= 0.0f)
	{
		// プレイヤー側を向く
		enemy.LookPlayer();
		// 攻撃状態に変更
		enemy.ChangeState(ENEMY_STATE::STATE_ATTACK);

		// Dragon自身のメンバ変数を使用するためにダウンキャストを行い、参照ポインタを作成
		Dragon& self = static_cast<Dragon&>(enemy);
		// 攻撃の種類を取得
		self.ChangeAttackState(SELECT);
	}
}

void Dragon::Attack(EnemyBase& enemy)
{
	// Dragon自身のメンバ変数を使用するためにダウンキャストを行い、参照ポインタを作成
	Dragon& self = static_cast<Dragon&>(enemy);

	// nullチェック
	if (self.attackStateTable_[self.attackState_])
	{
		self.attackStateTable_[self.attackState_](self);
	}
}

void Dragon::AttackSelect(Dragon& dragon)
{
	dragon.ChangeAttackState(RANGE_ATTACK);
	//// ランダムで決めた攻撃内容を入れる
	//int attackType = GetRand(RANDOM_NUM);

	//if (attackType >= 0 && attackType <= RANGE)
	//{
	//	// 範囲攻撃
	//	dragon.ChangeAttackState(RANGE_ATTACK);
	//}
	//else if (attackType > RANGE && attackType <= FORWARD)
	//{
	//	// 前方攻撃
	//	dragon.ChangeAttackState(FORWARD_ATTACK);
	//}
	//else if (attackType > FORWARD && attackType <= RUSH)
	//{
	//	// 突進攻撃
	//	dragon.ChangeAttackState(RUSH_ATTACK);
	//}
}

void Dragon::RangeAttack(Dragon& dragon)
{
	// EnemyBaseのメンバ変数を使用するためにアップキャストを行い、参照ポインタを作成
	EnemyBase& base = static_cast<EnemyBase&>(dragon);

	// ゲッター経由でアクセス
	AnimationController* animController = base.GetAnimationController();

	// ポインタが有効かチェックする
	if (animController != nullptr)
	{
		if (animController->IsEnd() && animController->GetPlayType() == static_cast<int>(ANIM_TYPE_FLY::ATTACK))
		{
			// アニメーションを再生
			animController->BlendAnimPlay(static_cast<int>(ANIM_TYPE_FLY::ATTACK_2), AnimationController::BLEND_LATIO, false);
			dragon.IsDrawMagicWhole();
		}
		else if (animController->GetPlayType() == static_cast<int>(ANIM_TYPE_FLY::ATTACK_2) && dragon.magicsRange_.size() != 0)
		{
			// ドラゴンの周りに魔法を発動(生成)
			dragon.CreateMagicWhole();
		}
		else if (animController->IsEnd())
		{
			// 攻撃発動したら戻す
			dragon.ChangeAttackState(ATTACK_END);
		}
		
	}
}

void Dragon::ForwardAttack(Dragon& dragon)
{
	if (dragon.forwardAttackStartTime_ > 0.0f)
	{
		// 前方攻撃待ち時間を0に近づける
		dragon.SubStartTime();
	}
	else
	{
		// EnemyBaseのメンバ変数を使用するためにアップキャストを行い、参照ポインタを作成
		EnemyBase& base = static_cast<EnemyBase&>(dragon);
		// ゲッター経由でアクセス
		AnimationController* animController = base.GetAnimationController();
		// ポインタが有効かチェックする
		if (animController != nullptr)
		{
			if (animController->IsEnd())
			{
				dragon.ChangeAttackState(ATTACK_END);
			}
		}
	}
}

void Dragon::RushAttack(Dragon& dragon)
{
	dragon.ChangeAttackState(ATTACK_END);
}

void Dragon::AttackEnd(Dragon& dragon)
{
	// 既定の位置にいなかったら(少しでもずれていたら)帰る処理を行う
	dragon.ReturnPositon();
}

void Dragon::IsDrawMagicWhole(void)
{
	for (int i = 0; i < MAGIC_NUM; i++)
	{
		// 有効な魔法を取得する
		auto* magic = EnemyManager::GetInstance().GetValidMagic(TYPE_MAGIC::DRAGON_MAGIC);
		// 初期化処理
		magic->Init();

		// モデルの回転行列取得
		VECTOR rot = { enemy_.angle_.x, i * 30.0f * (DX_PI_F / 180.0f) ,enemy_.angle_.z };

		MATRIX matRot = MatrixUtility::Multiplication(enemy_.angle_, rot);

		// 方向と同じ要領で、相対座標を回転
		VECTOR localPosRot = VTransform(relativeMagicPos_, matRot);

		VECTOR pos = VAdd(enemy_.pos_, localPosRot);

		magic->UpdatePos(pos);

		magicsRange_.emplace_back(magic);
	}
}

void Dragon::CreateMagicWhole(void)
{
	int i = 0;

	for (MagicBase* magic : magicsRange_)
	{
		// モデルの回転行列取得
		VECTOR rot = { enemy_.angle_.x, i * 30.0f * (DX_PI_F / 180.0f) ,enemy_.angle_.z };

		MATRIX matRot = MatrixUtility::Multiplication(enemy_.angle_,rot);

		// 方向と同じ要領で、相対座標を回転
		VECTOR localPosRot = VTransform(relativeMagicPos_, matRot);

		VECTOR pos = VAdd(enemy_.pos_, localPosRot);

		// 進行方向のベクトルを matRot で回転させる
		VECTOR dir = VTransform(enemy_.dir_, matRot);

		// 座標を更新する
		magic->CreateShot(pos, dir);

		i++;
	}

	// 魔法をSHOT状態にし終わったため、中身を消す
	magicsRange_.clear();
}

void Dragon::ReturnPositon(void)
{
	// 既定の位置の範囲にいなかったら(少しでもずれていたら)帰る処理を行う
	if (!CollisionUtility::IsCollidingSphereAndPos(enemy_.prevPos_,ATTACK_RANGE, enemy_.pos_))
	{

		// 相手へのベクトルを計算(引き算)
		VECTOR vec;
		vec.x = enemy_.prevPos_.x - enemy_.pos_.x;
		vec.y = enemy_.prevPos_.y - enemy_.pos_.y;
		vec.z = enemy_.prevPos_.z - enemy_.pos_.z;

		// ベクトルの正規化で単位ベクトル(方向)を取得する
		float length = sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);

		if (length == 0.0f)
		{
			vec.x = vec.z = vec.y = 0.0f;
			return;
		}

		// 大きさで割って単位ベクトルにする
		enemy_.dir_.x = vec.x / length;
		enemy_.dir_.y = vec.y / length;
		enemy_.dir_.z = vec.z / length;

		// 方向から角度を出す
		enemy_.angle_.y = atan2f(enemy_.dir_.x, enemy_.dir_.z);

		// 回転はY軸のみとする
		enemy_.angle_.x = enemy_.angle_.z = 0.0f;

		// モデルに向きを設定
		MV1SetRotationXYZ(enemy_.modelId_, enemy_.angle_);

		// 方向単位ベクトルに速度をかけた数を座標に足しこむ
		enemy_.pos_ = VAdd(enemy_.pos_, VScale(enemy_.dir_, enemy_.moveSpeed_));

		// 計算した座標をモデルに適用する
		MV1SetPosition(enemy_.modelId_, enemy_.pos_);
	}

	// 既定の位置まで戻ったら
	if (CollisionUtility::IsCollidingSphereAndPos(enemy_.prevPos_, COLLISION_RADIUS, enemy_.pos_))
	{
		// プレイヤー側を向く
		LookPlayer();
		// 元の位置に戻れたらIDLEへ戻す
		ChangeState(ENEMY_STATE::STATE_IDLE);
		// 攻撃待ち時間をセット
		SetAttackCooldown(ATTACK_COOLDOWN);
	}
}

void Dragon::SubStartTime(void)
{
	// 攻撃待ち時間を減らす
	forwardAttackStartTime_ -= SceneManager::GetInstance().GetDeltaTime();
	if (forwardAttackStartTime_ < 0.0f)
	{
		// 0に初期化
		forwardAttackStartTime_ = 0.0f;
		// 攻撃アニメーションを再生
		animationController_->BlendAnimPlay(static_cast<int>(ANIM_TYPE_FLY::ATTACK_2), AnimationController::BLEND_LATIO, false);
	}
	else if(forwardAttackStartTime_ > 1.0f)
	{
		// 攻撃可能時間の１秒前まで、プレイヤー側を向く処理を行う
		LookPlayer();
	}
}

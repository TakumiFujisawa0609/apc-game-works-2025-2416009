#include "../../Common/AnimationController.h"
#include "../../Magic/Base/MagicBase.h"
#include "../../../Utility/AsoUtility.h"
#include "../../../Utility/MatrixUtility.h"
#include "../../../Utility/Collision/CollisionUtility.h"
#include "../../../Scene/SceneManager.h"
#include "../../Enemy/EnemyManager.h"
#include "../../Player/Player.h"
#include "Dragon.h"

Dragon::Dragon(ENEMY_TYPE type, int baseModelId, Player* player)
	: EnemyBase(type, baseModelId, player)
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
	attack_.cooldown_ = ATTACK_COOLDOWN;
	attack_.range_ = ATTACK_RANGE;

	enemy_.dir_.x = sinf(enemy_.angle_.y);
	enemy_.dir_.y = 0.0f; // 飛行タイプの場合、通常は0
	enemy_.dir_.z = cosf(enemy_.angle_.y);
	enemy_.dir_ = VNorm(enemy_.dir_);

	// 最初のステートを攻撃選択モードにする
	ChangeAttackState(SELECT);

	// 魔法の相対座標
	relativeMagicPos_ = RELATIVE_MAGIC_POS;

	// 突撃時の速度
	rushSpeed_ = RUSH_SPEED;

	// 攻撃確定タイミング用
	animationNum_ = 0;
}

void Dragon::Draw(void)
{
	EnemyBase::Draw();

	if (attackState_ == DRAGON_ATTACK_STATE::FORWARD_ATTACK && enemy_.isAlive_)
	{
		// 前方攻撃だった場合扇状に危険区域を描画
		VECTOR pos_0, pos_1, pos_2, pos_3;

		MATRIX mat = MatrixUtility::GetMatrixRotateXYZ(enemy_.angle_);

		// 前方方向
		VECTOR forward = VTransform(AsoUtility::DIR_B, mat);

		// 右側方向
		MATRIX rightMat = MMult(mat, MGetRotY(AsoUtility::Deg2RadF(VIEW_ANGLE)));
		VECTOR right = VTransform(AsoUtility::DIR_B, rightMat);

		// 左側方向
		MATRIX leftMat = MMult(mat, MGetRotY(AsoUtility::Deg2RadF(-VIEW_ANGLE)));
		VECTOR left = VTransform(AsoUtility::DIR_B, leftMat);

		// エネミー座標
		pos_0 = enemy_.pos_;

		// 正面の位置
		pos_1 = VAdd(pos_0, VScale(forward, VIEW_RANGE));

		// 正面から反時計回り
		pos_2 = VAdd(pos_0, VScale(left, VIEW_RANGE));

		// 正面から時計回り
		pos_3 = VAdd(pos_0, VScale(right, VIEW_RANGE));

		// 視野の描画
		pos_0.y = pos_1.y = pos_2.y = pos_3.y = -50.0f;

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 125);

		DrawTriangle3D(pos_0, pos_2, pos_1, 0xff0000, true);
		DrawTriangle3D(pos_0, pos_1, pos_3, 0xff0000, true);

		DrawLine3D(pos_0, pos_2, 0xff0000);
		DrawLine3D(pos_0, pos_3, 0xff0000);

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}

void Dragon::Release(void)
{
	magicsRange_.clear();
	EnemyBase::Release();
}

bool Dragon::SearchAttackRange(void)
{
	// プレイヤーの座標
	VECTOR plaPos = player_->GetPlayerStatus().pos_;
	float plaRad = player_->GetPlayerStatus().collisionRadius_;

	// 視野の範囲内に入っているかつ、攻撃時間になったらtrueを返す
	return CollisionUtility::CollisionSecter(enemy_.pos_, enemy_.dir_, plaPos, plaRad, VIEW_RANGE, VIEW_ANGLE) && attackStart_;
}

void Dragon::ChangeAttackState(DRAGON_ATTACK_STATE state)
{
	attackState_ = state;

	// 初期化設定
	ChangeAttackStateInit();
	// アニメーション設定
	PlayAttackAnim();
}

void Dragon::CollisionStage(VECTOR pos)
{
	// 衝突したら指定座標に押し戻す
	enemy_.pos_ = VAdd(enemy_.pos_, pos);
	MV1SetPosition(enemy_.modelId_, enemy_.pos_);
}

void Dragon::CollisionStage(float posY)
{
	// 衝突したら指定座標に押し戻す
	enemy_.pos_.y = posY;

	if (rushStep_ == RUSH_STEP::END_POSITION)
	{
		// ラッシュ時には速度が早すぎて押し出し量が少ないため、無理やり上に押し出す
		enemy_.pos_.y += 30.0f;
	}

	MV1SetPosition(enemy_.modelId_, enemy_.pos_);

	enemy_.gravity_ = 0.0f;
	enemy_.isJump_ = false;
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
		PlayAttackAnim();
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
	// Dragon自身のメンバ変数を使用するためにダウンキャストを行う
	Dragon& self = dynamic_cast<Dragon&>(enemy);

	// nullチェック
	if (self.attackStateTable_[self.attackState_])
	{
		self.attackStateTable_[self.attackState_](self);
	}
}

void Dragon::AttackSelect(Dragon& dragon)
{
	// プレイヤー側を向く
	dragon.LookPlayer();

	// ランダムで決めた攻撃内容を入れる
	int attackRand = GetRand(RANDOM_NUM);

	if (attackRand >= 0 && attackRand <= RANGE)
	{
		// 範囲攻撃
		dragon.ChangeAttackState(RANGE_ATTACK);
	}
	else if (attackRand > RANGE && attackRand <= FORWARD)
	{
		// 前方攻撃
		dragon.ChangeAttackState(FORWARD_ATTACK);
	}
	else if (attackRand > FORWARD && attackRand <= RUSH)
	{
		// 突進攻撃
		dragon.ChangeAttackState(RUSH_ATTACK);
	}
}

void Dragon::RangeAttack(Dragon& dragon)
{
	// ポインタが有効かチェックする
	if (dragon.animationController_ != nullptr)
	{
		if (dragon.animationController_->IsEnd() && dragon.animationController_->GetPlayType() == static_cast<int>(ANIM_TYPE_FLY::ATTACK) && !dragon.attackStart_)
		{
			// アニメーションを再生
			dragon.animationController_->BlendAnimPlay(static_cast<int>(ANIM_TYPE_FLY::ATTACK_2), AnimationController::BLEND_LATIO, false);
			dragon.IsDrawMagicWhole();
		}
		// 中身が入っていたら
		else if (dragon.animationController_->GetPlayType() == static_cast<int>(ANIM_TYPE_FLY::ATTACK_2) && !dragon.attackStart_ && dragon.magicsRange_.size() > 0)
		{
			// ドラゴンの周りに魔法を発動(生成)
			dragon.CreateMagicWhole();
			// アタックしたことを知らせる
			dragon.attackStart_ = true;
		}
		else if ((dragon.animationController_->GetPlayType() == static_cast<int>(ANIM_TYPE_FLY::FLYING) 
			|| dragon.animationController_->IsEnd()) 
			&& dragon.attackStart_)
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
		// ポインタが有効かチェックする
		if (dragon.animationController_ != nullptr)
		{
			// 攻撃確定させたいタイミングになったらtrueにする
			if (dragon.animationNum_ == FORWARD_CONFIRM_FRAME)
			{
				dragon.attackStart_ = true;
			}

			// 攻撃が終了したかつ、攻撃中フラグが立っていた場合、攻撃中フラグを折る
			if (dragon.animationController_->IsEnd() 
				&& dragon.animationController_->GetPlayType() == static_cast<int>(ANIM_TYPE_FLY::ATTACK_2)
				&& dragon.IsAttack())
			{
				dragon.SetIsAttack(false);
			}

			// アニメーションが終わっている
			if ((dragon.animationController_->IsEnd() 
				|| dragon.animationController_->GetPlayType() == static_cast<int>(ANIM_TYPE_FLY::FLYING)) 
				&& dragon.attackStart_ 
				&& !dragon.IsAttack())
			{
				dragon.attackStart_ = false;
				dragon.ChangeAttackState(ATTACK_END);
			}
			else if(dragon.animationNum_ < FORWARD_CONFIRM_FRAME)
			{
				// アニメーションがどこまで進んでいるのかの目安
				dragon.animationNum_++;
			}
		}
	}
}

void Dragon::RushAttack(Dragon& dragon)
{
	switch (dragon.rushStep_)
	{
	case Dragon::RUSH_STEP::STATING_POSITION:

		// 突進開始する位置まで移動
		dragon.RushStartPosition();

		break;
	case Dragon::RUSH_STEP::END_POSITION:

		// 突進終了する位置まで移動
		dragon.RushEndPosition();

		break;
	case Dragon::RUSH_STEP::RETURN_POSITON:

		// 元の位置まで移動
		dragon.ReturnPositon();

		break;
	default:
		break;
	}

}

void Dragon::AttackEnd(Dragon& dragon)
{
	// プレイヤー側を向く
	dragon.LookPlayer();
	// 元の位置に戻れたらIDLEへ戻す
	dragon.ChangeState(ENEMY_STATE::STATE_IDLE);
	// 攻撃待ち時間をセット
	dragon.SetAttackCooldown(ATTACK_COOLDOWN);
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

void Dragon::RushStartPosition(void)
{

	// 突撃攻撃開始位置へ近づける
	MoveToDestination(rushStartPos_,enemy_.moveSpeed_);

	// 攻撃開始範囲に入っていたら
	if (CollisionUtility::IsCollidingSphereAndPos(rushStartPos_, RUSH_POINT_RANGE,GetEnemy().pos_))
	{
		// ステートを変更する
		rushStep_ = RUSH_STEP::END_POSITION;
		// 当たり判定をONにする
		SetIsAttack(true);

		// LookPlayer()でエネミーの向きと方向ベクトル(enemy_.dir_)を確定させる
		LookPlayer();

		// 突進開始時のプレイヤーの位置を基準点とし、VIEW_RANGE分だけ進んだ先を目的地とする。
		rushEndPos_ = VAdd(player_->GetPlayerStatus().pos_, VScale(enemy_.dir_, RUSH_RANGE));
		// Y軸移動は行わない
		rushEndPos_.y = 0.0f;
	}
}

void Dragon::RushEndPosition(void)
{
	// 突撃攻撃開始位置へ近づける
	MoveToDestination(rushEndPos_, rushSpeed_);

	// 攻撃終了範囲に入っていたら
	if (CollisionUtility::IsCollidingSphereAndPos(rushEndPos_, RUSH_POINT_RANGE, GetEnemy().pos_))
	{
		// ステートを帰宅に変更する
		rushStep_ = RUSH_STEP::RETURN_POSITON;
		SetIsAttack(false);
	}
}

void Dragon::ReturnPositon(void)
{
	// 目的地(最初にスポーンした場所)まで進む
	MoveToDestination(enemy_.prevPos_, enemy_.moveSpeed_);

	// 既定の位置まで戻ったら
	if (CollisionUtility::IsCollidingSphereAndPos(enemy_.prevPos_, COLLISION_RADIUS, enemy_.pos_))
	{
		// 攻撃を終了させる
		ChangeAttackState(ATTACK_END);
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

void Dragon::MoveToDestination(VECTOR destination, float moveSpeed)
{
	// 相手へのベクトルを計算(引き算)
	// Y軸移動は考慮しない
	VECTOR vec;
	vec.x = destination.x - enemy_.pos_.x;
	vec.y = 0.0f;
	vec.z = destination.z - enemy_.pos_.z;

	// ベクトルの正規化で単位ベクトル(方向)を取得する
	float length = sqrtf(vec.x * vec.x + vec.z * vec.z);

	if (length == 0.0f)
	{
		vec.x = vec.z = 0.0f;
		return;
	}

	// 大きさで割って単位ベクトルにする
	enemy_.dir_.x = vec.x / length;
	enemy_.dir_.z = vec.z / length;

	// 方向から角度を出す
	enemy_.angle_.y = atan2f(enemy_.dir_.x, enemy_.dir_.z);
	// モデルが反対を向いているため反対にする
	enemy_.angle_.y += 180.0f * (DX_PI_F / 180.0f);

	// 回転はY軸のみとする
	enemy_.angle_.x = enemy_.angle_.z = 0.0f;

	// モデルに向きを設定
	MV1SetRotationXYZ(enemy_.modelId_, enemy_.angle_);

	// 方向単位ベクトルに速度をかけた数を座標に足しこむ
	enemy_.pos_ = VAdd(enemy_.pos_, VScale(enemy_.dir_, moveSpeed));

	// 計算した座標をモデルに適用する
	MV1SetPosition(enemy_.modelId_, enemy_.pos_);
}

void Dragon::SetRushPosition(void)
{
	// ランダムで決める
	int rush = GetRand(RANDOM_NUM);

	if (rush >= 0 && rush <= POS_1_RANGE)
	{
		rushStartPos_ = POS_1;
	}
	else if (rush > POS_1_RANGE && rush <= POS_2_RANGE)
	{
		rushStartPos_ = POS_2;
	}
	else if (rush > POS_2_RANGE && rush <= POS_3_RANGE)
	{
		rushStartPos_ = POS_3;
	}
	else if (rush > POS_3_RANGE && rush <= POS_4_RANGE)
	{
		rushStartPos_ = POS_4;
	}
}

void Dragon::PlayAttackAnim(void)
{
	// ステートに合った初期化
	switch (attackState_)
	{
	case Dragon::SELECT:
		break;
	case Dragon::RANGE_ATTACK:

		if (attackStart_)
		{
			animationController_->BlendAnimPlay(static_cast<int>(ANIM_TYPE_FLY::FLYING), AnimationController::BLEND_LATIO);
		}
		else
		{
			animationController_->BlendAnimPlay(static_cast<int>(ANIM_TYPE_FLY::ATTACK), AnimationController::BLEND_LATIO, false);
		}

		break;
	case Dragon::FORWARD_ATTACK:

		if (attackStart_ && IsAttack())
		{
			animationController_->BlendAnimPlay(static_cast<int>(ANIM_TYPE_FLY::ATTACK_2), AnimationController::BLEND_LATIO);
		}
		else
		{
			animationController_->BlendAnimPlay(static_cast<int>(ANIM_TYPE_FLY::FLYING), AnimationController::BLEND_LATIO);
		}

		break;
	case Dragon::RUSH_ATTACK:

		animationController_->BlendAnimPlay(static_cast<int>(ANIM_TYPE_FLY::FLYING), AnimationController::BLEND_LATIO);

		break;
	case Dragon::ATTACK_END:

		animationController_->BlendAnimPlay(static_cast<int>(ANIM_TYPE_FLY::FLYING), AnimationController::BLEND_LATIO);

		break;
	default:
		break;
	}
}

void Dragon::ChangeAttackStateInit(void)
{
	// ステートに合った初期化
	switch (attackState_)
	{
	case Dragon::SELECT:
		break;
	case Dragon::RANGE_ATTACK:

		attackStart_ = false;

		break;
	case Dragon::FORWARD_ATTACK:

		attackStart_ = false;
		attack_.isAttacking_ = true;
		animationNum_ = 0;
		forwardAttackStartTime_ = STARTING_TIME;

		break;
	case Dragon::RUSH_ATTACK:

		attack_.isAttacking_ = false;
		// スタート位置に行くステップにする
		rushStep_ = RUSH_STEP::STATING_POSITION;
		// 突進攻撃用の座標を取得する
		SetRushPosition();

		break;
	case Dragon::ATTACK_END:
		attack_.isAttacking_ = false;
		break;
	default:
		break;
	}
}

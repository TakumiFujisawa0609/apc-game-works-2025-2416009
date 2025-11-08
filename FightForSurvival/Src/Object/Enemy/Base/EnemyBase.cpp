#include <DxLib.h>
#include "../../../Utility/AsoUtility.h"
#include "../../../Utility/MatrixUtility.h"
#include "../../../Utility/Collision/CollisionUtility.h"
#include "../../Player/Player.h"
#include "../../Common/AnimationController.h"
#include "../../../Manager/SystemManager.h"
#include "../../../Scene/SceneManager.h"
#include "../../Magic/BatMagic/BatMagic.h"
#include "EnemyBase.h"

EnemyBase::EnemyBase(ENEMY_TYPE type, int baseModelId, int baseAttackEffectModelId, std::vector<int> animModelIds, Player* player)
	:
	player_(nullptr),
	updateCollPosCounter_(0),
	score_(0)
{
	enemy_.modelId_ = -1;
	baseAttackEffectModelId_ = -1;

	// エネミー種別
	type_ = type;

	// モデルのロード
	enemy_.modelId_ = MV1DuplicateModel(baseModelId);

	// エフェクト用モデルハンドル
	baseAttackEffectModelId_ = baseAttackEffectModelId;

	// プレイヤーのポインタを格納
	player_ = player;

	// テーブルに関数のポインタを格納
	state_.stateTable_[STATE_CHASE] = Chase;
	state_.stateTable_[STATE_RETREAT] = Retreat;
	state_.stateTable_[STATE_HIT] = Hit;
	state_.stateTable_[STATE_DEAD] = Dead;
	state_.stateTable_[STATE_END] = End;

	// アニメーションクラスの生成
	animationController_ = nullptr;
	animationController_ = new AnimationController(enemy_.modelId_);

	// 生成時には生きていない状態にする
	enemy_.isAlive_ = false;
}

EnemyBase::~EnemyBase(void)
{
}

void EnemyBase::CreateEnemy(VECTOR pos)
{
	// 指定された座標を設定
	enemy_.prevPos_ = enemy_.pos_ = attack_.rangePos_ = pos;
	MV1SetPosition(enemy_.modelId_, enemy_.pos_);
	attack_.rangePos_.y += ATTACK_RANGE_POS_OFFSET;

	// パラメータ設定
	SetParam();

	// 当たり判定用の座標を初期化
	UpdateCollisionPositions();

	// 大きさを設定する
	MV1SetScale(enemy_.modelId_, enemy_.scales_);

	// アングルを設定する
	MV1SetRotationXYZ(enemy_.modelId_, enemy_.angles_);

	enemy_.dir_ = AsoUtility::VECTOR_ZERO;

	// 初期状態
	ChangeState(ENEMY_STATE::STATE_IDLE);

	// 攻撃待ち時間を初期化
	attack_.cooldown_ = 0.0f;

	// 攻撃中か
	SetIsAttack(false);

	// 左右フラグ
	move_.isLeft_ = true;

	// タイマーを初期化
	move_.leftRightRate_ = CHANGE_RATE_MAX;

	// 座標更新のタイミング
	updateCollPosCounter_ = 0;
}

void EnemyBase::Update(void)
{
	if (!enemy_.isAlive_)
	{
		// 生存していなければ処理を行わない
		return;
	}

	// 座標更新のタイミング用カウンター
	updateCollPosCounter_++;

	// nullチェック
	if (state_.stateTable_[state_.state_])
	{
		state_.stateTable_[state_.state_](*this);
	}

	// Idle状態の時、攻撃待ち時間が0より大きければ入る
	if (state_.state_ == STATE_IDLE && attack_.cooldown_ > 0.0f)
	{
		attack_.cooldown_ -= SceneManager::GetInstance().GetDeltaTime();
		if (attack_.cooldown_ < 0.0f)
		{
			attack_.cooldown_ = 0.0f;
		}
	}

	// アニメーション更新
	if (animationController_ != nullptr)
	{
		animationController_->Update();
	}

	// 座標更新のタイミングがきたら入る
	if (updateCollPosCounter_ > COLLISION_UPDATE_INTERVAL)
	{
		// 頭用座標と体用座標を更新させる
		UpdateCollisionPositions();
		// 初期化
		updateCollPosCounter_ = 0;
	}

	// 魔法の更新
	UpdateMagic();
}

void EnemyBase::Draw(void)
{
	if (!enemy_.isAlive_)
	{
		// 生存していなければ処理を行わない
		return;
	}

	MV1DrawModel(enemy_.modelId_);

	// 魔法の描画
	DrawMagic();

#ifdef _DEBUG
	// デバッグ用：衝突判定用球体
	DrawSphere3D(enemy_.pos_, enemy_.collisionRadius_, 10, 0xff0000, 0xffffff, false);

	// 頭 デバッグ用：衝突判定用球体
	DrawSphere3D(collision_.colPos_[HEAD], enemy_.collisionRadiusHead_, 10, 0xff0000, 0xff0000, false);

	// 体 デバッグ用：衝突判定用カプセル
	DrawCapsule3D(collision_.colPos_[BODY_TOP], collision_.colPos_[BODY_UNDER],
		enemy_.collisionRadiusBody_, 10, 0xff0000, 0xff0000, false);

	// 右腕 デバッグ用：衝突判定用カプセル
	DrawCapsule3D(collision_.colPos_[ARM_TOP_R], collision_.colPos_[ARM_UNDER_R],
		enemy_.collisionRadiusArm_, 10, 0xff0000, 0xff0000, false);

	// 左腕 デバッグ用：衝突判定用カプセル
	DrawCapsule3D(collision_.colPos_[ARM_TOP_L], collision_.colPos_[ARM_UNDER_L],
		enemy_.collisionRadiusArm_, 10, 0xff0000, 0xff0000, false);

	// 右手 デバッグ用：衝突判定用球体
	DrawSphere3D(collision_.colPos_[HAND_R],enemy_.collisionRadiusHand_, 10, 0xff0000, 0xff0000, false);

	// 左手 デバッグ用：衝突判定用球体
	DrawSphere3D(collision_.colPos_[HAND_L],enemy_.collisionRadiusHand_, 10, 0xff0000, 0xff0000, false);

	// 攻撃可能範囲
	DrawSphere3D(attack_.rangePos_, attack_.range_, 10, 0x0000ff, 0x0000ff, false);
#endif // _DEBUG
}

void EnemyBase::Release(void)
{
	// アニメーションクラスの解放
	if (animationController_ != nullptr)
	{
		animationController_->Release();
		delete animationController_;
		animationController_ = nullptr;
	}

	// 中身解放
	speed_.clear();

	// 中にデータが入っていたら解放する
	MV1DeleteModel(enemy_.modelId_);
	// 中にデータが入っていたら解放する
	MV1DeleteModel(baseAttackEffectModelId_);
}

bool EnemyBase::IsCollisionState(void)
{
	// 死亡していなかったらtrueを返す
	return !(state_.state_ == STATE_DEAD || state_.state_ == STATE_END);
}

void EnemyBase::SubHp(float hp)
{
	enemy_.hp_ -= hp;

	if (enemy_.hp_ <= 0)
	{
		enemy_.hp_ = 0;
		// 攻撃を受けてHPが無くなったら死亡させる
		ChangeState(STATE_DEAD);
	}
	else
	{
		// 攻撃を受けたがまだHPがあったらダメージ時のアニメーションをさせる
		ChangeState(STATE_HIT);
	}
}

void EnemyBase::ChangeState(ENEMY_STATE state)
{
	state_.state_ = state;

	if (state_.state_ == ENEMY_STATE::STATE_END)
	{
		// エフェクト停止
		//StopEffekseer3DEffect(effectBlastPlayId_);
		// 生存判定を折る
		enemy_.isAlive_ = false;
		auto& sysIns = SystemManager::GetInstance();
		// 撃破したため、スコア加算する
		sysIns.SetScore(sysIns.GetScore() + score_);

		return;
	}

	// 死亡していないのであれば、そのステートに合ったアニメーションを再生する
	PlayAnim();
}

void EnemyBase::Chase(EnemyBase& enemy)
{
	// プレイヤーのほうへ向く
	enemy.LookPlayer();

	// 左右移動を付ける
	enemy.MoveLeftAndRight();
	
	// 方向単位ベクトルに速度をかけた数を座標に足しこむ
	enemy.enemy_.pos_ = VAdd(enemy.enemy_.pos_, VScale(enemy.enemy_.dir_, enemy.enemy_.moveSpeed_));

	// 計算した座標をモデルに適用する
	MV1SetPosition(enemy.enemy_.modelId_, enemy.enemy_.pos_);

	// 攻撃可能範囲内に入っているか確認
	if (enemy.SearchAttackRange())
	{
		 //攻撃範囲内に入ったら攻撃を行う
		enemy.ChangeState(ENEMY_STATE::STATE_ATTACK);
		// 攻撃中にする
		enemy.SetIsAttack(true);
	}
}

void EnemyBase::Retreat(EnemyBase& enemy)
{
	// プレイヤーのほうへ向く
	enemy.LookPlayer();

	// 方向単位ベクトルに速度(負の値)をかけた数を座標に足しこむ
	// 負の値を足しこむことで、プレイヤーからは離れていくような動作となる
	enemy.enemy_.pos_ = VAdd(enemy.enemy_.pos_, VScale(enemy.enemy_.dir_, -enemy.enemy_.moveSpeed_));

	// 計算した座標をモデルに適用する
	MV1SetPosition(enemy.enemy_.modelId_, enemy.enemy_.pos_);

	if (enemy.animationController_ != nullptr)
	{
		if (enemy.animationController_->IsEnd())
		{
			enemy.ChangeState(ENEMY_STATE::STATE_IDLE);
		}
	}
}

void EnemyBase::Hit(EnemyBase& enemy)
{
	if (enemy.animationController_ != nullptr)
	{
		if (enemy.animationController_->IsEnd())
		{
			enemy.ChangeState(ENEMY_STATE::STATE_RETREAT);
		}
	}
}

void EnemyBase::Dead(EnemyBase& enemy)
{
	if (enemy.animationController_ != nullptr)
	{
		if (enemy.animationController_->IsEnd())
		{
			// 死亡リアクションを終えたらENDへ移行
			enemy.ChangeState(ENEMY_STATE::STATE_END);
		}
	}
}

void EnemyBase::End(EnemyBase& enemy)
{
}

void EnemyBase::LookPlayer(void)
{
	// プレイヤーの座標
	VECTOR playerPos = player_->GetPlayer().pos_;

	// 相手へのベクトルを計算(引き算)
	VECTOR vec;
	vec.x = playerPos.x - enemy_.pos_.x;
	vec.z = playerPos.z - enemy_.pos_.z;
	vec.y = 0.0f;

	// ベクトルの正規化で単位ベクトル(方向)を取得する
	float length = sqrtf(vec.x * vec.x + vec.z * vec.z);

	if (length == 0.0f)
	{
		// プレイヤーと位置が全く同じだった場合、無理やり移動するように向きの情報を入れる
		enemy_.dir_.x = 1.0f;
		return;
	}

	// 大きさで割って単位ベクトルにする
	enemy_.dir_.x = vec.x / length;
	enemy_.dir_.z = vec.z / length;

	// 方向から角度を出す
	enemy_.angles_.y = atan2(enemy_.dir_.x, enemy_.dir_.z);

	// 今回のモデルのY軸向きが逆なので向きを反転させる
	enemy_.angles_.y += 180.0f * (DX_PI_F / 180.0f);

	// 回転はY軸のみとする
	enemy_.angles_.x = enemy_.angles_.z = 0.0f;

	// モデルに向きを設定
	MV1SetRotationXYZ(enemy_.modelId_, enemy_.angles_);
}

void EnemyBase::CraateMagic(void)
{
	// 有効な魔法を取得する
	auto* magic = GetValidMagic();
	// 初期化処理
	magic->Init();

	// モデルの回転行列取得
	MATRIX matRot = MatrixUtility::GetMatrixRotateXYZ(enemy_.angles_);

	// 方向と同じ要領で、相対座標を回転
	VECTOR localPosRot = VTransform(RELATIVE_MAGIC_POS, matRot);

	VECTOR pos = VAdd(enemy_.pos_, localPosRot);

	// 座標を更新する
	magic->CreateShot(pos, enemy_.dir_);
}

void EnemyBase::UpdateCollisionPositions(void)
{
#pragma region 頭

	collision_.colPos_[HEAD] = GetBoneWorldPosition(collision_.headBone_, collision_.offsetHead_);

#pragma endregion

#pragma region 体

	collision_.colPos_[BODY_TOP] = GetBoneWorldPosition(collision_.bodyBoneTop_, collision_.offsetBodyTop_);

	collision_.colPos_[BODY_UNDER] = GetBoneWorldPosition(collision_.bodyBoneUnder_, collision_.offsetBodyUnder_);

#pragma endregion

#pragma region 右腕,右手

	collision_.colPos_[ARM_TOP_R] = GetBoneWorldPosition(collision_.armBoneTopR_, collision_.offsetArmTop_);

	collision_.colPos_[ARM_UNDER_R] = GetBoneWorldPosition(collision_.handBoneR_, collision_.offsetArmUnder_);

	// 右手
	collision_.colPos_[HAND_R] = GetBoneWorldPosition(collision_.handBoneR_, collision_.offsetHand_);

#pragma endregion

#pragma region 左腕,左手

	collision_.colPos_[ARM_TOP_L] = GetBoneWorldPosition(collision_.armBoneTopL_, collision_.offsetArmTop_);

	collision_.colPos_[ARM_UNDER_L] = GetBoneWorldPosition(collision_.handBoneL_, collision_.offsetArmUnder_);

	// 左手
	collision_.colPos_[HAND_L] = GetBoneWorldPosition(collision_.handBoneL_, collision_.offsetHand_);

#pragma endregion

	// 攻撃範囲判定用の
	attack_.rangePos_ = enemy_.pos_;
	attack_.rangePos_.y += ATTACK_RANGE_POS_OFFSET;

}

VECTOR EnemyBase::GetBoneWorldPosition(int bone,float offset)
{
	// フレームのローカル座標からワールド座標に変換する行列を取得
	MATRIX boneMatrix = MV1GetFrameLocalWorldMatrix(enemy_.modelId_, bone);

	// ローカルY軸方向の単位ベクトルをワールド座標系で取得
	VECTOR boneYAxis = VGet(boneMatrix.m[1][0], boneMatrix.m[1][1], boneMatrix.m[1][2]);

	// 単位ベクトルにする（スケールがかかっていても正規化して純粋な方向ベクトルにする）
	boneYAxis = VNorm(boneYAxis);

	// 行列から回転の情報を取得
	MATRIX boneRotate = MGetRotElem(boneMatrix);

	// ローカルオフセット(offset)をワールド座標のオフセットに変換
	VECTOR WorldOffset = VScale(boneYAxis, offset);

	// 行列から平行移動の情報取得する
	VECTOR bonePos = MGetTranslateElem(boneMatrix);

	// 平行移動の情報に、ワールド座標のオフセットを加算する
	VECTOR retPos = VAdd(bonePos, WorldOffset);

	return retPos;
}

bool EnemyBase::SearchAttackRange(void)
{
	// 攻撃可能範囲にプレイヤーがいるか確認
	return CollisionUtility::IsCollidingSphereCapsule(attack_.rangePos_,attack_.range_,player_->GetCollisionPosTop(),player_->GetCollisionPosUnder(),Player::COLLISION_RADIUS);
}

void EnemyBase::Extrusion(VECTOR overlap)
{
	// 押し出しを行う
	enemy_.pos_ = VAdd(enemy_.pos_, overlap);

	// 座標をモデルに設定
	MV1SetPosition(enemy_.modelId_, enemy_.pos_);
}

void EnemyBase::DuplicateAnimation(std::vector<float> speed, std::vector<int> animModelIds)
{
	for (int i = 0; i < animModelIds.size(); i++)
	{
		animationController_->Duplicate(i, speed[i], animModelIds[i]);
	}
}

void EnemyBase::MoveLeftAndRight(void)
{
	if (move_.isLeft_)
	{
		// 左率を上げる
		move_.leftRightRate_ -= SceneManager::GetInstance().GetDeltaTime();

		if (move_.leftRightRate_ < CHANGE_RATE_MIN)
		{
			move_.isLeft_ = !move_.isLeft_;
		}
	}
	else
	{
		// 右率を上げる
		move_.leftRightRate_ += SceneManager::GetInstance().GetDeltaTime();

		if (move_.leftRightRate_ > CHANGE_RATE_MAX)
		{
			move_.isLeft_ = !move_.isLeft_;
		}
	}

	VECTOR moveDir = VGet(move_.leftRightRate_, 0.0f, 0.0f);

	enemy_.dir_ = VAdd(enemy_.dir_, moveDir);
}

void EnemyBase::PlayAnim(void)
{
	if (animationController_ == nullptr)
	{
		// アニメーションコントローラーの中身が空だったら処理を行わない
		return;
	}

	switch (state_.state_)
	{
	case ENEMY_STATE::STATE_IDLE:
		animationController_->BlendAnimPlay(static_cast<int>(ENEMY_STATE::STATE_IDLE), AnimationController::BLEND_LATIO);
		break;
	case ENEMY_STATE::STATE_CHASE:
		animationController_->BlendAnimPlay(static_cast<int>(ENEMY_STATE::STATE_CHASE), AnimationController::BLEND_LATIO);
		break;
	case ENEMY_STATE::STATE_ATTACK:
		animationController_->BlendAnimPlay(static_cast<int>(ENEMY_STATE::STATE_ATTACK), AnimationController::BLEND_LATIO, false);
		break;
	case ENEMY_STATE::STATE_RETREAT:
		animationController_->BlendAnimPlay(static_cast<int>(ENEMY_STATE::STATE_RETREAT), AnimationController::BLEND_LATIO, false);
		break;
	case ENEMY_STATE::STATE_HIT:
		animationController_->BlendAnimPlay(static_cast<int>(ENEMY_STATE::STATE_HIT), AnimationController::BLEND_LATIO, false);
		break;
	case ENEMY_STATE::STATE_DEAD:
		animationController_->BlendAnimPlay(static_cast<int>(ENEMY_STATE::STATE_DEAD), AnimationController::BLEND_LATIO, false);
		break;
	default:
		break;
	}

}

void EnemyBase::UpdateMagic(void)
{
	// 魔法の更新
	for (auto& Magic : magics_)
	{
		if (Magic->GetMagic().isExists_)
		{
			Magic->Update();
		}
	}
}

void EnemyBase::DrawMagic(void)
{
	// 魔法の更新
	for (auto& Magic : magics_)
	{
		if (Magic->GetMagic().isExists_)
		{
			Magic->Draw();
		}
	}
}

MagicBase* EnemyBase::GetValidMagic(void)
{
	size_t size = magics_.size();

	for (int i = 0; i < size; i++)
	{
		// 未使用(生存していない)で、かつ、魔法の種別が同じ
		if (!magics_[i]->GetMagic().isExists_ && !magics_[i]->GetMagic().isDraw_)
		{
			return magics_[i];
		}
	}

	// 未使用の魔法がなかった場合新しい魔法を生成
	MagicBase* Magic;

	// 新しい魔法のインスタンスを生成する
	Magic = new BatMagic(TYPE_MAGIC::FIRE_MAGIC, baseAttackEffectModelId_);

	// 可変長配列に追加
	magics_.push_back(Magic);

	return Magic;
}

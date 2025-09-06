#include <DxLib.h>
#include "../../Utility/AsoUtility.h"
#include "../Player/Player.h"
#include "../Common/AnimationController.h"
#include "EnemyBase.h"
#include "Zombie.h"

EnemyBase::EnemyBase(void)
{
	enemy_.modelId_ = -1;
	baseAttackEffectModelId_ = -1;
	stateTable_[STATE_CHASE] = Chase;
	stateTable_[STATE_RETREAT] = Retreat;
	stateTable_[STATE_HIT] = Hit;
	stateTable_[STATE_DEAD] = Dead;
	stateTable_[STATE_END] = End;
}

EnemyBase::~EnemyBase(void)
{
}

void EnemyBase::Init(ENEMY_TYPE type, int baseModelId, int baseAttackEffectModelId, VECTOR pos, Player* player)
{
	// エネミー種別
	type_ = type;

	// モデルのロード
	enemy_.modelId_ = MV1DuplicateModel(baseModelId);

	// エフェクト用モデルハンドル
	baseAttackEffectModelId_ = baseAttackEffectModelId;

	// 指定された座標を設定
	enemy_.pos_ = pos;
	MV1SetPosition(enemy_.modelId_, enemy_.pos_);

	// プレイヤーのポインタを格納
	player_ = player;

	// パラメータ設定
	SetParam();

	// 当たり判定用の座標を初期化
	UpdateCollisionPositions();

	// アングルを設定する
	MV1SetRotationXYZ(enemy_.modelId_, enemy_.angles_);

	// 大きさを設定する
	MV1SetScale(enemy_.modelId_, enemy_.scales_);

	enemy_.dir_ = AsoUtility::VECTOR_ZERO;

	animationController_ = new AnimationController(enemy_.modelId_);

	// アニメーション登録
	AddAnimation();

	// 初期状態
	ChangeState(ENEMY_STATE::STATE_IDLE);
}

void EnemyBase::Update(void)
{
	// nullチェック
	if (stateTable_[state_])
	{
		stateTable_[state_](*this);
	}

	// アニメーション更新
	animationController_->Update();

	// 頭用座標と体用座標を更新させる
	UpdateCollisionPositions();
}

void EnemyBase::Draw(void)
{
	if (!enemy_.isAlive_)
	{
		// 生存していなければ処理を行わない
		return;
	}

	MV1DrawModel(enemy_.modelId_);

#ifdef _DEBUG
	// デバッグ用：衝突判定用球体
	//DrawSphere3D(enemy_.pos_, enemy_.collisionRadius_, 10, 0xff0000, 0xff0000, false);
	// 頭 デバッグ用：衝突判定用球体
	DrawSphere3D(collision_.colPos_[HEAD], enemy_.collisionRadius_, 10, 0xff0000, 0xff0000, false);

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

	// 右脚 デバッグ用：衝突判定用カプセル
	DrawCapsule3D(collision_.colPos_[LEG_TOP_R], collision_.colPos_[LEG_UNDER_R],
		enemy_.collisionRadiusLeg_, 10, 0xff0000, 0xff0000, false);

	// 左脚 デバッグ用：衝突判定用カプセル
	DrawCapsule3D(collision_.colPos_[LEG_TOP_L], collision_.colPos_[LEG_UNDER_L],
		enemy_.collisionRadiusLeg_, 10, 0xff0000, 0xff0000, false);
#endif // _DEBUG
}

void EnemyBase::Release(void)
{
	if (enemy_.modelId_ != -1)
	{
		// 中にデータが入っていたら解放する
		MV1DeleteModel(enemy_.modelId_);
	}

	if (baseAttackEffectModelId_ != -1)
	{
		// 中にデータが入っていたら解放する
		MV1DeleteModel(baseAttackEffectModelId_);
	}
}

bool EnemyBase::IsCollisionState(void)
{
	// 死亡していなかったらtrueを返す
	return !(state_ == STATE_DEAD || state_ == STATE_END);
}

void EnemyBase::SubHp(int hp)
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
	state_ = state;

	if (state_ == ENEMY_STATE::STATE_END)
	{
		// エフェクト停止
		//StopEffekseer3DEffect(effectBlastPlayId_);
		// 生存判定を折る
		enemy_.isAlive_ = false;
	}
	else
	{
		animationController_->Play(static_cast<int>(state_));
	}
}

void EnemyBase::Chase(EnemyBase& enemy)
{
	// プレイヤーのほうへ向く
	enemy.LookPlayer();

	// 方向単位ベクトルに速度をかけた数を座標に足しこむ
	enemy.enemy_.pos_ = VAdd(enemy.enemy_.pos_, VScale(enemy.enemy_.dir_, enemy.enemy_.moveSpeed_));

	// 計算した座標をモデルに適用する
	MV1SetPosition(enemy.enemy_.modelId_, enemy.enemy_.pos_);

	//if (範囲内に入っていたら攻撃)
	{
		// 攻撃範囲内に入ったら攻撃を行う
		//enemy.ChangeState(ENEMY_STATE::STATE_ATTACK);
	}
}

void EnemyBase::Retreat(EnemyBase& enemy)
{
	//if (アニメーションを終えたら)
	{
		enemy.ChangeState(ENEMY_STATE::STATE_IDLE);
	}
}

void EnemyBase::Hit(EnemyBase& enemy)
{
	//if (アニメーションを終えたら)
	{
		enemy.ChangeState(ENEMY_STATE::STATE_RETREAT);
	}
}

void EnemyBase::Dead(EnemyBase& enemy)
{
	//if (アニメーション終わったら入る)
	{
		// 死亡リアクションを終えたらENDへ移行
		enemy.ChangeState(ENEMY_STATE::STATE_END);
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
		vec.x = vec.z = 0.0f;
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

#pragma region 右脚

	collision_.colPos_[LEG_TOP_R] = GetBoneWorldPosition(collision_.legBoneTopR_, collision_.offsetLegTop_);

	collision_.colPos_[LEG_UNDER_R] = GetBoneWorldPosition(collision_.legBoneUnderR_, collision_.offsetLegUnder_);

#pragma endregion

#pragma region 左脚

	collision_.colPos_[LEG_TOP_L] = GetBoneWorldPosition(collision_.legBoneTopL_, collision_.offsetLegTop_);

	collision_.colPos_[LEG_UNDER_L] = GetBoneWorldPosition(collision_.legBoneUnderL_, collision_.offsetLegUnder_);

#pragma endregion

}

VECTOR EnemyBase::GetBoneWorldPosition(int bone,float offset)
{
	// フレームのローカル座標からワールド座標に変換する行列を取得
	MATRIX boneMatrix = MV1GetFrameLocalWorldMatrix(enemy_.modelId_, bone);
	// 行列から平行移動の情報を座標に格納する
	VECTOR retPos = VGet(boneMatrix.m[3][0], boneMatrix.m[3][1], boneMatrix.m[3][2]);
	retPos.y += offset;

	return retPos;
}

int EnemyBase::SearchFrame(const std::string& boneName)
{
	std::string fullBoneName = "mixamorig5:" + boneName;

	return MV1SearchFrame(enemy_.modelId_, fullBoneName.c_str());
}

#include <DxLib.h>
#include "../../../../Manager/SceneManager.h"
#include "BulletBase.h"

BulletBase::BulletBase(int baseModelId)
{
	bullet_.modelId_ = MV1DuplicateModel(baseModelId);
}

BulletBase::~BulletBase(void)
{
}

void BulletBase::CreateShot(VECTOR pos, VECTOR dir)
{
	// 弾の発射位置を設定
	bullet_.prevPos_ = bullet_.pos_ = pos;
	// 弾の発射方向の設定
	bullet_.dir_ = dir;
	// 弾の生存判定
	bullet_.isAlive_ = true;
	state_ = STATE::SHOT;
	// パラメータ設定
	SetParam();
	// 大きさの設定
	MV1SetScale(bullet_.modelId_, bullet_.scale_);
	// 回転の設定
	MV1SetRotationXYZ(bullet_.modelId_, bullet_.rotate_);
	// 位置の設定
	MV1SetPosition(bullet_.modelId_, bullet_.pos_);
}

void BulletBase::Update(void)
{

	if (!bullet_.isAlive_)
	{
		// 生存していなければ処理中断
		return;
	}

	switch (state_)
	{
	case BulletBase::STATE::NONE:
		break;
	case BulletBase::STATE::SHOT:
		UpdateShot();
		break;
	case BulletBase::STATE::BLAST:
		UpdateBlast();
		break;
	case BulletBase::STATE::END:
		UpdateEnd();
		break;
	default:
		break;
	}
}

void BulletBase::Draw(void)
{

	if (!bullet_.isAlive_)
	{
		// 生存していなければ処理中断
		return;
	}

	MV1DrawModel(bullet_.modelId_);

#ifdef _DEBUG
	// デバッグ用：衝突判定用球体
	DrawSphere3D(bullet_.pos_, bullet_.collisionRadius_, 10, 0x0000ff, 0x0000ff, false);
#endif // _DEBUG
}

void BulletBase::Release(void)
{
	MV1DeleteModel(bullet_.modelId_);
}

bool BulletBase::IsCollisionState(void)
{
	return state_ == STATE::SHOT;
}

void BulletBase::ChangeState(STATE state)
{
	state_ = state;

	switch (state_)
	{
	case BulletBase::STATE::NONE:
		ChangeNon();
		break;
	case BulletBase::STATE::SHOT:
		ChangeShot();
		break;
	case BulletBase::STATE::BLAST:
		ChangeBlast();
		break;
	case BulletBase::STATE::END:
		ChangeEnd();
		break;
	default:
		break;
	}
}


void BulletBase::ReduceCntAlive(void)
{

	bullet_.cntAlive_-= SceneManager::GetInstance().GetDeltaTime();
	if (bullet_.cntAlive_ < 0)
	{
		// 弾の存在可能時間が過ぎたら消す
		ChangeState(STATE::BLAST);
	}

}

void BulletBase::UpdateShot(void)
{
	// 移動前の座標を取得しておく
	bullet_.prevPos_ = bullet_.pos_;

	// 弾を移動させる
	bullet_.pos_ = VAdd(bullet_.pos_, VScale(bullet_.dir_, bullet_.speed_));

	//// 加速度的に重力を加える
	//gravityPow_ +=
	//	SceneManager::GRAVITY * SceneManager::GetInstance().GetDeltaTime();
	//pos_ = VAdd(pos_, VScale({ 0.0f, -1.0f, 0.0f }, gravityPow_));

	// 位置の設定
	MV1SetPosition(bullet_.modelId_, bullet_.pos_);

	// 生存カウンタの減少
	ReduceCntAlive();
}

void BulletBase::UpdateBlast(void)
{
	/*if (IsEffekseer3DEffectPlaying(effectBlastPlayId_) == -1)
	{*/
		ChangeState(STATE::END);
	/*}*/
}

void BulletBase::UpdateEnd(void)
{
	bullet_.isAlive_ = false;
}

void BulletBase::ChangeNon(void)
{
}

void BulletBase::ChangeShot(void)
{
}

void BulletBase::ChangeBlast(void)
{
	//EffectResManager::TYPE type;
	//switch (effectType_)
	//{
	//case ShotBase::EFFECT_TYPE::GROUND:
	//	type = EffectResManager::TYPE::BLAST_GROUND;
	//	break;
	//case ShotBase::EFFECT_TYPE::HIT:
	//	type = EffectResManager::TYPE::BLAST_HIT;
	//	break;
	//default:
	//	break;
	//}

	//// エフェクトの再生
	//int resId = EffectResManager::GetInstance().GetResourceId(type);
	//effectBlastPlayId_ = PlayEffekseer3DEffect(resId);

	//// エフェクトの大きさ
	//float SCALE = 10.0f;
	//SetScalePlayingEffekseer3DEffect(
	//	effectBlastPlayId_, SCALE, SCALE, SCALE);

	//// エフェクトの回転
	//VECTOR angles = { 0.0f, 0.0f, 0.0f };
	//SetRotationPlayingEffekseer3DEffect(
	//	effectBlastPlayId_, angles.x, angles.y, angles.z);

	//// エフェクトの位置
	//SetPosPlayingEffekseer3DEffect(
	//	effectBlastPlayId_, pos_.x, pos_.y, pos_.z);
}

void BulletBase::ChangeEnd(void)
{
	// エフェクト停止
	//StopEffekseer3DEffect(effectBlastPlayId_);
}


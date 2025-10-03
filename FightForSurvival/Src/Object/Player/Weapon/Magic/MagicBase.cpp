#include <DxLib.h>
#include "../../../../Manager/SceneManager.h"
#include "MagicBase.h"

MagicBase::MagicBase(int baseModelId)
{
	Magic_.modelId_ = MV1DuplicateModel(baseModelId);
}

MagicBase::~MagicBase(void)
{
}

void MagicBase::CreateShot(VECTOR pos, VECTOR dir)
{
	// 魔法の発射位置を設定
	Magic_.prevPos_ = Magic_.pos_ = pos;
	// 魔法の発射方向の設定
	Magic_.dir_ = dir;
	// 魔法の生存判定
	Magic_.isAlive_ = true;
	state_ = STATE::SHOT;
	// パラメータ設定
	SetParam();
	// 大きさの設定
	MV1SetScale(Magic_.modelId_, Magic_.scale_);
	// 回転の設定
	MV1SetRotationXYZ(Magic_.modelId_, Magic_.rotate_);
	// 位置の設定
	MV1SetPosition(Magic_.modelId_, Magic_.pos_);
}

void MagicBase::Update(void)
{

	if (!Magic_.isAlive_)
	{
		// 生存していなければ処理中断
		return;
	}

	switch (state_)
	{
	case MagicBase::STATE::NONE:
		break;
	case MagicBase::STATE::SHOT:
		UpdateShot();
		break;
	case MagicBase::STATE::BLAST:
		UpdateBlast();
		break;
	case MagicBase::STATE::END:
		UpdateEnd();
		break;
	default:
		break;
	}
}

void MagicBase::Draw(void)
{

	if (!Magic_.isAlive_)
	{
		// 生存していなければ処理中断
		return;
	}

	MV1DrawModel(Magic_.modelId_);

#ifdef _DEBUG
	// デバッグ用：衝突判定用球体
	DrawSphere3D(Magic_.pos_, Magic_.collisionRadius_, 10, 0x0000ff, 0x0000ff, false);
#endif // _DEBUG
}

void MagicBase::Release(void)
{
	MV1DeleteModel(Magic_.modelId_);
}

bool MagicBase::IsCollisionState(void)
{
	return state_ == STATE::SHOT;
}

void MagicBase::ChangeState(STATE state)
{
	state_ = state;

	switch (state_)
	{
	case MagicBase::STATE::NONE:
		ChangeNon();
		break;
	case MagicBase::STATE::SHOT:
		ChangeShot();
		break;
	case MagicBase::STATE::BLAST:
		ChangeBlast();
		break;
	case MagicBase::STATE::END:
		ChangeEnd();
		break;
	default:
		break;
	}
}


void MagicBase::ReduceCntAlive(void)
{

	Magic_.cntAlive_-= SceneManager::GetInstance().GetDeltaTime();
	if (Magic_.cntAlive_ < 0)
	{
		// 魔法の存在可能時間が過ぎたら消す
		ChangeState(STATE::BLAST);
	}

}

void MagicBase::UpdateShot(void)
{
	// 移動前の座標を取得しておく
	Magic_.prevPos_ = Magic_.pos_;

	// 魔法を移動させる
	Magic_.pos_ = VAdd(Magic_.pos_, VScale(Magic_.dir_, Magic_.speed_));

	//// 加速度的に重力を加える
	//gravityPow_ +=
	//	SceneManager::GRAVITY * SceneManager::GetInstance().GetDeltaTime();
	//pos_ = VAdd(pos_, VScale({ 0.0f, -1.0f, 0.0f }, gravityPow_));

	// 位置の設定
	MV1SetPosition(Magic_.modelId_, Magic_.pos_);

	// 生存カウンタの減少
	ReduceCntAlive();
}

void MagicBase::UpdateBlast(void)
{
	/*if (IsEffekseer3DEffectPlaying(effectBlastPlayId_) == -1)
	{*/
		ChangeState(STATE::END);
	/*}*/
}

void MagicBase::UpdateEnd(void)
{
	Magic_.isAlive_ = false;
}

void MagicBase::ChangeNon(void)
{
}

void MagicBase::ChangeShot(void)
{
}

void MagicBase::ChangeBlast(void)
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

void MagicBase::ChangeEnd(void)
{
	// エフェクト停止
	//StopEffekseer3DEffect(effectBlastPlayId_);
}


#include <DxLib.h>
#include "../../Scene/SceneManager.h"
#include "MagicBase.h"

MagicBase::MagicBase(int baseModelId)
{
	magic_.modelId_ = MV1DuplicateModel(baseModelId);
}

MagicBase::~MagicBase(void)
{
}

void MagicBase::Init(void)
{
	magic_.isAlive_ = false;
	magic_.isDraw_ = true;
	state_ = STATE::NONE;

	// パラメータ設定
	SetParam();
	// 大きさの設定
	MV1SetScale(magic_.modelId_, magic_.scale_);
	// 回転の設定
	MV1SetRotationXYZ(magic_.modelId_, magic_.rotate_);
	// 位置の設定
	MV1SetPosition(magic_.modelId_, magic_.pos_);
}

void MagicBase::CreateShot(VECTOR pos, VECTOR dir)
{
	// 魔法の発射位置を設定
	magic_.prevPos_ = magic_.pos_ = pos;
	// 魔法の発射方向の設定
	magic_.dir_ = dir;
	// 魔法の生存判定
	magic_.isAlive_ = true;
	state_ = STATE::SHOT;
}

void MagicBase::Update(void)
{

	if (!magic_.isAlive_)
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

	if (!magic_.isDraw_)
	{
		// 生存していなければ処理中断
		return;
	}

	MV1DrawModel(magic_.modelId_);

	// デバッグ用：衝突判定用球体
	DrawSphere3D(magic_.pos_, magic_.collisionRadius_, 10, 0x0000ff, 0x0000ff, false);
}

void MagicBase::Release(void)
{
	MV1DeleteModel(magic_.modelId_);
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

void MagicBase::ChargeMagic(void)
{
	// 魔法を徐々に大きくする(チャージする)
	magic_.collisionRadius_ += CHARGE_POW;

	if (magic_.collisionRadius_ > CHARGE_MAX)
	{
		magic_.collisionRadius_ = CHARGE_MAX;
	}

	// 大きさの設定
	//MV1SetScale(magic_.modelId_, magic_.scale_);
}

void MagicBase::UpdatePos(VECTOR pos)
{
	// 座標を更新する
	magic_.pos_ = pos;

	MV1SetPosition(magic_.modelId_, magic_.pos_);
}

void MagicBase::ReduceCntAlive(void)
{

	magic_.cntAlive_-= SceneManager::GetInstance().GetDeltaTime();
	if (magic_.cntAlive_ < 0)
	{
		// 魔法の存在可能時間が過ぎたら消す
		ChangeState(STATE::BLAST);
	}

}

void MagicBase::UpdateShot(void)
{
	// 移動前の座標を取得しておく
	magic_.prevPos_ = magic_.pos_;

	// 魔法を移動させる
	magic_.pos_ = VAdd(magic_.pos_, VScale(magic_.dir_, magic_.speed_));

	//// 加速度的に重力を加える
	//gravityPow_ +=
	//	SceneManager::GRAVITY * SceneManager::GetInstance().GetDeltaTime();
	//pos_ = VAdd(pos_, VScale({ 0.0f, -1.0f, 0.0f }, gravityPow_));

	// 位置の設定
	MV1SetPosition(magic_.modelId_, magic_.pos_);

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
	magic_.isAlive_ = false;
	magic_.isDraw_ = false;
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


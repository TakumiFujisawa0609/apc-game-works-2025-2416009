#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "../../../Scene/SceneManager.h"
#include "../../../Manager/EffectResManager/EffectResManager.h"
#include "MagicBase.h"

MagicBase::MagicBase(TYPE_MAGIC typeMagic, int baseModelId)
{
	magic_.typeMagic_ = typeMagic;
	magic_.modelId_ = MV1DuplicateModel(baseModelId);
}

MagicBase::~MagicBase(void)
{
}

void MagicBase::Init(void)
{
	magic_.isExists_ = false;
	magic_.isDraw_ = false;
	state_ = STATE::CHARGE;

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
	magic_.isDraw_ = true;

	// チャージ状態
	ChangeState(STATE::CHARGE);
}

void MagicBase::Update(void)
{
	if (!magic_.isExists_ && !magic_.isDraw_)
	{
		// 生存していなければ処理中断
		return;
	}

	switch (state_)
	{
	case MagicBase::STATE::CHARGE:
		UpdateCharge();
		break;
	case MagicBase::STATE::SHOT:
		UpdateShot();
		break;
	case MagicBase::STATE::BLAST:
		UpdateBlast();
		break;
	case MagicBase::STATE::END:
		break;
	default:
		break;
	}
}

void MagicBase::Draw(void)
{
	if (!magic_.isDraw_)
	{
		// 描画していなければ処理中断
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

void MagicBase::ChangeState(STATE state)
{
	state_ = state;

	switch (state_)
	{
	case MagicBase::STATE::CHARGE:
		ChangeCharge();
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

void MagicBase::UpdatePos(VECTOR pos)
{
	// 座標を更新する
	magic_.pos_ = pos;
	MV1SetPosition(magic_.modelId_, magic_.pos_);

	// エフェクトの位置の更新
	SetPosPlayingEffekseer3DEffect(
		effectPlayId_, magic_.pos_.x, magic_.pos_.y, magic_.pos_.z);
}

void MagicBase::UpdateDir(VECTOR dir)
{
	// 向きを更新する
	magic_.dir_ = dir;

	// エフェクトの回転
	dir.y += 90.0f * 180 / DX_PI_F;
	SetRotationPlayingEffekseer3DEffect(
		effectPlayId_, magic_.dir_.x, magic_.dir_.y, magic_.dir_.z);
}

void MagicBase::UpdateCharge(void)
{
	// 魔法を徐々に大きくする(チャージする)
	magic_.collisionRadius_ += chargePow_;

	if (magic_.collisionRadius_ > chargeMax_)
	{
		magic_.collisionRadius_ = chargeMax_;
	}
}

void MagicBase::UpdateShot(void)
{
	// 移動前の座標を取得しておく
	magic_.prevPos_ = magic_.pos_;

	// 魔法を移動させる
	magic_.pos_ = VAdd(magic_.pos_, VScale(magic_.dir_, magic_.speed_));
	
	// 位置の設定
	MV1SetPosition(magic_.modelId_, magic_.pos_);

	// 生存カウンタの減少
	ReduceCntAlive();
}

void MagicBase::UpdateBlast(void)
{
	if (IsEffekseer3DEffectPlaying(effectPlayId_) == -1)
	{
		ChangeState(STATE::END);
	}
}

void MagicBase::ChangeEnd(void)
{
	// エフェクト停止
	StopEffekseer3DEffect(effectPlayId_);

	magic_.isExists_ = false;
	magic_.isDraw_ = false;
}

void MagicBase::ReduceCntAlive(void)
{
	// 魔法の生存時間を減らす
	magic_.cntAlive_ -=
		SceneManager::GetInstance().GetDeltaTime();

	// 魔法の存在可能時間が過ぎたら消す
	if (magic_.cntAlive_ < 0)
	{
		ChangeState(STATE::BLAST);
	}
}

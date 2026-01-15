#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "../../../Scene/SceneManager.h"
#include "../../../Manager/EffectResManager/EffectResManager.h"
#include "MagicBase.h"

MagicBase::MagicBase(TYPE_MAGIC typeMagic, int baseModelId, VECTOR* weponPos)
{
	magic_.typeMagic_ = typeMagic;
	magic_.modelId_ = MV1DuplicateModel(baseModelId);
	weponPos_ = weponPos;
}

MagicBase::~MagicBase(void)
{
}

void MagicBase::Init(void)
{
	magic_.isExists_ = false;
	magic_.isDraw_ = false;
	state_ = MAGIC_STATE::CHARGE;

	// パラメータ設定
	SetParam();

	// 大きさの設定
	MV1SetScale(magic_.modelId_, magic_.scale_);
	// 回転の設定
	MV1SetRotationXYZ(magic_.modelId_, magic_.rotate_);
	// 位置の設定
	MV1SetPosition(magic_.modelId_, magic_.pos_);
}

void MagicBase::ChargeShot(VECTOR pos, VECTOR dir)
{
	// 魔法の発射位置を設定
	magic_.prevPos_ = magic_.pos_ = pos;

	// 魔法の発射方向の設定
	magic_.dir_ = dir;

	// 描画フラグのみ立てる
	magic_.isDraw_ = true;

	// 最大チャージフラグを初期化
	isMaxCharge_ = false;

	// チャージ状態
	ChangeState(MAGIC_STATE::CHARGE);
}

void MagicBase::CreateShot(VECTOR pos, VECTOR dir)
{
	// 魔法の発射位置を設定
	magic_.prevPos_ = magic_.pos_ = pos;

	// 魔法の発射方向の設定
	magic_.dir_ = dir;

	magic_.isDraw_ = true;
	magic_.isExists_ = true;

	// チャージ状態
	ChangeState(MAGIC_STATE::SHOT);
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
	case MAGIC_STATE::CHARGE:

		UpdateCharge();

		UpdateEffectPos();

		break;
	case MAGIC_STATE::SHOT:
		UpdateShot();
		break;
	case MAGIC_STATE::BLAST:
		UpdateBlast();
		break;
	case MAGIC_STATE::END:
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

#ifdef _DEBUG

	// デバッグ用：衝突判定用球体
	DrawSphere3D(magic_.pos_, magic_.collisionRadius_, 10, 0x0000ff, 0x0000ff, false);

#endif // _DEBUG

}

void MagicBase::Release(void)
{
	MV1DeleteModel(magic_.modelId_);
}

void MagicBase::ChangeState(MAGIC_STATE state)
{
	// エフェクト停止
	StopEffekseer3DEffect(effectPlayId_);

	state_ = state;

	switch (state_)
	{
	case MAGIC_STATE::CHARGE:
		ChangeCharge();
		break;
	case MAGIC_STATE::SHOT:
		ChangeShot();
		break;
	case MAGIC_STATE::BLAST:
		ChangeBlast();
		break;
	case MAGIC_STATE::END:
		ChangeEnd();
		break;
	default:
		break;
	}
}

void MagicBase::UpdateEffectPos(void)
{
	// 座標を更新する
	magic_.pos_ = *weponPos_;
	MV1SetPosition(magic_.modelId_, magic_.pos_);

	// エフェクトの位置の更新
	SetPosPlayingEffekseer3DEffect(
		effectPlayId_, magic_.pos_.x, magic_.pos_.y, magic_.pos_.z);
}

void MagicBase::UpdateEffectPos(VECTOR pos)
{
	// 座標を更新する
	magic_.pos_ = pos;
	MV1SetPosition(magic_.modelId_, magic_.pos_);
}

void MagicBase::UpdateEffectDir(VECTOR dir)
{
	// 向きを更新する
	magic_.dir_ = dir;

	// エフェクトの回転
	// 方向から角度を出す
	VECTOR angle;
	angle.y = atan2(dir.x, dir.z);

	// XZのベクトルの長さを計算する
	float XZLength = sqrtf(dir.x * dir.x + dir.z * dir.z);

	// X軸の角度を計算する
	angle.x = atan2(dir.y, XZLength);

	// 回転はXY軸のみとする
	angle.z = 0.0f;

	SetRotationPlayingEffekseer3DEffect(
		effectPlayId_, -angle.x, angle.y, angle.z);
}

void MagicBase::UpdateCharge(void)
{
	// 魔法を徐々に大きくする(チャージする)
	if (isMaxCharge_)
	{
		// 指定のエフェクトが表示されていなかったら再生
		if (IsEffekseer3DEffectPlaying(effectPlayId_) == -1)
		{
			// チャージ状態のエフェクト再生
			// 最大チャージ状態のエフェクト再生
			effectPlayId_ = EffectResManager::GetInstance().PlayEffect(
				effectScale_, magic_.dir_, magic_.pos_, EffectResManager::TYPE::PLAYER_MAGIC_CHARGE_MAX);
		}
	}
	else
	{
		magic_.collisionRadius_ += chargePow_;

		if (magic_.collisionRadius_ > chargeMax_)
		{
			magic_.collisionRadius_ = chargeMax_;
			// 最大になったことをフラグで知らせる
			isMaxCharge_ = true;

			// 他のエフェクトが表示されていたら停止する
			if (IsEffekseer3DEffectPlaying(effectPlayId_) != -1)
			{
				StopEffekseer3DEffect(effectPlayId_);
			}

			// 最大チャージ状態のエフェクト再生
			effectPlayId_ = EffectResManager::GetInstance().PlayEffect(
				effectScale_, magic_.dir_, magic_.pos_, EffectResManager::TYPE::PLAYER_MAGIC_CHARGE_MAX);
		}
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

	// エフェクトの位置の更新
	SetPosPlayingEffekseer3DEffect(
		effectPlayId_, magic_.pos_.x, magic_.pos_.y, magic_.pos_.z);

	// 生存カウンタの減少
	ReduceCntAlive();
}

void MagicBase::UpdateBlast(void)
{
	if (IsEffekseer3DEffectPlaying(effectPlayId_) == -1)
	{
		ChangeState(MAGIC_STATE::END);
	}
}

void MagicBase::ChangeEnd(void)
{
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
		ChangeState(MAGIC_STATE::BLAST);
	}
}

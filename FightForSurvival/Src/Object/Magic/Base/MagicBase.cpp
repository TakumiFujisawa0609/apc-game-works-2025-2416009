#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "../../../Scene/SceneManager.h"
#include "../../../Manager/EffectResManager/EffectResManager.h"
#include "MagicBase.h"

MagicBase::MagicBase(TYPE_MAGIC typeMagic, VECTOR* weponPos)
{
	// 魔法の種類を貰う
	magic_.typeMagic_ = typeMagic;

	// 杖の座標ポインタ
	weponPos_ = weponPos;
}

MagicBase::~MagicBase(void)
{
}

void MagicBase::Init(void)
{
	// 存在フラグの初期化
	magic_.isExists_ = false;

	// 描画フラグの初期化
	magic_.isDraw_ = false;

	// チャージ可能状態にしておく
	state_ = MAGIC_STATE::CHARGE;

	// パラメータ設定
	SetParam();
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

	// 存在フラグを立てる
	magic_.isExists_ = true;

	// 描画フラグを立てる
	magic_.isDraw_ = true;

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
	// チャージ可能状態
	case MAGIC_STATE::CHARGE:

		// 魔法のチャージ処理を行う
		UpdateCharge();

		// 魔法のエフェクトの更新を行う
		UpdateEffectPos();

		break;
	case MAGIC_STATE::SHOT:

		// 魔法の移動処理を行う
		UpdateShot();

		break;
	case MAGIC_STATE::BLAST:

		// 魔法の爆発処理を行う
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
#ifdef _DEBUG

	if (!magic_.isDraw_)
	{
		// 描画していなければ処理中断
		return;
	}

	// デバッグ用：衝突判定用球体
	DrawSphere3D(magic_.pos_, magic_.collisionRadius_, 10, 0x0000ff, 0x0000ff, false);

#endif // _DEBUG

}

void MagicBase::Release(void)
{
	// エフェクト停止
	StopEffekseer3DEffect(magic_.effectPlayId_);
}

void MagicBase::ChangeState(MAGIC_STATE state)
{
	// エフェクト停止
	StopEffekseer3DEffect(magic_.effectPlayId_);

	state_ = state;

	// 状態に応じて処理を変更
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

	// エフェクトの位置の更新
	SetPosPlayingEffekseer3DEffect(
		magic_.effectPlayId_, magic_.pos_.x, magic_.pos_.y, magic_.pos_.z);
}

void MagicBase::UpdateEffectPos(VECTOR pos)
{
	// 座標を更新する
	magic_.pos_ = pos;
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

	// 再生中のエフェクトの角度を設定
	SetRotationPlayingEffekseer3DEffect(
		magic_.effectPlayId_, -angle.x, angle.y, angle.z);
}

void MagicBase::UpdateCharge(void)
{
	// 魔法を徐々に大きくする(チャージする)
	if (isMaxCharge_)
	{
		// 指定のエフェクトが表示されていなかったら再生
		if (IsEffekseer3DEffectPlaying(magic_.effectPlayId_) == -1)
		{
			// チャージ状態のエフェクト再生
			// 最大チャージ状態のエフェクト再生
			magic_.effectPlayId_ = EffectResManager::GetInstance().PlayEffect(
				magic_.effectScale_, magic_.dir_, magic_.pos_, EffectResManager::TYPE::PLAYER_MAGIC_CHARGE_MAX);
		}
	}
	else
	{
		// 魔法をチャージしていたら当たり判定用半径も大きくしておく
		magic_.collisionRadius_ += chargePow_;

		// 当たり判定用半径が指定より大きくならないようにする
		if (magic_.collisionRadius_ > chargeMax_)
		{
			// 超えていたら指定値を代入
			magic_.collisionRadius_ = chargeMax_;

			// 最大になったことをフラグで知らせる
			isMaxCharge_ = true;

			// 他のエフェクトが表示されていたら停止する
			if (IsEffekseer3DEffectPlaying(magic_.effectPlayId_) != -1)
			{
				StopEffekseer3DEffect(magic_.effectPlayId_);
			}

			// 最大チャージ状態のエフェクト再生
			magic_.effectPlayId_ = EffectResManager::GetInstance().PlayEffect(
				magic_.effectScale_, magic_.dir_, magic_.pos_, EffectResManager::TYPE::PLAYER_MAGIC_CHARGE_MAX);
		}
	}
}

void MagicBase::UpdateShot(void)
{
	// 移動前の座標を取得しておく
	magic_.prevPos_ = magic_.pos_;

	// 魔法を移動させる
	magic_.pos_ = VAdd(magic_.pos_, VScale(magic_.dir_, magic_.speed_));
	
	// エフェクトの位置の更新
	SetPosPlayingEffekseer3DEffect(
		magic_.effectPlayId_, magic_.pos_.x, magic_.pos_.y, magic_.pos_.z);

	// 生存カウンタの減少
	ReduceCntAlive();
}

void MagicBase::UpdateBlast(void)
{
	// エフェクトを再生し終えていたら
	if (IsEffekseer3DEffectPlaying(magic_.effectPlayId_) == -1)
	{
		// END状態へ変更
		ChangeState(MAGIC_STATE::END);
	}
}

void MagicBase::ChangeEnd(void)
{
	// 存在フラグを折る
	magic_.isExists_ = false;
	// 描画フラグを折る
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

#include <DxLib.h>
#include "../../../Manager/SystemManager.h"
#include "../../../Scene/SceneManager.h"
#include "../../../Application.h"
#include "../Player.h"
#include "../../Magic/StraightMagic/StraightMagic.h"
#include "../../Magic/ChaseMagic/ChaseMagic.h"
#include "../../Magic/ExplosionMagic/ExplosionMagic.h"
#include "../../../Utility/AsoUtility.h"
#include "../../../Utility/MatrixUtility.h"
#include "WeaponBase.h"

WeaponBase::WeaponBase(Player* player)
{
	player_ = player;
	modelId_ = -1;
	magic_ = nullptr;
}

WeaponBase::~WeaponBase(void)
{
}

void WeaponBase::Init(void)
{
	// 魔法の種類を取得
	typeMagic_ = SystemManager::GetInstance().GetTypeMagic();

	// パラメータ設定
	SetParam();

	// 大きさを設定する
	MV1SetScale(modelId_, scales_);

	// アングルを設定する
	MV1SetRotationXYZ(modelId_, rotate_);

	// 杖の座標設定
	pos_ = player_->GetCameraPos();
	pos_ = VAdd(pos_, RELATIVE_POS_STICK);

	// 座標を設定する
	MV1SetPosition(modelId_, pos_);

	// 待ち状態に初期化
	state_ = STATE::IDLE;

	// 魔法発射の硬直時間
	pitch_ = 0.0f;
	pitchAngle_ = 0.0f;

	// 強制的に視点を動かすか
	isRecoil_ = false;

	// 魔法の座標
	magicPos_ = targetPos_ = AsoUtility::VECTOR_ZERO;

}

void WeaponBase::Update(void)
{
	// 魔法の種類変更が行われていたら変更する
	CheckMagicType();

	// 杖、魔法の座標計算
	UpdatePos();

	// 魔法の更新
	UpdateMagic();

	switch (state_)
	{
	case WeaponBase::STATE::IDLE:
		IdleUpdate();
		break;
	case WeaponBase::STATE::ATTACK:
		AttackUpdate();
		break;
	case WeaponBase::STATE::CREATE_MAGIC:
		GenerateMagicUpdate();
		break;
	case WeaponBase::STATE::CHARGE_MAGIC:
		ChargeMagicUpdate();
		break;
	case WeaponBase::STATE::WAIT:
		WaitUpdate();
		break;
	default:
		break;
	}

}

void WeaponBase::Draw(void)
{
	if (modelId_ != -1)
	{
		// モデルを何か読み込んでいたら描画させる
		MV1DrawModel(modelId_);
	}

	// 魔法の描画
	DrawMagic();

#ifdef _DEBUG

	// デバッグ用描画
	DebugDraw();

#endif // _DEBUG

}

void WeaponBase::Release(void)
{
	// モデルを何か読み込んでいたら解放させる
	MV1DeleteModel(modelId_);

	// 魔法解放
	for (MagicBase* Magic : magics_)
	{
		Magic->Release();
		delete Magic;
	}

	magics_.clear(); // 魔法のポインターをすべて削除
}

void WeaponBase::ChangeState(STATE state)
{
	state_ = state;
}

void WeaponBase::IdleUpdate(void)
{
}

void WeaponBase::GenerateMagicUpdate(void)
{
	// 有効な魔法を取得する
	magic_ = GetValidMagic();
	// 初期化処理
	magic_->Init();
	// 座標を更新する
	magic_->ChargeShot(magicPos_,magic_->GetMagic().dir_);

	// 状態をチャージ状態変更
	ChangeState(STATE::CHARGE_MAGIC);
}

void WeaponBase::ChargeMagicUpdate(void)
{
}

void WeaponBase::AttackUpdate(void)
{

#pragma region 方向

	// 狙う場所から魔法の発射位置へのベクトルを計算
	VECTOR vec = VSub(targetPos_, magicPos_);

	// ベクトルを正規化し、魔法の方向とする
	VECTOR dir = VNorm(vec);

#pragma endregion

	// 中身がnullptrだったら処理を行わない
	if (magic_ == nullptr)
	{
		return;
	}

	// 魔法生成
	magic_->CreateShot(magicPos_, dir);
	// 放ったら中身を消す
	magic_ = nullptr;

	// 魔法発射後の反動を計算
	pitch_ = player_->GetPlayerStatus().angle_.x;
	pitchAngle_ = pitch_ - recoil;

	// 上を向きすぎないように制限をかける
	if (pitchAngle_ < Player::MIN_VIEW_ANGLE)
	{
		pitchAngle_ = Player::MIN_VIEW_ANGLE;
	}

	// リコイルフラグを立てる
	isRecoil_ = true;

	// 状態を硬直中へ変更
	ChangeState(STATE::WAIT);
}

void WeaponBase::WaitUpdate(void)
{
	// リコイルフラグが立っていれば
	if (isRecoil_)
	{
		// 魔法発射後の反動を設定
		player_->SetPitch(pitchAngle_);
	}

	// 角度を元の位置に戻す
	if (pitchAngle_ < pitch_) {
		pitchAngle_ += RECOVERY_SPEED;

		// 元の位置より大きくならないようにする
		if (pitchAngle_ > pitch_) {
			pitchAngle_ = pitch_;
		}
	}
	else
	{
		// 元の位置まで戻ったらIDLE状態へ戻す
		ChangeState(STATE::IDLE);
	}
}

void WeaponBase::UpdateMagic(void)
{
	// 魔法の更新
	for (auto& Magic : magics_)
	{
		Magic->Update();
	}
}

void WeaponBase::DrawMagic(void)
{
	// 魔法の描画
	for (auto& Magic : magics_)
	{
		// 描画フラグが立っていれば
		if (Magic->GetMagic().isDraw_)
		{
			// 描画する
			Magic->Draw();
		}
	}
}

MagicBase* WeaponBase::GetValidMagic(void)
{
	size_t size = magics_.size();

	for (int i = 0; i < size; i++)
	{
		// 未使用(生存していない)で、かつ、魔法の種別が同じ
		if (!magics_[i]->GetMagic().isExists_ && magics_[i]->GetTypeMagic() == typeMagic_)
		{
			return magics_[i];
		}
	}

	// 未使用の魔法がなかった場合新しい魔法を生成
	MagicBase* Magic;

	switch (typeMagic_)
	{
	case TYPE_MAGIC::STRAIGHT_MAGIC:

		// 直線魔法のインスタンスを生成する
		Magic = new StraightMagic(typeMagic_, &magicPos_);

		break;
	case TYPE_MAGIC::CHASE_MAGIC:

		// 追跡魔法のインスタンスを生成する
		Magic = new ChaseMagic(typeMagic_, &magicPos_);

		break;
	case TYPE_MAGIC::EXPLOSION_MAGIC:

		// 爆発魔法のインスタンスを生成する
		Magic = new ExplosionMagic(typeMagic_, &magicPos_);

		break;
	default:
		break;
	}

	// 可変長配列に追加
	magics_.push_back(Magic);

	return Magic;
}

void WeaponBase::UpdatePos(void)
{
	// 視点座標を取得
	VECTOR playerCameraPos = player_->GetCameraPos();
	// プレイヤーの向きを取得
	VECTOR playerAngle = player_->GetPlayerStatus().angle_;

	// 砲身の回転行列
	VECTOR vec = { playerAngle.x ,playerAngle.y ,0.0f };
	MATRIX matRot = MatrixUtility::GetMatrixRotateXYZ(vec);

	// カメラの視線方向のベクトルを計算
	// DxlibのVTransformを使用
	VECTOR forward = VGet(0.0f, 0.0f, 1.0f); // 前方向をZ軸とする
	// カメラの方向を算出
	VECTOR cameraDir = VTransform(forward, matRot);

#pragma region 杖

	// 座標
	// 方向と同じ要領で、相対座標を回転
	VECTOR localPosRot = VTransform(RELATIVE_POS_STICK, matRot);

	// 杖の座標に反映
	pos_ = VAdd(playerCameraPos, localPosRot);

	// モデルに座標を反映
	MV1SetPosition(modelId_, pos_);

	// 回転
	// 杖の回転を行列にする
	MATRIX weaponMat = MatrixUtility::GetMatrixRotateXYZ(rotate_);

	// プレイヤーの回転を杖のの回転行列に反映する
	MATRIX mat = MatrixUtility::Multiplication(weaponMat, matRot);
	// 回転行列をモデルに反映
	MV1SetRotationMatrix(modelId_, mat);

#pragma endregion

#pragma region 杖口

	// 方向と同じ要領で、相対座標を回転
	localPosRot = VTransform(RELATIVE_POS_MAGIC, matRot);

	// 魔法の座標に反映
	magicPos_ = VAdd(pos_, localPosRot);

#pragma endregion

#pragma region 狙う場所

	// カメラから遠く離れた点をターゲットとする
	// カーソルの場所はカメラの視線方向と一致すると仮定
	targetPos_ = VAdd(playerCameraPos, VScale(cameraDir, RELATIVE_POS_TARGET));

	// ターゲットをわずかに上へ補正
	targetPos_.y += TARGET_POS_OFFSET;
#pragma endregion
}

void WeaponBase::CheckMagicType(void)
{
	// 今現在の魔法の種類を確認し、種類が一緒でなかったら
	if (typeMagic_ != SystemManager::GetInstance().GetTypeMagic())
	{
		// 種類を合わせる
		typeMagic_ = SystemManager::GetInstance().GetTypeMagic();
	}
}

void WeaponBase::DebugDraw(void)
{
	// 杖の位置仮表示
	DrawSphere3D(pos_, 15.0f, 10, 0xff0000, 0xff0000, false);

	// 魔法の発射位置目安
	DrawSphere3D(magicPos_, 3.0f, 10, 0x00ff00, 0x00ff00, true);

	// 狙う場所の位置目安
	DrawSphere3D(targetPos_, 10.0f, 10, 0x00ff00, 0x0000ff, true);
}


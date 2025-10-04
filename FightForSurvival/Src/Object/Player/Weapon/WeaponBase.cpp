#include <DxLib.h>
#include "../../../Manager/SystemManager.h"
#include "../../../Manager/SceneManager.h"
#include "../../../Application.h"
#include "../Player.h"
#include "Magic/FireMagic.h"
#include "../../../Utility/AsoUtility.h"
#include "WeaponBase.h"

WeaponBase::WeaponBase(Player* player)
{
	player_ = player;
	modelId_ = -1;
	magicModelId_ = -1;
}

WeaponBase::~WeaponBase(void)
{
}

void WeaponBase::Init(void)
{
	// 画像やモデルなどのロード
	Load();

	//// 魔法のモデルを読み込む
	//magicModelId_ = MV1LoadModel(
	//	(Application::PATH_MODEL + "Cannon/Barrel.mv1").c_str());

	//if (magicModelId_ == -1)
	//{
	//	// モデルの読み込みに失敗した場合の処理
	//	DrawString(0, 0, "CannonBarrel model load failed", GetColor(255, 0, 0));
	//}

	// パラメータ設定
	SetParam();
	
	MV1SetScale(modelId_, scales_);
	MV1SetRotationXYZ(modelId_, rotate_);

	// 杖の座標設定
	pos_ = player_->GetCameraPos();
	pos_ = VAdd(pos_, RELATIVE_POS_GUN);

	MV1SetPosition(modelId_, pos_);

	state_ = STATE::IDLE;

	// 魔法発射の硬直時間
	pitch_ = 0.0f;
	pitchAngle_ = 0.0f;

	// 強制的に視点を動かすか
	isRecoil_ = false;

	// 魔法の座標
	magicPos_ = targetPos_ = AsoUtility::VECTOR_ZERO;

	// リロード時間
	reloadTime_ = 0.0f;
}

void WeaponBase::Update(void)
{
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
	case WeaponBase::STATE::WAIT:
		WaitUpdate();
		break;
	case WeaponBase::STATE::RELOAD:
		ReloadUpdate();
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

	if (state_ == STATE::RELOAD)
	{
		int posX = Application::SCREEN_SIZE_X / 2;
		int posY = Application::SCREEN_SIZE_Y / 2;

		// 背景の枠
		DrawBox(posX - 50, posY - 30,
			posX + 50, posY - 40, 0x696969, true);
		// プログレスバー本体
		DrawBox(posX - 50, posY - 30,
			posX - 50 + static_cast<int>((50 * reloadTime_)), posY - 40, 0xff7f50, true);
		DrawString(posX - 70, posY - 60, "ポーション使用中", 0xffffff);
	}

	DrawFormatString(Application::SCREEN_SIZE_X - 380, Application::SCREEN_SIZE_Y - 25,
		0xffffff, "攻撃可能回数：%d　/　残りのMPポーション：%d", magicNum_, MPPotionNum_);

#ifdef _DEBUG

	// 杖の位置仮表示
	DrawSphere3D(pos_, 15.0f, 10, 0xff0000, 0xff0000, false);

	// 魔法の発射位置目安
	DrawSphere3D(magicPos_, 3.0f, 10, 0x00ff00, 0x00ff00, true);

	// 狙う場所の位置目安
	//DrawSphere3D(targetPos_, 10.0f, 10, 0x00ff00, 0x0000ff, true);


#endif // _DEBUG

}

void WeaponBase::Release(void)
{
	if (modelId_ != -1)
	{
		// モデルを何か読み込んでいたら解放させる
		MV1DeleteModel(modelId_);
	}

	if (magicModelId_ != -1)
	{
		// モデルを何か読み込んでいたら解放させる
		MV1DeleteModel(magicModelId_);
	}

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

void WeaponBase::AttackUpdate(void)
{

#pragma region 方向

	VECTOR dir;

	// 狙う場所から魔法の発射位置へのベクトルを計算
	VECTOR vec = VSub(targetPos_, magicPos_);

	// ベクトルを正規化し、魔法の方向とする
	dir = VNorm(vec);

#pragma endregion

	// 有効な魔法を取得する
	MagicBase* Magic = GetValidMagic();
	// 魔法を生成(方向は仮で正面方向)
	Magic->CreateShot(magicPos_, dir);

	// 魔法発射後の反動を計算
	pitch_ = player_->GetPitch();
	pitchAngle_ = pitch_ - recoil;
	// 上を向きすぎないように制限をかける
	if (pitchAngle_ < Player::MIN_VIEW_ANGLE)
	{
		pitchAngle_ = Player::MIN_VIEW_ANGLE;
	}
	isRecoil_ = true;
	player_->SetPitch(pitchAngle_);

	magicNum_--;

	ChangeState(STATE::WAIT);
}

void WeaponBase::WaitUpdate(void)
{

	if (isRecoil_)
	{
		// 魔法発射後の反動を設定
		player_->SetPitch(pitchAngle_);
	}

	// 角度を元の位置に戻す
	if (pitchAngle_ < pitch_) {
		pitchAngle_ += RECOVERY_SPEED;
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

void WeaponBase::ReloadUpdate(void)
{
	if (MPPotionNum_ <= 0 || magicNum_ == magicCapacity_)
	{
		// 残りのMPポーションが無いか、攻撃可能回数が最大だったら処理を行わない
		ChangeState(STATE::IDLE);
		return;
	}

	// リロード時間を進める
	reloadTime_ += SceneManager::GetInstance().GetDeltaTime();

	// リロード時間が既定の時間経ったらIDLE状態へ戻す
	if (reloadTime_ >= RELOAD_TIME)
	{
		reloadTime_ = 0.0f;

		// 残りのMPポーションを減らす
		MPPotionNum_--;
		// 攻撃可能回数を増やす
		magicNum_ = magicCapacity_;

		ChangeState(STATE::IDLE);
	}
}

void WeaponBase::UpdateMagic(void)
{
	// 魔法の更新
	for (auto& Magic : magics_)
	{
		if (Magic->GetMagic().isAlive_)
		{
			Magic->Update();
		}
	}
}

void WeaponBase::DrawMagic(void)
{
	// 魔法の更新
	for (auto& Magic : magics_)
	{
		if (Magic->GetMagic().isAlive_)
		{
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
		if (!magics_[i]->GetMagic().isAlive_)
		{
			return magics_[i];
		}
	}

	// 未使用の魔法がなかった場合新しい魔法を生成
	MagicBase* Magic;

	// 新しい魔法のインスタンスを生成する
	Magic = new FireMagic(magicModelId_);

	// 可変長配列に追加
	magics_.push_back(Magic);

	return Magic;
}

void WeaponBase::UpdatePos(void)
{
	VECTOR playerCameraPos = player_->GetCameraPos();

	// 砲身の回転行列
	MATRIX matRot = MGetIdent();
	matRot = MMult(matRot, MGetRotX(player_->GetPitch()));
	matRot = MMult(matRot, MGetRotY(player_->GetYaw()));

#pragma region 杖

	// 方向と同じ要領で、相対座標を回転
	VECTOR localPosRot = VTransform(RELATIVE_POS_GUN, matRot);

	pos_ = VAdd(playerCameraPos, localPosRot);

	MV1SetPosition(modelId_, pos_);

#pragma endregion

#pragma region 杖口

	// 方向と同じ要領で、相対座標を回転
	localPosRot = VTransform(RELATIVE_POS_MAGIC, matRot);

	magicPos_ = VAdd(pos_, localPosRot);


#pragma endregion

#pragma region 狙う場所

	// カメラの視線方向のベクトルを計算
	// DxlibのVTransformを使用
	VECTOR forward = VGet(0.0f, 0.0f, 1.0f); // 前方向をZ軸とする
	VECTOR cameraDir = VTransform(forward, matRot);

	// カメラから遠く離れた点をターゲットとする
	// カーソルの場所はカメラの視線方向と一致すると仮定
	targetPos_ = VAdd(playerCameraPos, VScale(cameraDir, RELATIVE_POS_TARGET));

	// ターゲットをわずかに上へ補正
	targetPos_.y += 0.8f; 
#pragma endregion
}


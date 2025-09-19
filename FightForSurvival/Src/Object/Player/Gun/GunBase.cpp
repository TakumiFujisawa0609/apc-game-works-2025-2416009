#include <DxLib.h>
#include "../../../Manager/SystemManager.h"
#include "../../../Manager/SceneManager.h"
#include "../../../Application.h"
#include "../Player.h"
#include "Bullet/HundgunBullet.h"
#include "../../../Utility/AsoUtility.h"
#include "GunBase.h"

GunBase::GunBase(Player* player)
{
	player_ = player;
	modelId_ = -1;
	bulletModelId_ = -1;
}

GunBase::~GunBase(void)
{
}

void GunBase::Init(void)
{
	// 画像やモデルなどのロード
	Load();

	//// 弾のモデルを読み込む
	//bulletModelId_ = MV1LoadModel(
	//	(Application::PATH_MODEL + "Cannon/Barrel.mv1").c_str());

	//if (bulletModelId_ == -1)
	//{
	//	// モデルの読み込みに失敗した場合の処理
	//	DrawString(0, 0, "CannonBarrel model load failed", GetColor(255, 0, 0));
	//}

	// パラメータ設定
	SetParam();
	
	MV1SetScale(modelId_, gunScales_);
	MV1SetRotationXYZ(modelId_, gunRotate_);

	// 銃の座標設定
	gunPos_ = player_->GetCameraPos();
	gunPos_ = VAdd(gunPos_, RELATIVE_POS_GUN);

	MV1SetPosition(modelId_, gunPos_);

	state_ = STATE::IDLE;

	// 弾発射の硬直時間
	pitch_ = 0.0f;
	pitchAngle_ = 0.0f;

	// 強制的に視点を動かすか
	isRecoil_ = false;

	// 弾の座標
	bulletPos_ = targetPos_ = AsoUtility::VECTOR_ZERO;

	// リロード時間
	reloadTime_ = 0.0f;
}

void GunBase::Update(void)
{
	// 銃、弾の座標計算
	UpdatePos();

	// 弾の更新
	UpdateBullet();

	switch (state_)
	{
	case GunBase::STATE::IDLE:
		IdleUpdate();
		break;
	case GunBase::STATE::ATTACK:
		AttackUpdate();
		break;
	case GunBase::STATE::WAIT:
		WaitUpdate();
		break;
	case GunBase::STATE::RELOAD:
		ReloadUpdate();
		break;
	default:
		break;
	}

}

void GunBase::Draw(void)
{
	if (modelId_ != -1)
	{
		// モデルを何か読み込んでいたら描画させる
		MV1DrawModel(modelId_);
	}

	// 弾の描画
	DrawBullet();

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
		DrawString(posX - 45, posY - 60, "リロード中", 0xffffff);
	}

	DrawFormatString(Application::SCREEN_SIZE_X - 260, Application::SCREEN_SIZE_Y - 25,
		0xffffff, "装填数：%d　/　残りの弾数：%d", bulletNum_, bulletNumMax_);

#ifdef _DEBUG

	// 銃の位置仮表示
	DrawSphere3D(gunPos_, 15.0f, 10, 0xff0000, 0xff0000, false);

	// 弾の発射位置目安
	DrawSphere3D(bulletPos_, 3.0f, 10, 0x00ff00, 0x00ff00, true);

	// 狙う場所の位置目安
	//DrawSphere3D(targetPos_, 10.0f, 10, 0x00ff00, 0x0000ff, true);


#endif // _DEBUG

}

void GunBase::Release(void)
{
	if (modelId_ != -1)
	{
		// モデルを何か読み込んでいたら解放させる
		MV1DeleteModel(modelId_);
	}

	if (bulletModelId_ != -1)
	{
		// モデルを何か読み込んでいたら解放させる
		MV1DeleteModel(bulletModelId_);
	}

	for (BulletBase* bullet : bullets_)
	{
		bullet->Release();
		delete bullet;
	}

	bullets_.clear(); // 弾のポインターをすべて削除
}

void GunBase::ChangeState(STATE state)
{
	state_ = state;
}

void GunBase::IdleUpdate(void)
{
}

void GunBase::AttackUpdate(void)
{

#pragma region 方向

	VECTOR dir;

	// 狙う場所から弾の発射位置へのベクトルを計算
	VECTOR vec = VSub(targetPos_, bulletPos_);

	// ベクトルを正規化し、弾の方向とする
	dir = VNorm(vec);

#pragma endregion

	// 有効な弾を取得する
	BulletBase* bullet = GetValidBullet();
	// 弾を生成(方向は仮で正面方向)
	bullet->CreateShot(bulletPos_, dir);

	// 弾発射後の反動を計算
	pitch_ = player_->GetPitch();
	pitchAngle_ = pitch_ - recoil;
	// 上を向きすぎないように制限をかける
	if (pitchAngle_ < Player::MIN_VIEW_ANGLE)
	{
		pitchAngle_ = Player::MIN_VIEW_ANGLE;
	}
	isRecoil_ = true;
	player_->SetPitch(pitchAngle_);

	bulletNum_--;

	ChangeState(STATE::WAIT);
}

void GunBase::WaitUpdate(void)
{

	if (isRecoil_)
	{
		// 弾発射後の反動を設定
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

void GunBase::ReloadUpdate(void)
{
	if (bulletNumMax_ <= 0 || bulletNum_ == bulletCapacity_)
	{
		// 残りの弾数が無いか、装填数が最大だったら処理を行わない
		ChangeState(STATE::IDLE);
		return;
	}

	// リロード時間を進める
	reloadTime_ += SceneManager::GetInstance().GetDeltaTime();

	// リロード時間が既定の時間経ったらIDLE状態へ戻す
	if (reloadTime_ >= RELOAD_TIME)
	{
		reloadTime_ = 0.0f;

		while (bulletNum_ < bulletCapacity_)
		{
			if (bulletNumMax_ <= 0)
			{
				// 残りの弾数が無かったら処理を抜ける
				break;
			}

			// 残りの弾数を減らす
			bulletNumMax_--;
			// 装填数を増やす
			bulletNum_++;
		}

		ChangeState(STATE::IDLE);
	}
}

void GunBase::UpdateBullet(void)
{
	// 弾の更新
	for (auto& bullet : bullets_)
	{
		if (bullet->GetBullet().isAlive_)
		{
			bullet->Update();
		}
	}
}

void GunBase::DrawBullet(void)
{
	// 弾の更新
	for (auto& bullet : bullets_)
	{
		if (bullet->GetBullet().isAlive_)
		{
			bullet->Draw();
		}
	}
}

BulletBase* GunBase::GetValidBullet(void)
{
	size_t size = bullets_.size();

	for (int i = 0; i < size; i++)
	{
		// 未使用(生存していない)で、かつ、弾の種別が同じ
		if (!bullets_[i]->GetBullet().isAlive_)
		{
			return bullets_[i];
		}
	}

	// 未使用の弾がなかった場合新しい弾を生成
	BulletBase* bullet;

	// 新しい弾のインスタンスを生成する
	bullet = new HundgunBullet(bulletModelId_);

	// 可変長配列に追加
	bullets_.push_back(bullet);

	return bullet;
}

void GunBase::UpdatePos(void)
{
	VECTOR playerCameraPos = player_->GetCameraPos();

	// 砲身の回転行列
	MATRIX matRot = MGetIdent();
	matRot = MMult(matRot, MGetRotX(player_->GetPitch()));
	matRot = MMult(matRot, MGetRotY(player_->GetYaw()));

#pragma region 銃

	// 方向と同じ要領で、相対座標を回転
	VECTOR localPosRot = VTransform(RELATIVE_POS_GUN, matRot);

	gunPos_ = VAdd(playerCameraPos, localPosRot);

#pragma endregion

#pragma region 銃口

	// 方向と同じ要領で、相対座標を回転
	localPosRot = VTransform(RELATIVE_POS_BULLET, matRot);

	bulletPos_ = VAdd(gunPos_, localPosRot);


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


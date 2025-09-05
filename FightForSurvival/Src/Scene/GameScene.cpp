#include <DxLib.h>
#include "../Object/Grid/Grid.h"
#include "../Application.h"
#include "../Manager/Camera.h"
#include "../Manager/InputManager.h"
#include "../Object/Player/Player.h"
#include "../Object/Common/Cursor.h"
#include "../Object/Enemy/Zombie.h"
#include "../Object/Player/Gun/GunBase.h"
#include "../Object/Player/Gun/Bullet/BulletBase.h"
#include "../CollisionManager.h"
#include "GameScene.h"

GameScene::GameScene(void)
{
	grid_ = nullptr;
	player_ = nullptr;
	camera_ = nullptr;
	cursor_ = nullptr;
	enemy_ = nullptr;

	enemyId_ = -1;

	// マウスカーソルを表示しない
	SetMouseDispFlag(false);
}

GameScene::~GameScene(void)
{
}

void GameScene::Load(void)
{
	player_->Load();
	cursor_->Load();
}

void GameScene::Init(void)
{

	// グリッド生成
	grid_ = new Grid();
	grid_->Init();

	// プレイヤー生成
	player_ = new Player();
	player_->Init();

	// カメラの生成
	camera_ = new Camera(player_);
	camera_->Init();

	// カーソルの生成
	cursor_ = new Cursor();
	cursor_->Init();

	// 敵の生成
	enemyId_ = MV1LoadModel((Application::PATH_MODEL + "Enemy/Zombie.mv1").c_str());
	enemy_ = new Zombie();
	enemy_->Init(EnemyBase::ENEMY_TYPE::ZOMBIE, enemyId_, -1, VGet(0.0f, 0.0f, 100.0f), player_);
}

void GameScene::Update(void)
{
	// グリッド更新
	grid_->Update();

	// プレイヤー更新
	player_->Update();

	// カメラの更新
	camera_->Update();

	// 敵の更新
	enemy_->Update();

	// 当たり判定
	CheckCollisions();
}

void GameScene::Draw(void)
{
	// カメラの設定
	camera_->SetBeforeDraw();

	// グリッド描画
	grid_->Draw();

	// プレイヤー描画
	player_->Draw();

	// 敵の描画
	enemy_->Draw();

#ifdef _DEBUG
	DrawString(0, 0, "GameScene", 0xffffff);

	// カメラのデバック描画
	camera_->DrawDebug();
#endif // _DEBUG

	// カーソルの描画
	cursor_->Draw();

}

void GameScene::Release(void)
{

	// 敵の解放
	if (enemy_ != nullptr)
	{
		enemy_->Release();
		delete enemy_;
	}
	MV1DeleteModel(enemyId_);

	// カーソルの解放
	if (cursor_ != nullptr)
	{
		cursor_->Release();
		delete cursor_;
	}

	// カメラの解放
	if (camera_ != nullptr)
	{
		camera_->Release();
		delete camera_;
	}

	// プレイヤーの解放
	if (player_ != nullptr)
	{
		player_->Release();
		delete player_;
		player_ = nullptr;
	}

	// グリッドの解放
	if (grid_ != nullptr)
	{
		grid_->Release();
		delete grid_;
		grid_ = nullptr;
	}

	// マウスカーソルを表示させる
	SetMouseDispFlag(true);
}

void GameScene::CheckCollisions(void)
{
	if (!enemy_->IsCollisionState())
	{
		// 敵が生存していなければ処理を行わない
		return;
	}

	// 敵の情報
	Unit eneInfo = enemy_->GetEnemy();
	CollisionPos eneColPosInfo = enemy_->GetColPos();

	// 敵の座標
	// 頭
	VECTOR enePosHead = eneColPosInfo.posHead_;
	// 体
	VECTOR enePosBodyTop = eneColPosInfo.posBodyTop_;
	VECTOR enePosBodyUnder = eneColPosInfo.posBodyUnder_;
	// 右腕
	VECTOR enePosArmTopR = eneColPosInfo.posArmTopR_;
	VECTOR enePosArmUnderR = eneColPosInfo.posArmUnderR_;
	// 左腕
	VECTOR enePosArmTopL = eneColPosInfo.posArmTopL_;
	VECTOR enePosArmUnderL = eneColPosInfo.posArmUnderL_;
	// 右手
	VECTOR enePosHandR = eneColPosInfo.posHandR_;
	// 左手
	VECTOR enePosHandL = eneColPosInfo.posHandL_;
	// 右脚
	VECTOR enePosLegTopR = eneColPosInfo.posLegTopR_;
	VECTOR enePosLegUnderR = eneColPosInfo.posLegUnderR_;
	// 左脚
	VECTOR enePosLegTopL = eneColPosInfo.posLegTopL_;
	VECTOR enePosLegUnderL = eneColPosInfo.posLegUnderL_;

	// 敵の半径
	float eneRadHead = eneInfo.collisionRadius_;
	float eneRadBody = eneInfo.collisionRadiusBody_;
	float eneRadArm = eneInfo.collisionRadiusArm_;
	float eneRadHand = eneInfo.collisionRadiusHand_;
	float eneRadLeg = eneInfo.collisionRadiusLeg_;

	// 弾クラスのポインター取得
	auto bullets = player_->GetGun()->GetBullets();

	// 弾の数分回す
	for (auto bullet : bullets)
	{
		// 弾が生存していなかったら次の弾に進む
		if (!bullet->IsCollisionState())
		{
			continue;
		}

		// 弾の情報
		auto bulletInfo = bullet->GetBullet();

		// 弾の移動経路の線分を定義
		VECTOR bulletLineStart = bulletInfo.pos_;
		VECTOR bulletLineEnd = bulletInfo.prevPos_; // 前のフレームでの弾の位置

		// 弾の半径
		float bulletRad = bulletInfo.collisionRadius_;

		// 頭の当たり判定
		if (CollisionManager::IsCollidingSphereAndSphere(enePosHead, eneRadHead, bulletLineStart, bulletLineEnd, bulletRad))
		{
			// 敵にダメージを与える
			enemy_->SubHp(bulletInfo.headDamage_);
			// 弾を爆発させる
			bullet->ChangeState(BulletBase::STATE::BLAST);
		}
		// 体の当たり判定
		else if (CollisionManager::IsCollidingCapsuleSphere(enePosBodyTop, enePosBodyUnder, eneRadBody, bulletLineStart, bulletLineEnd, bulletRad)
			|| CollisionManager::IsCollidingCapsuleSphere(enePosArmTopR, enePosArmUnderR, eneRadArm, bulletLineStart, bulletLineEnd, bulletRad)
			|| CollisionManager::IsCollidingCapsuleSphere(enePosArmTopL, enePosArmUnderL, eneRadArm, bulletLineStart, bulletLineEnd, bulletRad)
			|| CollisionManager::IsCollidingSphereAndSphere(enePosHandR, eneRadHand, bulletLineStart, bulletLineEnd, bulletRad)
			|| CollisionManager::IsCollidingSphereAndSphere(enePosHandL, eneRadHand, bulletLineStart, bulletLineEnd, bulletRad)
			|| CollisionManager::IsCollidingCapsuleSphere(enePosLegTopR, enePosLegUnderR, eneRadLeg, bulletLineStart, bulletLineEnd, bulletRad)
			|| CollisionManager::IsCollidingCapsuleSphere(enePosLegTopL, enePosLegUnderL, eneRadLeg, bulletLineStart, bulletLineEnd, bulletRad))
		{
			// 敵にダメージを与える
			enemy_->SubHp(bulletInfo.bodyDamage_);
			// 弾を爆発させる
			bullet->ChangeState(BulletBase::STATE::BLAST);
		}
	}

}

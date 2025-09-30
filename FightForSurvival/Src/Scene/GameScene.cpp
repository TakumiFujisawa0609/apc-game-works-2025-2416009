#include "GameScene.h"

#include <DxLib.h>

#include "../Application.h"
#include "../Manager/Camera.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/SystemManager.h"

#include "../Object/Grid/Grid.h"
#include "../Object/Player/Player.h"
#include "../Object/Player/Gun/GunBase.h"
#include "../Object/Player/Gun/Bullet/BulletBase.h"
#include "../Object/Common/Cursor.h"
#include "../Common/Score/Score.h"
#include "../Object/Enemy/EnemyManager.h"
#include "../Object/Enemy/Zombie.h"
#include "../Manager/CollisionManager.h"
#include "../Common/Pause/Pause.h"

#include "../WaveSystem/WaveManager.h"
#include "../WaveSystem/Wave/Wave1.h"
//#include "../WaveSystem/Wave/WaveFinal.h"

GameScene::GameScene(void)
{
	grid_ = nullptr;
	player_ = nullptr;
	camera_ = nullptr;
	cursor_ = nullptr;
	score_ = nullptr;
	pause_ = nullptr;

	// マウスカーソルを表示しない
	SetMouseDispFlag(false);
}

GameScene::~GameScene(void)
{
}

void GameScene::Init(void)
{
	// スコアの初期化
	SystemManager::GetInstance().ResetGame();

	// グリッド生成
	grid_ = new Grid();
	grid_->Init();

	// プレイヤー生成
	player_ = new Player();
	player_->Init();

	// 敵マネージャの生成
	EnemyManager::CreateInstance();
	EnemyManager::GetInstance().Load();
	EnemyManager::GetInstance().GetPlayyerPoint(player_);

	// カメラの生成
	camera_ = new Camera(player_);
	camera_->Init();

	// カーソルの生成
	cursor_ = new Cursor();
	cursor_->Init();

	// スコアの生成
	score_ = new Score();
	score_->Init();

	// ポーズモードの生成
	pause_ = new Pause();
	pause_->Init();
}

void GameScene::Load(void)
{
	player_->Load();
	cursor_->Load();
	pause_->Load();

	WaveManager::CreateInstance();
	WaveManager::GetInstance().AddWave(std::make_unique<Wave1>());
	//WaveManager::GetInstance().AddWave(std::make_unique<WaveFinal>());
}

void GameScene::Update(void)
{
	// ポーズモード確認
	prevPause_ = nowPause_;
	nowPause_ = pause_->GetPauseMode();

	if (!prevPause_ && !nowPause_)
	{
		// グリッド更新
		grid_->Update();

		// プレイヤー更新
		player_->Update();

		// 敵の更新
		EnemyManager::GetInstance().Update();

		// カメラの更新
		camera_->Update();

		// ウェーブの更新
		WaveManager::GetInstance().Update();

		// 当たり判定
		CheckCollisions();

		// ゲームクリア・ゲームオーバー判定
		IsClear();
		IsOver();
	}
	else if (prevPause_ && !nowPause_)
	{
		//SoundManager::GetInstance()->Play(SoundManager::BGM::GAME, false);
	}

	// ポーズモード更新
	pause_->Update();

#ifdef _DEBUG

	auto& inputIns = InputManager::GetInstance();
	auto& sceneIns = SceneManager::GetInstance();

	if (inputIns.IsTrgDown(KEY_INPUT_C))
	{
		sceneIns.ChangeScene(SceneManager::SCENE_ID::CLEAR);
	}

	if (inputIns.IsTrgDown(KEY_INPUT_O))
	{
		sceneIns.ChangeScene(SceneManager::SCENE_ID::OVER);
	}

#endif // _DEBUG

}

void GameScene::Draw(void)
{
	// カメラの設定
	camera_->SetBeforeDraw();

	// グリッド描画
	grid_->Draw();

	// 敵の描画
	WaveManager::GetInstance().Draw();

	// 敵の描画
	EnemyManager::GetInstance().Draw();

	// プレイヤー描画
	player_->Draw();

#ifdef _DEBUG
	DrawString(0, 0, "GameScene", 0xffffff);

	// カメラのデバック描画
	camera_->DrawDebug();
#endif // _DEBUG

	// カーソルの描画
	cursor_->Draw();

	// スコアの描画
	score_->Draw();

	// ポーズモードの描画
	pause_->Draw();

}

void GameScene::Release(void)
{

	// ウェーブの解放
	WaveManager::GetInstance().DeleteInstance();

	// ポーズモードの解放
	if (pause_ != nullptr)
	{
		pause_->Release();
		delete pause_;
		pause_ = nullptr;
	}

	// スコアの解放
	if (score_ != nullptr)
	{
		score_->Release();
		delete score_;
		score_ = nullptr;
	}

	// カーソルの解放
	if (cursor_ != nullptr)
	{
		cursor_->Release();
		delete cursor_;
		cursor_ = nullptr;
	}

	// カメラの解放
	if (camera_ != nullptr)
	{
		camera_->Release();
		delete camera_;
		camera_ = nullptr;
	}

	// 敵の解放
	EnemyManager::GetInstance().Delete();
	EnemyManager::GetInstance().DeleteInstance();

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
	auto& eneManaIns = EnemyManager::GetInstance();
	// 生成してある敵を取得
	auto& enemys_ = eneManaIns.GetEnemy();

	for (auto* enemy : enemys_)
	{

		if (!enemy->IsCollisionState())
		{
			// 敵が生存していなければ処理を行わない
			continue;
		}

		// 敵の情報
		Unit eneInfo = enemy->GetEnemy();
		EnemyCollision eneColInfo = enemy->GetColPos();

		// 敵の座標
		VECTOR enePos[COLLISION_POS::MAX];

		for (int i = 0; i < static_cast<int>(COLLISION_POS::MAX); i++)
		{
			enePos[static_cast<COLLISION_POS>(i)] = eneColInfo.colPos_[static_cast<COLLISION_POS>(i)];
		}

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
			if (CollisionManager::IsCollidingSphereCapsule(enePos[HEAD], eneRadHead, bulletLineStart, bulletLineEnd, bulletRad))
			{
				// 敵にダメージを与える
				enemy->SubHp(bulletInfo.headDamage_);
				// 弾を爆発させる
				bullet->ChangeState(BulletBase::STATE::BLAST);
			}
			// 体、腕、手、脚の当たり判定
			else if (CollisionManager::IsCollidingCapsules(enePos[BODY_TOP], enePos[BODY_UNDER], eneRadBody, bulletLineStart, bulletLineEnd, bulletRad)
				|| CollisionManager::IsCollidingCapsules(enePos[ARM_TOP_R], enePos[ARM_UNDER_R], eneRadArm, bulletLineStart, bulletLineEnd, bulletRad)
				|| CollisionManager::IsCollidingCapsules(enePos[ARM_TOP_L], enePos[ARM_UNDER_L], eneRadArm, bulletLineStart, bulletLineEnd, bulletRad)
				|| CollisionManager::IsCollidingSphereCapsule(enePos[HAND_R], eneRadHand, bulletLineStart, bulletLineEnd, bulletRad)
				|| CollisionManager::IsCollidingSphereCapsule(enePos[HAND_L], eneRadHand, bulletLineStart, bulletLineEnd, bulletRad)
				|| CollisionManager::IsCollidingCapsules(enePos[LEG_TOP_R], enePos[LEG_UNDER_R], eneRadLeg, bulletLineStart, bulletLineEnd, bulletRad)
				|| CollisionManager::IsCollidingCapsules(enePos[LEG_TOP_L], enePos[LEG_UNDER_L], eneRadLeg, bulletLineStart, bulletLineEnd, bulletRad))
			{
				// 敵にダメージを与える
				enemy->SubHp(bulletInfo.bodyDamage_);
				// 弾を爆発させる
				bullet->ChangeState(BulletBase::STATE::BLAST);
			}
		}

		if (!enemy->IsAttack())
		{
			// 敵が攻撃状態ではなかったら抜ける
			continue;
		}

		VECTOR plaPos = player_->GetPlayer().pos_;
		float plaRad = player_->GetPlayer().collisionRadius_;

		// プレイヤーと敵の攻撃の当たり判定
		if (CollisionManager::IsCollidingSpheres(plaPos, plaRad, enePos[HAND_R], eneRadHand))
		{
			// プレイヤーにダメージを与える
			player_->SubHp(1);
			enemy->SetIsAttack(false);
		}

	}

}

void GameScene::IsClear(void)
{
	auto& eneManaIns = EnemyManager::GetInstance();

	auto& enemys_ = eneManaIns.GetEnemy();

	bool isEnd_ = true;

	for (auto& enemy : enemys_)
	{
		if (enemy->GetEnemy().isAlive_)
		{
			// 敵が一匹でも残っていたら終了しない
			isEnd_ = false;
			break;
		}
	}

	// WAVEが最終段階かつ、敵全てが死亡していたら
	if (WaveManager::GetInstance().AllCleared() || isEnd_)
	{
		// ゲームクリアに遷移
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::CLEAR);
	}
}

void GameScene::IsOver(void)
{
	// プレイヤーが死亡したら
	if (!player_->GetPlayer().isAlive_)
	{
		// ゲームオーバーに遷移
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::OVER);
	}
}

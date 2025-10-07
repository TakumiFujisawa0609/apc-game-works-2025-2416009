#include <DxLib.h>
#include "../Object/Grid/Grid.h"
#include "../Application.h"
#include "../Manager/Camera.h"
#include "../Manager/InputManager.h"
#include "../Object/Player/Player.h"
#include "../Object/Common/Cursor.h"
#include "../Common/Score/Score.h"
#include "../Object/Enemy/Zombie.h"
#include "../Object/Player/Weapon/WeaponBase.h"
#include "../Object/Player/Weapon/Magic/MagicBase.h"
#include "../Manager/CollisionManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/SystemManager.h"
#include "../Common/Pause/Pause.h"
#include "../Wave/WaveManager.h"
#include "../Wave/Wave1.h"
#include "../Wave/Wave2.h"
#include "../Wave/WaveFinal.h"
#include "../Object/Enemy/EnemyManager.h"
#include "GameScene.h"

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
	EnemyManager::GetInstance().GetPlayerPoint(player_);

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
	WaveManager::GetInstance().AddWave(std::make_unique<Wave2>());
	WaveManager::GetInstance().AddWave(std::make_unique<WaveFinal>());
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
	//DrawString(0, 0, "GameScene", 0xffffff);

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

		// 魔法クラスのポインター取得
		auto Magics = player_->GetWeapon()->GetMagics();

		// 魔法の数分回す
		for (auto Magic : Magics)
		{
			// 魔法が生存していなかったら次の魔法に進む
			if (!Magic->IsCollisionState())
			{
				continue;
			}

			// 魔法の情報
			auto MagicInfo = Magic->GetMagic();

			// 魔法の移動経路の線分を定義
			VECTOR MagicLineStart = MagicInfo.pos_;
			VECTOR MagicLineEnd = MagicInfo.prevPos_; // 前のフレームでの魔法の位置

			// 魔法の半径
			float MagicRad = MagicInfo.collisionRadius_;

			// 頭の当たり判定
			if (CollisionManager::IsCollidingSphereCapsule(enePos[HEAD], eneRadHead, MagicLineStart, MagicLineEnd, MagicRad))
			{
				// 敵にダメージを与える
				enemy->SubHp(MagicInfo.headDamage_);
				// 魔法を爆発させる
				Magic->ChangeState(MagicBase::STATE::BLAST);
			}
			// 体、腕、手、脚の当たり判定
			else if (CollisionManager::IsCollidingCapsules(enePos[BODY_TOP], enePos[BODY_UNDER], eneRadBody, MagicLineStart, MagicLineEnd, MagicRad)
				|| CollisionManager::IsCollidingCapsules(enePos[ARM_TOP_R], enePos[ARM_UNDER_R], eneRadArm, MagicLineStart, MagicLineEnd, MagicRad)
				|| CollisionManager::IsCollidingCapsules(enePos[ARM_TOP_L], enePos[ARM_UNDER_L], eneRadArm, MagicLineStart, MagicLineEnd, MagicRad)
				|| CollisionManager::IsCollidingSphereCapsule(enePos[HAND_R], eneRadHand, MagicLineStart, MagicLineEnd, MagicRad)
				|| CollisionManager::IsCollidingSphereCapsule(enePos[HAND_L], eneRadHand, MagicLineStart, MagicLineEnd, MagicRad)
				|| CollisionManager::IsCollidingCapsules(enePos[LEG_TOP_R], enePos[LEG_UNDER_R], eneRadLeg, MagicLineStart, MagicLineEnd, MagicRad)
				|| CollisionManager::IsCollidingCapsules(enePos[LEG_TOP_L], enePos[LEG_UNDER_L], eneRadLeg, MagicLineStart, MagicLineEnd, MagicRad))
			{
				// 敵にダメージを与える
				enemy->SubHp(MagicInfo.bodyDamage_);
				// 魔法を爆発させる
				Magic->ChangeState(MagicBase::STATE::BLAST);
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
	if ((int)enemys_.size() <= 0)
	{
		isEnd_ = false;
	}

	for (auto& enemy : enemys_)
	{
		if (enemy->GetEnemy().isAlive_)
		{
			// 敵が一匹でも残っていたら終了しない
			isEnd_ = false;
			break;
		}
	}

	// WAVEが最終段階でなかったらクリアに遷移しないように処理を終了させる
	if (WaveManager::GetInstance().GetCurrentWave() != nullptr)
	{
		return;
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

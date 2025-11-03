#include <DxLib.h>
#include "../../Application.h"
#include "../../Manager/Camera.h"
#include "../../Manager/InputManager.h"
#include "../../Object/Player/Player.h"
#include "../../Object/Common/Cursor.h"
#include "../../Common/Score/Score.h"
#include "../../Object/Enemy/Zombie/Zombie.h"
#include "../../Object/Player/Weapon/WeaponBase.h"
#include "../../Object/Magic/MagicBase.h"
#include "../../Utility/Collision/CollisionUtility.h"
#include "../../Scene/SceneManager.h"
#include "../../Manager/SystemManager.h"
#include "../../Manager/SoundManager.h"
#include "../../Common/Pause/Pause.h"
#include "../../Common/Effect/RedDamageEffect.h"
#include "../../Wave/WaveManager.h"
#include "../../Wave/Wave1.h"
#include "../../Wave/Wave2.h"
#include "../../Wave/WaveFinal.h"
#include "../../Object/Enemy/EnemyManager.h"
#include "../../Object/Player/Upgrade/UpgradeManager.h"
#include "../../Object/SkyDome/SkyDome.h"
#include "../../Object/Spawner/SpawnerManager.h"
#include "../../Object/Spawner/Spawner.h"
#include "../../Utility/AsoUtility.h"
#include "GameScene.h"

GameScene::GameScene(void)
{
	player_ = nullptr;
	camera_ = nullptr;
	cursor_ = nullptr;
	score_ = nullptr;
	pause_ = nullptr;
	redEffect_ = nullptr;
	skydome_ = nullptr;

	// マウスカーソルを表示しない
	SetMouseDispFlag(false);
}

GameScene::~GameScene(void)
{
}

void GameScene::Load(void)
{

	// プレイヤー生成・ロード
	player_ = new Player();
	player_->Load();

	// 敵マネージャの生成・ロード
	EnemyManager::CreateInstance();
	EnemyManager::GetInstance().GetPlayerPoint(player_);
	EnemyManager::GetInstance().Load();

	// カメラの生成
	camera_ = new Camera(player_);

	// カーソルの生成・ロード
	cursor_ = new Cursor();
	cursor_->Load();

	// スコアの生成
	score_ = new Score();

	// ポーズモードの生成・ロード
	pause_ = new Pause();
	pause_->Load();

	// エフェクトの生成・ロード
	redEffect_ = new RedDamageEffect();
	redEffect_->Load();

	// スカイドームの生成・ロード
	skydome_ = new SkyDome();
	skydome_->Load();

	// ウェーブの作成・各ウェーブの追加
	WaveManager::CreateInstance();
	WaveManager::GetInstance().AddWave(std::make_unique<Wave1>());
	WaveManager::GetInstance().AddWave(std::make_unique<Wave2>());
	WaveManager::GetInstance().AddWave(std::make_unique<WaveFinal>());

	// スポナーを生成・ロード
	SpawnerManager::CreateInstance();
	SpawnerManager::GetInstance().Load();

	// アップグレードの作成
	UpgradeManager::CreateInstance();
	UpgradeManager::GetInstance().Load(player_);
}

void GameScene::Init(void)
{
	// スコアの初期化
	SystemManager::GetInstance().ResetGame();

	// プレイヤーの初期化
	player_->Init();

	// カメラの初期化
	camera_->Init();

	// カーソルの初期化
	cursor_->Init();

	// スコアの初期化
	score_->Init();

	// ポーズモードの初期化
	pause_->Init();

	// エフェクトの初期化
	redEffect_->Init();

	// スカイドーム
	skydome_->Init(camera_->GetPos());

	// アップグレードの初期化
	UpgradeManager::GetInstance().Init();

	ChangeState(STATE::PLAY);

	// BGMをかける
	SoundManager::GetInstance().Play(SoundManager::BGM::GAME);
}

void GameScene::Update(void)
{
	// ポーズモード確認
	prevPause_ = nowPause_;
	nowPause_ = pause_->GetPauseMode();

	if (!prevPause_ && !nowPause_)
	{
		// グリッド更新
		switch (state_)
		{
		case GameScene::STATE::PLAY:

			// スカイドームの更新
			skydome_->Update(camera_->GetPos());

			// プレイヤー更新
			player_->Update();

			// 敵の更新
			EnemyManager::GetInstance().Update();

			// カメラの更新
			camera_->Update();

			// エフェクトの更新
			redEffect_->Update();

			// スポナーの更新
			SpawnerManager::GetInstance().Update();

			// 当たり判定
			Collisions();

			// ゲームクリア・ゲームオーバー判定
			IsClear();
			IsOver();

			// アップグレードモードスタート条件
			StartUpgrade();

			break;
		case GameScene::STATE::UPGRADE:

			if (!GetMouseDispFlag())
			{
				// 表示が行われていなかったら表示する
				SetMouseDispFlag(true);
			}

			UpgradeManager::GetInstance().Update();

			// アップグレードモード終了条件
			StopUpgrade();

			break;
		}

		// ウェーブの更新
		WaveManager::GetInstance().Update();

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

	// スカイドームの描画
	skydome_->Draw();

	// 敵の描画
	EnemyManager::GetInstance().Draw();

	// スポナーの描画
	SpawnerManager::GetInstance().Draw();

	// プレイヤーの描画
	player_->Draw();

	// エフェクトの描画
	redEffect_->Draw();

	// カーソルの描画
	cursor_->Draw();

	// スコアの描画
	score_->Draw();

	// アップグレードの描画
	UpgradeManager::GetInstance().Draw();

	// ウェーブの描画
	WaveManager::GetInstance().Draw();

	// ポーズモードの描画
	pause_->Draw();

#ifdef _DEBUG
	//DrawString(0, 0, "GameScene", 0xffffff);

	// カメラのデバック描画
	camera_->DrawDebug();
#endif // _DEBUG

}

void GameScene::Release(void)
{
	// アップグレードの開放
	UpgradeManager::GetInstance().Destroy();

	// スポナーの解放
	SpawnerManager::GetInstance().Destroy();

	// ウェーブの解放
	WaveManager::GetInstance().DeleteInstance();

	// ポーズモードの解放
	if (skydome_ != nullptr)
	{
		skydome_->Release();
		delete skydome_;
		skydome_ = nullptr;
	}
	
	// ポーズモードの解放
	if (redEffect_ != nullptr)
	{
		delete redEffect_;
		redEffect_ = nullptr;
	}

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

	// マウスカーソルを表示させる
	SetMouseDispFlag(true);

	// BGMをかける
	SoundManager::GetInstance().Stop(SoundManager::BGM::GAME);
}

void GameScene::Collisions(void)
{
	// 敵やプレイヤーにダメージが入る当たり判定
	DamageCollision();
	// 敵同士の押し出し判定
	enemiesExtrusionCollision();
	// スポナーとプレイヤーの攻撃の当たり判定
	SpawnerAndAttackCollision();
}

void GameScene::DamageCollision(void)
{
	auto& eneManaIns = EnemyManager::GetInstance();
	// 生成してある敵を取得
	auto& enemies_ = eneManaIns.GetEnemy();

	for (auto* enemy : enemies_)
	{
		if (!enemy->GetEnemy().isAlive_)
		{
			// 生存していなければ処理を行わない
			continue;
		}

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
		float eneRadHead = eneInfo.collisionRadiusHead_;
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
			if (CollisionUtility::IsCollidingSphereCapsule(enePos[HEAD], eneRadHead, MagicLineStart, MagicLineEnd, MagicRad))
			{
				// 敵にダメージを与える
				enemy->SubHp(MagicInfo.headDamage_);
				// 魔法を爆発させる
				Magic->ChangeState(MagicBase::STATE::BLAST);

				// ダメージSEをながす
				SoundManager::GetInstance().Play(SoundManager::SE::DAMEGED_ENEMY);
			}
			// 体、腕、手の当たり判定
			else if (CollisionUtility::IsCollidingCapsules(enePos[BODY_TOP], enePos[BODY_UNDER], eneRadBody, MagicLineStart, MagicLineEnd, MagicRad)
				|| CollisionUtility::IsCollidingCapsules(enePos[ARM_TOP_R], enePos[ARM_UNDER_R], eneRadArm, MagicLineStart, MagicLineEnd, MagicRad)
				|| CollisionUtility::IsCollidingCapsules(enePos[ARM_TOP_L], enePos[ARM_UNDER_L], eneRadArm, MagicLineStart, MagicLineEnd, MagicRad)
				|| CollisionUtility::IsCollidingSphereCapsule(enePos[HAND_R], eneRadHand, MagicLineStart, MagicLineEnd, MagicRad)
				|| CollisionUtility::IsCollidingSphereCapsule(enePos[HAND_L], eneRadHand, MagicLineStart, MagicLineEnd, MagicRad))
			{
				// 敵にダメージを与える
				enemy->SubHp(MagicInfo.bodyDamage_);
				// 魔法を爆発させる
				Magic->ChangeState(MagicBase::STATE::BLAST);

				// ダメージSEをながす
				SoundManager::GetInstance().Play(SoundManager::SE::DAMEGED_ENEMY);
			}
		}

		if (!enemy->IsAttack())
		{
			// 敵が攻撃状態ではなかったら抜ける
			continue;
		}

		VECTOR plaPosTop = player_->GetCollisionPosTop();
		VECTOR plaPosUnder = player_->GetCollisionPosUnder();
		float plaRad = player_->GetPlayer().collisionRadius_;

		// プレイヤーと敵の攻撃の当たり判定
		if (CollisionUtility::IsCollidingSphereCapsule(enePos[HAND_R], eneRadHand, plaPosTop, plaPosUnder, plaRad))
		{
			// プレイヤーにダメージを与える
			player_->Damage(1);
			enemy->SetIsAttack(false);

			// カメラを揺らす
			camera_->SetHitStop();

			// 画面を赤くするエフェクトを付ける
			redEffect_->SetRedEffect();

			// ダメージSEをながす
			SoundManager::GetInstance().Play(SoundManager::SE::DAMEGED);
		}

	}
}

void GameScene::enemiesExtrusionCollision(void)
{
	auto& eneManaIns = EnemyManager::GetInstance();
	auto& enemies = eneManaIns.GetEnemy();

	for (int i = 0; i < enemies.size(); i++)
	{
		if (!enemies[i]->GetEnemy().isAlive_)
		{
			continue;
		}

		for (int j = i + 1; j < enemies.size(); j++)
		{

			if (!enemies[j]->GetEnemy().isAlive_)
			{
				continue;
			}

			// 敵１の情報
			VECTOR ene1Pos = enemies[i]->GetEnemy().pos_;
			float ene1CollRad = enemies[i]->GetEnemy().collisionRadius_;

			// 敵2の情報
			VECTOR ene2Pos = enemies[j]->GetEnemy().pos_;
			float ene2CollRad = enemies[j]->GetEnemy().collisionRadius_;

			// 押し出し判定を行う
			VECTOR pushPow = CollisionUtility::ExtrusionCollision(ene1Pos, ene1CollRad, ene2Pos, ene2CollRad);

			// 敵1の押し出しを行う
			enemies[i]->Extrusion(pushPow);

			// 敵1の方向とは逆のほうへ押し出しを行うように符号反転させる
			pushPow = VScale(pushPow, -1.0f);
			// 敵2の押し出しを行う
			enemies[j]->Extrusion(pushPow);
		}
	}
}

void GameScene::SpawnerAndAttackCollision(void)
{
	// スポナーの情報
	auto spawners = SpawnerManager::GetInstance().GetSpawners();

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
		for (auto& spawner : spawners)
		{
			// 存在していなかったら次のスポナーを見る
			if (!spawner->GetSpawnerIns().isExists_)
			{
				continue;
			}

			// 座標を取得
			VECTOR spawnerPos = spawner->GetSpawnerIns().pos_[0];

			// 半径を取得
			float spawnerRad = spawner->GetSpawnerIns().collisionRadius_;

			// 当たり判定
			if (CollisionUtility::IsCollidingSphereCapsule(spawnerPos, spawnerRad, MagicLineStart, MagicLineEnd, MagicRad))
			{
				// 当たっていたら
				// スポナー耐久値にダメージを与える
				spawner->Damage(1);
				// 魔法を爆発させる
				Magic->ChangeState(MagicBase::STATE::BLAST);

			}
		}
	}
}

void GameScene::IsClear(void)
{
	auto& eneManaIns = EnemyManager::GetInstance();

	auto& enemies_ = eneManaIns.GetEnemy();

	bool isEnd_ = true;
	if ((int)enemies_.size() <= 0)
	{
		isEnd_ = false;
	}

	for (auto& enemy : enemies_)
	{
		if (enemy->GetEnemy().isAlive_)
		{
			// 敵が一匹でも残っていたら終了しない
			isEnd_ = false;
			break;
		}
	}

	// 次にWAVEが控えていたらクリアに遷移しないように処理を終了させる
	if (WaveManager::GetInstance().GetNextWave() != nullptr)
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

void GameScene::StartUpgrade(void)
{
	if (WaveManager::GetInstance().GetWaveIsClear())
	{
		// アップグレードモードにする
		UpgradeManager::GetInstance().StartIsUpgrade();
		ChangeState(STATE::UPGRADE);

		// アップグレードスタート時SEながす
		SoundManager::GetInstance().Play(SoundManager::SE::UPGRADE);
	}
}

void GameScene::StopUpgrade(void)
{
	auto* wave = WaveManager::GetInstance().GetCurrentWave();

	if (wave == nullptr)
	{
		// 中身がなかったら処理を行わない
		return;
	}

	if (UpgradeManager::GetInstance().GetIsUpgradeEnd() || wave->GetState() == WaveBase::WaveState::INWAVE)
	{
		if (wave->GetState() != WaveBase::WaveState::INWAVE)
		{
			// ウェーブが始まっていなければ、強制的にウェーブを始める
			wave->StartInWave();
		}
		else
		{
			// 決定する前にINWAVEとなった場合強制的にアップグレードを終了させる
			UpgradeManager::GetInstance().StopIsUpgrade();
		}

		// 選択処理が終わったため前ウェーブのクリア情報を消去
		WaveManager::GetInstance().EndWaveIsClear();
		ChangeState(STATE::PLAY);

		// 攻撃チャージ状態だったら強制的にアタック状態にする
		if (player_->GetWeapon()->GetState() == WeaponBase::STATE::CHARGE_MAGIC)
		{
			player_->GetWeapon()->ChangeState(WeaponBase::STATE::ATTACK);
		}

	}
}


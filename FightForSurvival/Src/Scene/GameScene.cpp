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
#include "../Manager/SoundManager.h"
#include "../Common/Pause/Pause.h"
#include "../Common/Effect/RedDamageEffect.h"
#include "../Wave/WaveManager.h"
#include "../Wave/Wave1.h"
#include "../Wave/Wave2.h"
#include "../Wave/WaveFinal.h"
#include "../Object/Enemy/EnemyManager.h"
#include "../Object/Player/Upgrade/UpgradeManager.h"
#include "GameScene.h"

GameScene::GameScene(void)
{
	grid_ = nullptr;
	player_ = nullptr;
	camera_ = nullptr;
	cursor_ = nullptr;
	score_ = nullptr;
	pause_ = nullptr;
	redEffect_ = nullptr;

	// マウスカーソルを表示しない
	SetMouseDispFlag(false);
}

GameScene::~GameScene(void)
{
}

void GameScene::Load(void)
{

	// グリッド生成
	grid_ = new Grid();

	// プレイヤー生成・ロード
	player_ = new Player();
	player_->Load();

	// 敵マネージャの生成・ロード
	EnemyManager::CreateInstance();
	EnemyManager::GetInstance().Load();
	EnemyManager::GetInstance().GetPlayerPoint(player_);

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

	// ウェーブの作成・各ウェーブの追加
	WaveManager::CreateInstance();
	WaveManager::GetInstance().AddWave(std::make_unique<Wave1>());
	WaveManager::GetInstance().AddWave(std::make_unique<Wave2>());
	WaveManager::GetInstance().AddWave(std::make_unique<WaveFinal>());

	// アップグレードの作成
	UpgradeManager::CreateInstance();
	UpgradeManager::GetInstance().Load(player_);
}

void GameScene::Init(void)
{
	// スコアの初期化
	SystemManager::GetInstance().ResetGame();

	// グリッドの初期化
	grid_->Init();

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

			grid_->Update();

			// プレイヤー更新
			player_->Update();

			// 敵の更新
			EnemyManager::GetInstance().Update();

			// カメラの更新
			camera_->Update();

			// エフェクトの更新
			redEffect_->Update();

			// 当たり判定
			Collisions();

			// ゲームクリア・ゲームオーバー判定
			IsClear();
			IsOver();

			// アップグレードモードスタート条件
			StartUpgrade();

			break;
		case GameScene::STATE::UPGRADE:

			UpgradeManager::GetInstance().Update();

			// アップグレードモード終了条件
			StopUpgrade();

			break;
		}

		// ウェーブの更新
		WaveManager::GetInstance().Update();

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
	EnemyManager::GetInstance().Draw();

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

	// ウェーブの解放
	WaveManager::GetInstance().DeleteInstance();

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

	// グリッドの解放
	if (grid_ != nullptr)
	{
		grid_->Release();
		delete grid_;
		grid_ = nullptr;
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
	// 敵やプレイヤーの押し出し判定
	ExtrusionCollision();
}

void GameScene::DamageCollision(void)
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
			if (CollisionManager::IsCollidingSphereCapsule(enePos[HEAD], eneRadHead, MagicLineStart, MagicLineEnd, MagicRad))
			{
				// 敵にダメージを与える
				enemy->SubHp(MagicInfo.headDamage_);
				// 魔法を爆発させる
				Magic->ChangeState(MagicBase::STATE::BLAST);

				// ダメージSEをながす
				SoundManager::GetInstance().Play(SoundManager::SE::DAMEGED_ENEMY);
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

				// ダメージSEをながす
				SoundManager::GetInstance().Play(SoundManager::SE::DAMEGED_ENEMY);
			}
		}

		if (!enemy->IsAttack())
		{
			// 敵が攻撃状態ではなかったら抜ける
			continue;
		}

		VECTOR plaPos = player_->GetPlayer().pos_;
		float plaRad = player_->GetPlayer().collisionRadiusHead_;

		// プレイヤーと敵の攻撃の当たり判定
		if (CollisionManager::IsCollidingSpheres(plaPos, plaRad, enePos[HAND_R], eneRadHand))
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

void GameScene::ExtrusionCollision(void)
{
	auto& eneManaIns = EnemyManager::GetInstance();
	auto& enemys_ = eneManaIns.GetEnemy();

	for (int i = 0; i < enemys_.size(); i++)
	{
		for (int j = i + 1; j < enemys_.size(); j++)
		{

			VECTOR ene1Pos = enemys_[i]->GetEnemy().pos_;
			VECTOR ene2Pos = enemys_[j]->GetEnemy().pos_;

			// 球体と球体の衝突判定
			// ２つの座標間の距離をピタゴラスの定理で算出

			VECTOR distance;
			distance.x = ene2Pos.x - ene1Pos.x;
			distance.y = ene2Pos.y - ene1Pos.y;
			distance.z = ene2Pos.z - ene1Pos.z;

			float dis = distance.x * distance.x + distance.y * distance.y + distance.z * distance.z;

			// お互いの半径を合計する
			float radius = enemys_[i]->GetEnemy().collisionRadius_ + enemys_[j]->GetEnemy().collisionRadius_;

			// 合計した半径の２乗よりも、
			// ２つの座標間の距離が小さければ球体は衝突している
			if (radius * radius > dis && dis != 0.0f)
			{
				float length = sqrtf(dis);
				auto overlap = radius - length;

				// 正規化ベクトル（A -> Bの方向）
				VECTOR vec = VNorm(distance);

				// 重なり量の半分
				float push_half = overlap / 2.0f;

				// 敵A (i) の押し出し：Bから離れる方向
				// A -> Bの逆方向 (-vec) に push_half だけ移動
				enemys_[i]->Extrusion(VScale(vec, -push_half));

				// 敵B (j) の押し出し：Aから離れる方向
				// A -> Bの順方向 (+vec) に push_half だけ移動
				enemys_[j]->Extrusion(VScale(vec, push_half));
			}
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


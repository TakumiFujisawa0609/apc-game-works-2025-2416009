#include <DxLib.h>
#include "../../Application.h"
#include "../../Manager/Camera.h"
#include "../../Manager/InputManager.h"
#include "../../Object/Player/Player.h"
#include "../../Object/Common/Cursor.h"
#include "../../Common/Score/Score.h"
#include "../../Object/Enemy/Zombie/Zombie.h"
#include "../../Object/Enemy/Dragon/Dragon.h"
#include "../../Object/Player/Weapon/WeaponBase.h"
#include "../../Object/Magic/Base/MagicBase.h"
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
#include "../../Object/Stage/Stage.h"
#include "../../Utility/AsoUtility.h"
#include "GameScene.h"

#include "../../UI/UIManager.h"
#include "../../UI/TextrueManager/TextureManager.h"
#include "../../UI/Object/MagicStatus/MagicStatus.h"
#include "../../UI/Object/Bar/HPBar.h"
#include "../../UI/Object/Bar/StaminaBar.h"

GameScene::GameScene(void)
{
	player_ = nullptr;
	camera_ = nullptr;
	cursor_ = nullptr;
	score_ = nullptr;
	pause_ = nullptr;
	redEffect_ = nullptr;
	skydome_ = nullptr;
	stage_ = nullptr;
	texMgr_ = nullptr;
	uiMgr_ = nullptr;

	prevPause_ = -1;
	nowPause_ = -1;

	state_ = STATE::PLAY;

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
	score_->Load();

	// ポーズモードの生成・ロード
	pause_ = new Pause();
	pause_->Load();

	// エフェクトの生成・ロード
	redEffect_ = new RedDamageEffect();
	redEffect_->Load();

	// スカイドームの生成・ロード
	skydome_ = new SkyDome();
	skydome_->Load();

	// ステージの生成・ロード
	stage_ = new Stage();
	stage_->Load();

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

	// UI管理の生成処理
	uiMgr_ = new UIManager();
	texMgr_ = new TextureManager();

	// UIを生成
	UIBase* magicStatus = UIFactory::GetInstance()->CreateUI(UI_KIND::MAGIC_STATUS, texMgr_);

	// ダウンキャストして Bar固有の関数を呼べるようにする
	MagicStatus* magic = dynamic_cast<MagicStatus*>(magicStatus);
	magic->SetPlayer(player_);
	uiMgr_->AddUI(magic);

	// ダウンキャストして Bar固有の関数を呼べるようにする
	UIBase* hBar = UIFactory::GetInstance()->CreateUI(UI_KIND::HP_BAR, texMgr_);
	HPBar* hp = dynamic_cast<HPBar*>(hBar);
	hp->SetPlayer(player_);
	uiMgr_->AddUI(hp);

	// ダウンキャストして Bar固有の関数を呼べるようにする
	UIBase* sBar = UIFactory::GetInstance()->CreateUI(UI_KIND::STAMINA_BAR, texMgr_);
	StaminaBar* stamina = dynamic_cast<StaminaBar*>(sBar);
	stamina->SetPlayer(player_);
	uiMgr_->AddUI(stamina);
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

	// ステージ
	stage_->Init();

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

			// ステージの更新
			stage_->Update();

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

			// UIの更新
			uiMgr_->Update();

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

	// ステージの更新
	stage_->Draw();

	// 敵の描画
	EnemyManager::GetInstance().Draw();

	// スポナーの描画
	SpawnerManager::GetInstance().Draw();

	// プレイヤーの描画
	player_->Draw();

	// エフェクトの描画
	redEffect_->Draw();

	// スコアの描画
	score_->Draw();

	// UIの描画
	uiMgr_->Draw();

	// プレイヤー情報(2D)の描画
	player_->Draw2D();

	// カーソルの描画
	cursor_->Draw();

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
	// UIの解放
	delete uiMgr_;
	delete texMgr_;

	// アップグレードの開放
	UpgradeManager::GetInstance().Destroy();

	// スポナーの解放
	SpawnerManager::GetInstance().Destroy();

	// ウェーブの解放
	WaveManager::GetInstance().DeleteInstance();

	// ステージの解放
	if (stage_ != nullptr)
	{
		stage_->Release();
		delete stage_;
		stage_ = nullptr;
	}

	// スカイドームの解放
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

	// BGMを止める
	SoundManager::GetInstance().Stop(SoundManager::BGM::GAME);
}

void GameScene::Collisions(void)
{
	// プレイヤーの魔法当たり判定
	MagicCollision();
	// 爆発魔法
	ExplosionMagicCollision();

	// 敵の攻撃の当たり判定
	EnemiesAttackCollision();
	// 敵同士の押し出し判定
	EnemiesExtrusionCollision();
	// スポナーとプレイヤーの攻撃の当たり判定
	SpawnerAndAttackCollision();
	// ステージとプレイヤーの当たり判定
	StageAndPlayerCollision();
	// ステージと敵の当たり判定
	StageAndEnemiesCollision();
}

void GameScene::MagicCollision(void)
{
	// 生成してある敵を取得
	auto& enemies_ = EnemyManager::GetInstance().GetEnemy();

	// 魔法クラスのポインター取得
	auto magics = player_->GetWeapon()->GetMagics();


	// 魔法の数分回す
	for (auto& magic : magics)
	{
		// 魔法が爆発魔法か見る(ture / 爆発魔法ではない、false / 爆発魔法である）
		bool notExplosionMagic = { magic->GetMagic().typeMagic_ != TYPE_MAGIC::EXPLOSION_MAGIC };

		// 魔法がSHOT状態でなければ処理を飛ばす
		if (magic->GetState() != MagicBase::STATE::SHOT)
		{
			continue;
		}

		// 魔法の情報
		Magic magicInfo = magic->GetMagic();

		// 魔法の移動経路の線分を定義
		VECTOR magicLineStart = magicInfo.pos_;
		VECTOR magicLineEnd = magicInfo.prevPos_; // 前のフレームでの魔法の位置

		// 魔法の半径
		float magicRad = magicInfo.collisionRadius_;
		for (auto* enemy : enemies_)
		{
			if (!enemy->IsCollisionState() || !enemy->GetEnemy().isAlive_)
			{
				// 生存していなければ処理を行わない
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

			// 頭の当たり判定(ボスは頭の当たり判定を行わない)
			if (CollisionUtility::IsCollidingSphereCapsule(enePos[HEAD], eneRadHead, magicLineStart, magicLineEnd, magicRad) && enemy->GetType() != ENEMY_TYPE::DRAGON)
			{
				// 爆発魔法でなければダメージを与える
				if (notExplosionMagic)
				{
					// 敵にダメージを与える
					enemy->SubHp(magicInfo.headDamage_);
					// ダメージSEをながす
					SoundManager::GetInstance().Play(SoundManager::SE::DAMEGED_ENEMY);
				}

				// 魔法を爆発させる
				magic->ChangeState(MagicBase::STATE::BLAST);

			}
			// 体、腕、手の当たり判定
			else if (CollisionUtility::IsCollidingCapsules(enePos[BODY_TOP], enePos[BODY_UNDER], eneRadBody, magicLineStart, magicLineEnd, magicRad)
				|| CollisionUtility::IsCollidingCapsules(enePos[ARM_TOP_R], enePos[ARM_UNDER_R], eneRadArm, magicLineStart, magicLineEnd, magicRad)
				|| CollisionUtility::IsCollidingCapsules(enePos[ARM_TOP_L], enePos[ARM_UNDER_L], eneRadArm, magicLineStart, magicLineEnd, magicRad)
				|| CollisionUtility::IsCollidingSphereCapsule(enePos[HAND_R], eneRadHand, magicLineStart, magicLineEnd, magicRad)
				|| CollisionUtility::IsCollidingSphereCapsule(enePos[HAND_L], eneRadHand, magicLineStart, magicLineEnd, magicRad))
			{
				// 爆発魔法でなければダメージを与える
				if (notExplosionMagic)
				{
					// 敵にダメージを与える
					enemy->SubHp(magicInfo.bodyDamage_);
					// ダメージSEをながす
					SoundManager::GetInstance().Play(SoundManager::SE::DAMEGED_ENEMY);
				}

				// 魔法を爆発させる
				magic->ChangeState(MagicBase::STATE::BLAST);
			}
		}
	}
}

void GameScene::ExplosionMagicCollision(void)
{
	// 魔法クラスのポインター取得
	auto magics = player_->GetWeapon()->GetMagics();

	// 魔法の数分回す
	for (auto& magic : magics)
	{
		// 魔法の種類が爆発魔法以外であれば処理を行わない
		if (magic->GetMagic().typeMagic_ != TYPE_MAGIC::EXPLOSION_MAGIC)
		{
			continue;
		}

		// 魔法の種類が爆発魔法かつ、爆発中であれば処理を行う
		if (magic->GetMagic().isExists_ && magic->GetState() == MagicBase::STATE::BLAST)
		{
			// 魔法の情報
			Magic magicInfo = magic->GetMagic();

			// 魔法の移動経路の線分を定義
			VECTOR magicPos = magicInfo.pos_;

			// 魔法の半径
			float magicRad = magicInfo.collisionRadius_;

			// 生成してある敵を取得
			auto& enemies_ = EnemyManager::GetInstance().GetEnemy();
			for (auto* enemy : enemies_)
			{
				if (!enemy->IsCollisionState() || !enemy->GetEnemy().isAlive_)
				{
					// 生存していなければ処理を行わない
					continue;
				}

				if (CollisionUtility::IsCollidingSphereCapsule(magicPos, magicRad, enemy->GetColPos().colPos_[COLLISION_POS::BODY_TOP], enemy->GetColPos().colPos_[COLLISION_POS::BODY_UNDER], enemy->GetEnemy().collisionRadiusBody_))
				{
					// 敵にダメージを与える
					enemy->SubHp(magicInfo.bodyDamage_);
				}
			}

			// 爆発範囲判定が終了したため存在を消す(描画は別フラグで行っている)
			magic->SetIsExists(false);
		}
	}
}

void GameScene::EnemiesAttackCollision(void)
{
	// 生成してある敵を取得
	auto& enemies_ = EnemyManager::GetInstance().GetEnemy();

	// 固有の攻撃の場合の当たり判定処理
	for (auto* enemy : enemies_)
	{
		if (!enemy->IsCollisionState() || !enemy->GetEnemy().isAlive_)
		{
			// 生存していなければ処理を行わない
			continue;
		}

		if (!enemy->IsAttack())
		{
			// 敵が攻撃状態ではなかったら抜ける
			continue;
		}

		switch (enemy->GetType())
		{
		case ENEMY_TYPE::ZOMBIE:


			// ゾンビの攻撃中の右手と当たり判定を行う
			ZombieAttackCollision(enemy);

			break;
		case ENEMY_TYPE::BAT:

			break;
		case ENEMY_TYPE::DRAGON:

			// ドラゴンの固有攻撃処理の当たり判定
			DragonAttackCollision(enemy);

			break;
		default:
			break;
		}

	}

	// 敵全体の魔法の当たり判定を行う
	EnemyMagicCollision();

}

void GameScene::ZombieAttackCollision(EnemyBase* enemy)
{
	VECTOR enePos = enemy->GetColPos().colPos_[COLLISION_POS::HAND_R];
	float eneHandRad = enemy->GetEnemy().collisionRadiusHand_;

	VECTOR plaPosTop = player_->GetCollisionPosTop();
	VECTOR plaPosUnder = player_->GetCollisionPosUnder();
	float plaRad = player_->GetPlayerStatus().collisionRadius_;

	// プレイヤーと敵の攻撃の当たり判定
	if (CollisionUtility::IsCollidingSphereCapsule(enePos, eneHandRad, plaPosTop, plaPosUnder, plaRad))
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

void GameScene::DragonAttackCollision(EnemyBase* enemy)
{
	// Dragon自身のメンバ変数を使用するためにダウンキャストを行い、参照ポインタを作成
	Dragon* self = static_cast<Dragon*>(enemy);
	// 攻撃のステートを確認
	auto attackState = self->GetAttackState();

	switch (attackState)
	{
	case Dragon::FORWARD_ATTACK:
		DragonForwardAttackCollision(enemy);
		break;
	case Dragon::RUSH_ATTACK:
		DragonRushAttackCollision(enemy);
		break;
	default:
		break;
	}
}

void GameScene::EnemyMagicCollision(void)
{
	// 魔法クラスのポインター取得
	auto magics = EnemyManager::GetInstance().GetMagics();

	VECTOR plaPosTop = player_->GetCollisionPosTop();
	VECTOR plaPosUnder = player_->GetCollisionPosUnder();
	float plaRad = player_->GetPlayerStatus().collisionRadius_;

	// 魔法の数分回す
	for (auto magic : magics)
	{
		// 魔法がSHOT状態でなければ処理を飛ばす
		if (magic->GetState() != MagicBase::STATE::SHOT)
		{
			continue;
		}

		// 魔法の情報
		auto magicInfo = magic->GetMagic();

		// 魔法の移動経路の線分を定義
		VECTOR magicLineStart = magicInfo.prevPos_;
		VECTOR magicLineEnd = magicInfo.pos_; // 前のフレームでの魔法の位置

		// 魔法の半径
		float MagicRad = magicInfo.collisionRadius_;

		// プレイヤーと敵の攻撃の当たり判定
		if (CollisionUtility::IsCollidingCapsules(plaPosTop, plaPosUnder, plaRad, magicLineStart, magicLineEnd, MagicRad))
		{
			// 魔法を爆発させる
			magic->ChangeState(MagicBase::STATE::BLAST);

			// プレイヤーにダメージを与える
			player_->Damage(magic->GetMagic().bodyDamage_);

			// カメラを揺らす
			camera_->SetHitStop();

			// 画面を赤くするエフェクトを付ける
			redEffect_->SetRedEffect();

			// ダメージSEをながす
			SoundManager::GetInstance().Play(SoundManager::SE::DAMEGED);
		}
	}
}

void GameScene::DragonForwardAttackCollision(EnemyBase* enemy)
{
	// 前方の範囲内に入っていたいるかつ、まだ一度も攻撃が当たっていない
	if (enemy->SearchAttackRange() && enemy->IsAttack())
	{
		// 攻撃が当たったことを伝える
		enemy->SetIsAttack(false);

		// プレイヤーにダメージを与える
		player_->Damage(1);

		// カメラを揺らす
		camera_->SetHitStop();

		// 画面を赤くするエフェクトを付ける
		redEffect_->SetRedEffect();

		// ダメージSEをながす
		SoundManager::GetInstance().Play(SoundManager::SE::DAMEGED);
	}
}

void GameScene::DragonRushAttackCollision(EnemyBase* enemy)
{
	VECTOR enePosTop = enemy->GetColPos().colPos_[COLLISION_POS::BODY_TOP];
	VECTOR enePosUnder = enemy->GetColPos().colPos_[COLLISION_POS::BODY_UNDER];
	float eneBodyRad = enemy->GetEnemy().collisionRadiusBody_;

	VECTOR plaPosTop = player_->GetCollisionPosTop();
	VECTOR plaPosUnder = player_->GetCollisionPosUnder();
	float plaRad = player_->GetPlayerStatus().collisionRadius_;

	// プレイヤーと敵の攻撃の当たり判定
	if (CollisionUtility::IsCollidingCapsules(enePosTop, enePosUnder, eneBodyRad, plaPosTop, plaPosUnder, plaRad))
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

void GameScene::EnemiesExtrusionCollision(void)
{
	auto& enemies = EnemyManager::GetInstance().GetEnemy();

	for (int i = 0; i < enemies.size(); i++)
	{
		if (!enemies[i]->GetEnemy().isAlive_ || enemies[i]->GetType() == ENEMY_TYPE::DRAGON)
		{
			continue;
		}

		for (int j = i + 1; j < enemies.size(); j++)
		{

			if (!enemies[j]->GetEnemy().isAlive_ || enemies[i]->GetType() == ENEMY_TYPE::DRAGON)
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
	auto magics = player_->GetWeapon()->GetMagics();

	// 魔法の数分回す
	for (auto magic : magics)
	{
		// 魔法がSHOT状態でなければ処理を飛ばす
		if (magic->GetState() != MagicBase::STATE::SHOT)
		{
			continue;
		}

		// 魔法の情報
		auto magicInfo = magic->GetMagic();

		// 魔法の移動経路の線分を定義
		VECTOR magicLineStart = magicInfo.pos_;
		VECTOR magicLineEnd = magicInfo.prevPos_; // 前のフレームでの魔法の位置

		// 魔法の半径
		float magicRad = magicInfo.collisionRadius_;
		for (auto& spawner : spawners)
		{
			// 存在していなかったら次のスポナーを見る
			if (!spawner->GetSpawnerIns().isExists_)
			{
				continue;
			}

			// 座標を取得
			VECTOR spawnerPos = spawner->GetSpawnerIns().basePos_;

			// 半径を取得
			float spawnerRad = spawner->GetSpawnerIns().collisionRadius_;

			// 当たり判定
			if (CollisionUtility::IsCollidingSphereCapsule(spawnerPos, spawnerRad, magicLineStart, magicLineEnd, magicRad))
			{
				// 当たっていたら
				// スポナー耐久値にダメージを与える
				spawner->Damage(magic->GetMagic().bodyDamage_);
				// 魔法を爆発させる
				magic->ChangeState(MagicBase::STATE::BLAST);

			}
		}
	}
}

void GameScene::StageAndPlayerCollision(void)
{
	// カプセル上側の座標
	VECTOR capsuleStartPos = player_->GetCollisionPosTop();

	// カプセル下側の座標
	VECTOR capsuleEndPos = player_->GetPlayerStatus().pos_;

	// 当たり判定半径
	float rad = player_->GetPlayerStatus().collisionRadius_;

#pragma region CapsuleCollision

	// ステージとの当たり判定を行う(カプセル)
	VECTOR movePos = CollisionUtility::CoolisionCapsule(
		capsuleStartPos,
		capsuleEndPos,
		rad,
		stage_->GetModelId());

	// X,Z軸のみの押し出しを行う
	movePos.y = 0.0f;
	player_->CollisionStage(movePos);

	// ステージの壁(外に出ないようにする描画していないモデル)との当たり判定(カプセル)
	movePos = CollisionUtility::CoolisionCapsule(
		capsuleStartPos,
		capsuleEndPos,
		rad,
		stage_->GetWallModelId());

	// X,Z軸のみの押し出しを行う
	movePos.y = 0.0f;
	player_->CollisionStage(movePos);

#pragma endregion

#pragma region LineCollision

	// 線分上側の座標
	VECTOR lineStartPos = player_->GetPlayerStatus().pos_;

	// 線分下側の座標
	VECTOR lineEndPos = player_->GetCollisionPosUnder();
	lineEndPos.y -= (Player::COLLISION_RADIUS + 10.0f);

	// ステージとの当たり判定を行う(ライン)
	if (CollisionUtility::CollisionLine(
		lineStartPos,
		lineEndPos,
		stage_->GetModelId(), movePos))
	{
		// Y軸のみの押し出しを行う
		player_->CollisionStage(movePos.y);
	}


#pragma endregion

}

void GameScene::StageAndEnemiesCollision(void)
{
	auto& enemies = EnemyManager::GetInstance().GetEnemy();
	for (auto& enemy : enemies)
	{

		// カプセル上側の座標(敵の頭)
		VECTOR capsuleStartPos = enemy->GetColPos().colPos_[HEAD];

		// カプセル下側の座標(敵の胴体らへん)
		VECTOR capsuleEndPos = enemy->GetColPos().colPos_[BODY_TOP];
		capsuleEndPos.y -= 20.0f;

		// 当たり判定半径
		float rad = enemy->GetEnemy().collisionRadiusBody_;

#pragma region CapsuleCollision

		// ステージとの当たり判定を行う(カプセル)
		VECTOR movePos = CollisionUtility::CoolisionCapsule(
			capsuleStartPos,
			capsuleEndPos,
			rad,
			stage_->GetModelId());

		// X,Z軸のみの押し出しを行う
		movePos.y = 0.0f;
		enemy->CollisionStage(movePos);

		// ステージの壁(外に出ないようにする描画していないモデル)との当たり判定(カプセル)
		movePos = CollisionUtility::CoolisionCapsule(
			capsuleStartPos,
			capsuleEndPos,
			rad,
			stage_->GetWallModelId());

		// X,Z軸のみの押し出しを行う
		movePos.y = 0.0f;
		enemy->CollisionStage(movePos);

#pragma endregion

#pragma region LineCollision

		// 線分上側の座標
		VECTOR lineStartPos = enemy->GetEnemy().pos_;
		lineStartPos.y += 20.0f;

		// 線分下側の座標
		VECTOR lineEndPos = enemy->GetEnemy().pos_;
		lineEndPos.y -= 20.0f;

		// ステージとの当たり判定を行う(ライン)
		if (CollisionUtility::CollisionLine(
			lineStartPos,
			lineEndPos,
			stage_->GetModelId(), movePos))
		{
			// Y軸のみの押し出しを行う
			enemy->CollisionStage(movePos.y);
		}

#pragma endregion

	}
}

void GameScene::IsClear(void)
{
	auto& enemies_ = EnemyManager::GetInstance().GetEnemy();

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
	if (!player_->GetPlayerStatus().isAlive_)
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


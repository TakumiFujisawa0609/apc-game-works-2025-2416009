#include <DxLib.h>
#include "../../Application.h"
#include "../../Manager/SceneManager.h"
#include "EnemyManager.h"

EnemyManager* EnemyManager::instance_ = nullptr;

EnemyManager::EnemyManager(void) {}
EnemyManager::~EnemyManager(void) {}

void EnemyManager::AddEnemy(EnemyBase* enemy)
{
	enemys_.emplace_back(std::move(enemy));
}

//
//void EnemyManager::Load(void)
//{
//	// エネミーモデルのロード
//	enemyModelIds_.emplace_back(
//		MV1LoadModel((Application::PATH_MODEL + "Enemy/Demon.mv1").c_str()));
//	//enemyModelIds_.emplace_back(
//	//	MV1LoadModel((Application::PATH_MODEL + "Enemy/Wizard.mv1").c_str()));
//	//enemyModelIds_.emplace_back(
//	//	MV1LoadModel((Application::PATH_MODEL + "Enemy/Giant.mv1").c_str()));
//
//	//// 攻撃エフェクト用のモデルのロード
//	//attackEffectModelIds_.emplace_back(
//	//	MV1LoadModel(
//	//		(Application::PATH_MODEL + "Effect/Fireball/Fireball.mv1").c_str()));
//	//attackEffectModelIds_.emplace_back(
//	//	MV1LoadModel(
//	//		(Application::PATH_MODEL + "Effect/Rockfall/Rock.mv1").c_str()));
//}

void EnemyManager::Update(void)
{
	// エネミーの更新
	for (auto& enemy : enemys_)
	{
		enemy->Update();
	}
}

void EnemyManager::Draw(void)
{
	for (auto& enemy : enemys_)
	{
		enemy->Draw();
	}
}

void EnemyManager::Delete(void)
{
	// Enemyクラスのメモリ解放
	for (auto& enemy : enemys_)
	{
		enemy->Release();
	}

	//// ベースとなるモデルのメモリ解放
	//for (int id : enemyModelIds_)
	//{
	//	MV1DeleteModel(id);
	//}

	enemys_.clear();
}


//void EnemyManager::ChangeWave(WAVE wave)
//{
//	wave_ = wave;
//}
//
//void EnemyManager::UpdateWave01(void)
//{
//
//	// 経過時間の取得
//	float deltaTime = SceneManager::GetInstance().GetDeltaTime();
//
//	frameNum_ += deltaTime;
//
//	zombieTime_ += deltaTime;
//
//	// 一定間隔でエネミーを出現させる
//	if (zombieTime_ >= ZOMBIE_SPAWN_INTERVAL)
//	{
//		zombieNum_++;
//		//EnemyBase* enemy = new EnemyDemon();
//
//		//enemy->Init(
//		//	EnemyBase::ENEMY_TYPE::ZOMBIE,
//		//	enemyModelIds_[static_cast<int>(EnemyBase::ENEMY_TYPE::ZOMBIE)], -1,
//		//	VGet(0.0f,0.0f,0.0f),player_);
//
//		//enemys_.emplace_back(enemy);
//
//		zombieTime_ = 0.0f;
//	}
//
//	bool next = true;
//
//	for (EnemyBase* enemy : enemys_)
//	{
//		if (enemy->GetEnemy().isAlive_)
//		{
//			// エネミーが1匹でもいたら進まない
//			next = false;
//		}
//	}
//
//	// フレームが超えるか、エネミーが0匹だったらウェーブを進める
//	if (frameNum_ >= NEXT_WAVE_FRAME || next)
//	{
//		frameNum_ = 0.0f;
//		zombieNum_ = 0;
//		ChangeWave(nextWave_);
//		nextWave_ = WAVE::WAVE02;
//	}
//}
//
//void EnemyManager::UpdateWave02(void)
//{
//}

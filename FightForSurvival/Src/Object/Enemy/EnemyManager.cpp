#include <DxLib.h>
#include "../../Application.h"
#include "../../Scene/SceneManager.h"
#include "Zombie/Zombie.h"
#include "Bat/Bat.h"
#include "EnemyManager.h"

EnemyManager* EnemyManager::instance_ = nullptr;

EnemyManager::EnemyManager(void) {}
EnemyManager::~EnemyManager(void) {}

void EnemyManager::AddEnemy(EnemyBase* enemy)
{
	enemies_.emplace_back(std::move(enemy));
}

void EnemyManager::Load(void)
{
	// 敵へのパス
	std::string enePas = Application::PATH_MODEL + "Enemy/";

	// エネミーモデルのロード
	enemyModelIds_.emplace_back(MV1LoadModel((enePas + "Zombie.mv1").c_str()));
	enemyModelIds_.emplace_back(MV1LoadModel((enePas + "Bat.mv1").c_str()));

	// 空のVector型を渡すためにアニメーションのロードの前にコウモリのメモリ確保を行う
	for (int i = 0; i < BAT_NUM; i++)
	{
		// 先にメモリ確保しておく(ゲーム途中にnewを行わないようにする)
		auto enemy = new Bat(ENEMY_TYPE::BAT, enemyModelIds_[static_cast<int>(ENEMY_TYPE::BAT)],-1,zombieAnimModelIds_, player_);
		AddEnemy(enemy);
	}

	// アニメーションのロード
	zombieAnimModelIds_.emplace_back(MV1LoadModel((enePas + "Zombie Idle.mv1").c_str()));
	zombieAnimModelIds_.emplace_back(MV1LoadModel((enePas + "Zombie Running.mv1").c_str()));
	zombieAnimModelIds_.emplace_back(MV1LoadModel((enePas + "Standing Melee Attack Downward.mv1").c_str()));
	zombieAnimModelIds_.emplace_back(MV1LoadModel((enePas + "Standing Walk Back.mv1").c_str()));
	zombieAnimModelIds_.emplace_back(MV1LoadModel((enePas + "Zombie HitIdle.mv1").c_str()));
	zombieAnimModelIds_.emplace_back(MV1LoadModel((enePas + "Zombie Dying.mv1").c_str()));

	for (int i = 0; i < ZOMBIE_NUM; i++)
	{
		// 先にメモリ確保しておく(ゲーム途中にnewを行わないようにする)
		auto enemy = new Zombie(ENEMY_TYPE::ZOMBIE, enemyModelIds_[static_cast<int>(ENEMY_TYPE::ZOMBIE)], -1, zombieAnimModelIds_, player_);
		AddEnemy(enemy);
	}

	//// 攻撃エフェクト用のモデルのロード
	//attackEffectModelIds_.emplace_back(
	//	MV1LoadModel(
	//		(Application::PATH_MODEL + "Effect/Fireball/Fireball.mv1").c_str()));
	//attackEffectModelIds_.emplace_back(
	//	MV1LoadModel(
	//		(Application::PATH_MODEL + "Effect/Rockfall/Rock.mv1").c_str()));
}

void EnemyManager::Update(void)
{
	// エネミーの更新
	for (auto& enemy : enemies_)
	{
		enemy->Update();
	}
}

void EnemyManager::Draw(void)
{

	DrawFormatString(0, 200, 0xffffff, "敵の総数 = %d", enemies_.size());

	for (auto& enemy : enemies_)
	{
		enemy->Draw();
	}
}

void EnemyManager::Delete(void)
{
	// Enemyクラスのメモリ解放
	for (auto& enemy : enemies_)
	{
		enemy->Release();
	}

	//// ベースとなるモデルのメモリ解放
	//for (int id : enemyModelIds_)
	//{
	//	MV1DeleteModel(id);
	//}

	enemies_.clear();

	// エネミーモデルの解放を追加
	for (auto& modelId : enemyModelIds_)
	{
		MV1DeleteModel(modelId);
	}

	enemyModelIds_.clear();
	zombieAnimModelIds_.clear();
}

void EnemyManager::Spawn(ENEMY_TYPE type, VECTOR pos)
{
	// 有効な敵を取得する
	EnemyBase* enemy = GetValidEnemy(type);

	if (enemy == nullptr)
	{
		// 使われていない敵がいなかったら復活処理は行わない
		return;
	}

	// 敵の初期化
	enemy->CreateEnemy(pos);
}

EnemyBase* EnemyManager::GetValidEnemy(ENEMY_TYPE type)
{
	auto& ins = EnemyManager::GetInstance();
	auto& enemies_ = ins.GetEnemy();

	size_t size = enemies_.size();

	for (int i = 0; i < size; i++)
	{
		// 敵の種類が違ったら次の敵を見る
		if (enemies_[i]->GetType() != type)
		{
			continue;
		}

		// 魔法の種別が同じ、かつ、未使用(生存していない)なら再利用する
		if (!enemies_[i]->GetEnemy().isAlive_)
		{
			return enemies_[i];
		}
	}

	return nullptr;
}

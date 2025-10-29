#include <DxLib.h>
#include "../../Application.h"
#include "../../Scene/SceneManager.h"
#include "Zombie.h"
#include "EnemyManager.h"

EnemyManager* EnemyManager::instance_ = nullptr;

EnemyManager::EnemyManager(void) {}
EnemyManager::~EnemyManager(void) {}

void EnemyManager::AddEnemy(EnemyBase* enemy)
{
	enemys_.emplace_back(std::move(enemy));
}

void EnemyManager::Load(void)
{
	// エネミーモデルのロード
	enemyModelIds_.emplace_back(MV1LoadModel((Application::PATH_MODEL + "Enemy/Zombie.mv1").c_str()));

	//enemyModelIds_.emplace_back(
	//	MV1LoadModel((Application::PATH_MODEL + "Enemy/Wizard.mv1").c_str()));
	//enemyModelIds_.emplace_back(
	//	MV1LoadModel((Application::PATH_MODEL + "Enemy/Giant.mv1").c_str()));

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

	// エネミーモデルの解放を追加
	for (auto& modelId : enemyModelIds_)
	{
		MV1DeleteModel(modelId);
	}

	enemyModelIds_.clear();
}

void EnemyManager::Spawn(ENEMY_TYPE type, VECTOR pos)
{
	// 有効な敵を取得する
	EnemyBase* enemy = GetValidEnemy(type);
	// 敵の初期化
	enemy->CreateEnemy(pos);
}

EnemyBase* EnemyManager::GetValidEnemy(ENEMY_TYPE type)
{
	auto& ins = EnemyManager::GetInstance();
	auto& enemys_ = ins.GetEnemy();

	size_t size = enemys_.size();

	for (int i = 0; i < size; i++)
	{
		// 魔法の種別が同じ、かつ、未使用(生存していない)なら再利用する
		if (!enemys_[i]->GetEnemy().isAlive_)
		{
			return enemys_[i];
		}

		// 敵の種類が違ったら次の敵を見る
		if (enemys_[i]->GetType() != type)
		{
			continue;
		}
	}

	// 未使用の敵がいなかった場合新しい敵を生成
	EnemyBase* enemy = nullptr;

	// 新しい敵のインスタンスを生成する
	switch (type)
	{
	case ENEMY_TYPE::ZOMBIE:
		enemy = new Zombie(type, enemyModelIds_[static_cast<int>(ENEMY_TYPE::ZOMBIE)], -1, player_);
		break;
	case ENEMY_TYPE::WIZARD:
		break;
	case ENEMY_TYPE::GIANT:
		break;
	case ENEMY_TYPE::MAX:
		break;
	default:
		break;
	}

	// nullチェック
	if (enemy == nullptr)
	{
		return nullptr;
	}

	// 可変長配列に追加
	ins.AddEnemy(enemy);

	return enemy;
}

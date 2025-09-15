#include "../Manager/SceneManager.h"
#include "../Object/Enemy/EnemyManager.h"
#include "../Object/Enemy/Zombie.h"
#include "WaveManager.h"
#include "WaveBase.h"

WaveBase::WaveBase(WaveManager* waveManager)
{
	waveManager_ = waveManager;

	// テーブルに関数のポインタを格納
	waveStateTable_[WAIT] = Wait;
	waveStateTable_[END] = End;
}

WaveBase::~WaveBase(void)
{
}

void WaveBase::Init(void)
{
	// ステート初期化
	ChangeWaveState(CREATE);

	// 出現数初期化
	enemyNums_.enemyNumAll_ = enemyNums_.zombieNumCnt_ = 0;

	// 出現間隔用カウンタの初期化
	enemyInterval_.zombieSpawnCnt_ = 0.0f;

	// パラメータ初期化
	SetParam();
}

void WaveBase::Update(void)
{
	// nullチェック
	if (waveStateTable_[waveState_])
	{
		waveStateTable_[waveState_](*this);
	}

	// 生成した敵の総数を更新
	enemyNums_.enemyNumAll_ = enemyNums_.zombieNumCnt_;
}

void WaveBase::Release(void)
{
}

void WaveBase::Wait(WaveBase& waveBase)
{
	waveBase.endCounter_ += SceneManager::GetInstance().GetDeltaTime();

	if (waveBase.endCounter_ >= WAVE_END_TIME)
	{
		waveBase.ChangeWaveState(END);
	}
}

void WaveBase::End(WaveBase& waveBase)
{
}

void WaveBase::EnemyCounter(EnemyBase::ENEMY_TYPE eneType)
{
	switch (eneType)
	{
	case EnemyBase::ENEMY_TYPE::ZOMBIE:

		enemyInterval_.zombieSpawnCnt_ += SceneManager::GetInstance().GetDeltaTime();

		break;
	case EnemyBase::ENEMY_TYPE::WIZARD:
		break;
	case EnemyBase::ENEMY_TYPE::GIANT:
		break;
	case EnemyBase::ENEMY_TYPE::MAX:
		break;
	default:
		break;
	}
}

void WaveBase::SpawnEnemy(EnemyBase::ENEMY_TYPE eneType, VECTOR spawnPos)
{
	switch (eneType)
	{
	case EnemyBase::ENEMY_TYPE::ZOMBIE:

		// ゾンビが規定量に達していなかったら入る
		if (enemyNums_.zombieNum_ > enemyNums_.zombieNumCnt_)
		{
			// 有効な敵を取得する
			EnemyBase* enemy = GetValidEnemy(EnemyBase::ENEMY_TYPE::ZOMBIE);
			// 敵の初期化
			enemy->CreateEnemy(spawnPos);
			// 生成したゾンビをカウントに加算
			enemyNums_.zombieNumCnt_++;

			// 間隔を初期化
			enemyInterval_.zombieSpawnCnt_ = 0.0f;
		}

		break;
	case EnemyBase::ENEMY_TYPE::WIZARD:
		break;
	case EnemyBase::ENEMY_TYPE::GIANT:
		break;
	case EnemyBase::ENEMY_TYPE::MAX:
		break;
	default:
		break;
	}
}

EnemyBase* WaveBase::GetValidEnemy(EnemyBase::ENEMY_TYPE eneType)
{
	auto& ins = EnemyManager::GetInstance();
	auto& enemys_ = ins.GetEnemy();

	size_t size = enemys_.size();

	for (int i = 0; i < size; i++)
	{
		// 敵の種類が違ったら次の敵を見る
		if (enemys_[i]->GetType() != eneType)
		{
			continue;
		}

		// 弾の種別が同じ、かつ、未使用(生存していない)なら再利用する
		if (!enemys_[i]->GetEnemy().isAlive_)
		{
			return enemys_[i];
		}
	}

	// 未使用の敵がいなかった場合新しい敵を生成
	EnemyBase* enemy = nullptr;

	auto* player = waveManager_->GetPlayerPoint();

	// 新しい敵のインスタンスを生成する
	switch (eneType)
	{
	case EnemyBase::ENEMY_TYPE::ZOMBIE:
		enemy = new Zombie(eneType, waveManager_->GetEnemyModelIds(static_cast<int>(eneType)), -1, player);
		break;
	case EnemyBase::ENEMY_TYPE::WIZARD:
		break;
	case EnemyBase::ENEMY_TYPE::GIANT:
		break;
	case EnemyBase::ENEMY_TYPE::MAX:
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

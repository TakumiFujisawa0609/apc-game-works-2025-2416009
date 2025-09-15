#include "Wave1.h"

Wave1::Wave1(WaveManager* waveManager) : WaveBase(waveManager)
{

	// テーブルに関数のポインタを格納
	waveStateTable_[CREATE] = Create;
}

Wave1::~Wave1(void)
{
}

void Wave1::Load(void)
{
}

void Wave1::Draw(void)
{
	DrawString(0, 20, "Wave1", 0xffffff);
}

void Wave1::SetParam(void)
{
	enemyNums_.zombieNum_ = ZOMBIE_NUM;
}

void Wave1::Create(WaveBase& waveBase)
{
	auto zombie = EnemyBase::ENEMY_TYPE::ZOMBIE;

	// スポーン可能時間ではなかったら、
	if (waveBase.GetEnemyInterval().zombieSpawnCnt_ < ZOMBIE_INTERVAL)
	{
		// スポーンカウンタを動かす
		waveBase.EnemyCounter(zombie);
	}
	else
	{
		// スピーン可能時間だったらゾンビを生成する
		waveBase.SpawnEnemy(zombie,VGet(0.0f, 0.0f, 100.0f));
	}

	// 敵の数が規定量になったら出現処理を終了させる
	if (waveBase.GetEnemyNum().enemyNumAll_ >= ENEMY_NUM_ALL)
	{
		waveBase.ChangeWaveState(WAIT);
	}
}

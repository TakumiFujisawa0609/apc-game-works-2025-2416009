#pragma once

#include "../Object/Enemy/EnemyBase.h"
#include "WaveState.h"
class EnemyBase;
class WaveManager;

class WaveBase
{
public:

	// ウェーブ終了までの時間
	static constexpr float WAVE_END_TIME = 5.0f;

	// コンストラクタ
	WaveBase(WaveManager* waveManager);

	// デストラクタ
	~WaveBase(void);

	virtual void Init(void);
	virtual void Load(void) = 0;
	virtual void Update(void);
	virtual void Draw(void) = 0;
	virtual void Release(void);

	WAVE_STATE GetWaveState(void)const { return waveState_; }

	ENEMY_NUMS GetEnemyNum(void) { return enemyNums_; }
	ENEMY_INTERVAL GetEnemyInterval(void) { return enemyInterval_; }

	// ウェーブの変更を行う
	void ChangeWaveState(WAVE_STATE waveState) { waveState_ = waveState; }

	// 敵のスポーンカウンタを動かす
	void EnemyCounter(EnemyBase::ENEMY_TYPE eneType);
	// 敵をスポーンさせる
	void SpawnEnemy(EnemyBase::ENEMY_TYPE eneType, VECTOR spawnPos);

protected:
	// ウェーブマネージャのインスタンスポインタ
	WaveManager* waveManager_;

	// ステート
	WAVE_STATE waveState_;

	// 状態のテーブル
	WaveStateFunction waveStateTable_[WAVE_STATE_MAX];

	// 出現させる敵の数
	ENEMY_NUMS enemyNums_;

	// 敵の出現間隔
	ENEMY_INTERVAL enemyInterval_;

	// 終了後に次に遷移させる条件用カウンター
	float endCounter_;

	static void Wait(WaveBase& waveBase);
	static void End(WaveBase& waveBase);

	// 使用する敵のインスタンスを探す
	EnemyBase* GetValidEnemy(EnemyBase::ENEMY_TYPE eneType);

	// パラメータ設定
	virtual void SetParam(void) = 0;
};
#pragma once

#include "WaveBase.h"
#include "WaveState.h"
#include "../Object/Enemy/EnemyBase.h"

class Wave1 : public WaveBase
{
public:
	// そのウェーブで出現させる総数数
	static constexpr int ENEMY_NUM_ALL = 5;

	// ゾンビを出現させる総数
	static constexpr int ZOMBIE_NUM = 5;

	// ゾンビを出現させる総数
	static constexpr float ZOMBIE_INTERVAL = 2.0f;

	Wave1(WaveManager* waveManager);
	~Wave1(void);

	void Load(void)override;
	void Draw(void)override;
protected:

	void SetParam(void)override;

private:
	static void Create(WaveBase& waveBase);
};


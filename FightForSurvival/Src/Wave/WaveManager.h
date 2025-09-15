#pragma once

#include <vector>
#include "WaveState.h"

class WaveBase;
class Player;

class WaveManager
{
public:

	// コンストラクタ・デストラクタ
	WaveManager(Player* player);
	~WaveManager(void);

	void Init(void);
	void Load(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	void ChangeWave(WAVE wave);

	// ゲッター関数
	WAVE GetNowWave(void)const { return wave_; }
	int GetEnemyModelIds(int i)const { return enemyModelIds_[i]; }
	Player* GetPlayerPoint(void)const { return player_; }

private:
	
	// プレイヤーのインスタンスポインタ
	Player* player_;

	// ウェーブのインスタンスポインタ
	WaveBase* wave1_;
	WaveBase* wave2_;

	// 現在のウェーブ
	WAVE wave_;

	// 状態のテーブル
	WaveFunction waveTable_[WAVE_MAX][PROCESS_MAX];

	// エネミー用のモデルハンドルID
	std::vector<int> enemyModelIds_;

	static void Wave1Update(WaveManager& wave);
	static void Wave2Update(WaveManager& wave);
	static void SelectUpdate(WaveManager& wave);
	static void EndUpdate(WaveManager& wave);

	static void Wave1Draw(WaveManager& wave);
	static void Wave2Draw(WaveManager& wave);
	static void SelectDraw(WaveManager& wave);
	static void EndDraw(WaveManager& wave);


};


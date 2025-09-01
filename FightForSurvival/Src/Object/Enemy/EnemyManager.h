#pragma once

#include <vector>
#include "EnemyBase.h"
class EnemyBase;
class Player;

class EnemyManager
{
public:

	// ウェーブが進む条件フレーム数
	static constexpr float NEXT_WAVE_FRAME = 60.0f;

	// スポーン間隔
	static constexpr float ZOMBIE_SPAWN_INTERVAL = 1.5f;

	// スポーンさせる数
	static constexpr int WAVE01_ZOMBIE_SPAWN_NUM = 5;

	static constexpr int WAVE02_ZOMBIE_SPAWN_NUM = 10;

	// 敵の生成ウェーブ
	enum class WAVE
	{
		WAVE01,
		WAVE02,
		WAVE03,
		WAIT,
		END
	};

	// コンストラクタ
	EnemyManager(Player* player);
	// デストラクタ
	~EnemyManager(void);

	void Init(void);
	void Load(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	std::vector<EnemyBase*> GetEnemys(void)const { return enemys_; }

	// 今現在のウェーブを確認する
	WAVE GetNowWave(void)const { return wave_; }
	WAVE GetNextWave(void)const { return nextWave_; }

private:

	// エネミー用のモデルハンドルID
	std::vector<int> enemyModelIds_;

	// 攻撃エフェクト用のモデルハンドルID
	//std::vector<int> attackEffectModelIds_;

	// エネミー
	std::vector<EnemyBase*> enemys_;

	EnemyBase::ENEMY_TYPE enemyType_;

	// プレイヤーのポインタ
	Player* player_;

	WAVE wave_;
	WAVE nextWave_;

	float zombieTime_;

	int zombieNum_;

	float frameNum_;

	// 待ちタイムか
	bool isWait_;

	void Collision(void);

	void ChangeWave(WAVE wave);
	void UpdateWave01(void);
	void UpdateWave02(void);
	//void UpdateWave03(void);
	void UpdateEnd(void);
};


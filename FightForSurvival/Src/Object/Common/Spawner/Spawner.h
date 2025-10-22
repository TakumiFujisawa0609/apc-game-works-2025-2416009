#pragma once

#include <DxLib.h>
#include "../../Enemy/EnemyManager.h"

class Spawner
{
public:

	// 出現させる敵の数
	static constexpr int SPAWN_ENEMY_NUM = 5;

	// 軸座標から移動させる量(ローカル)
	static constexpr VECTOR LEFT_UP = {-100.0f,0.0f,100};
	static constexpr VECTOR LEFT_DOWN = { -100.0f,0.0f,-100 };
	static constexpr VECTOR RIGHT_UP = { 100.0f,0.0f,100 };
	static constexpr VECTOR RIGHT_DOWN = { 100.0f,0.0f,-100 };
	
	// 生成する敵の種類のパターン
	enum class PATTERN
	{
		PATTERN_1,
		PATTERN_2,
		PATTERN_3,

		MAX,
	};

	// コンストラクタ・デストラクタ
	Spawner(void);
	~Spawner(void);
	 
	// スポナー生成処理
	void Create(VECTOR pos, float interval);
	// 更新処理
	void Update(void);
	// 描画処理
	void Draw(void);
	// 解放処理
	void Release(void);

private:

	// 座標
	VECTOR pos_[SPAWN_ENEMY_NUM];

	// スポーンさせる間隔
	float spawnInterval_;

	// 存在フラグ(true/ 存在している false/ 存在していない)
	bool isExists_;

	// 実際に経った時間
	float time_;

	// 敵の出現パターン
	PATTERN pattern_;

	// エネミーのタイプ
	ENEMY_TYPE eneType_[SPAWN_ENEMY_NUM];

	// パターン決め(ランダム)
	void SelectPattern(void);
	// パターンごとに座標を変更する
	void PatternInsInit(PATTERN pattern);

	// 座標初期化
	void PositionInit(void);
};


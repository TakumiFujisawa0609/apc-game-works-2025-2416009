#pragma once

#include <DxLib.h>

enum class ENEMY_TYPE;

class Spawner
{
public:

	// 出現させる敵の数
	static constexpr int SPAWN_ENEMY_NUM = 4;
	static constexpr float COLLISION_RADIUS = 40.0f;
	static constexpr float DURABILITY = 3.0f;

	// 軸座標から移動させる量(ローカル)
	static constexpr VECTOR LEFT_UP = {-300.0f,0.0f,300.0f };
	static constexpr VECTOR LEFT_DOWN = { -300.0f,0.0f,-300.0f };
	static constexpr VECTOR RIGHT_UP = { 300.0f,0.0f,300.0f };
	static constexpr VECTOR RIGHT_DOWN = { 300.0f,0.0f,-300.0f };
	
	// 生成する敵の種類のパターン
	enum class PATTERN
	{
		PATTERN_1,
		PATTERN_2,
		PATTERN_3,

		MAX,
	};

	struct SpawnerIns
	{
		int modelId_;

		VECTOR basePos_;

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

		// 当たり判定用半径
		float collisionRadius_;

		// 耐久力
		float durability_;

	};

	// コンストラクタ・デストラクタ
	Spawner(int modelId);
	~Spawner(void);
	 
	// スポナー生成処理
	void Create(VECTOR pos, float interval);
	// 更新処理
	void Update(void);
	// 描画処理
	void Draw(void);
	// 解放処理
	void Release(void);

	// 耐久力にダメージを与える
	void Damage(float durability);

	// スポナーの情報を渡す
	const SpawnerIns& GetSpawnerIns(void)const { return spawner_; }

private:

	SpawnerIns spawner_;

	// パターン決め(ランダム)
	void SelectPattern(void);
	// パターンごとに座標を変更する
	void PatternInsInit(PATTERN pattern);

	// 座標初期化
	void PositionInit(void);
	// 基軸から座標を設定
	void SetPosition(int i,VECTOR offset);
};


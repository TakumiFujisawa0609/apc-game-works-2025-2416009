#pragma once

#include <DxLib.h>

enum class ENEMY_TYPE;

class Spawner
{
public:

	// 出現させる敵の数
	static constexpr int SPAWN_ENEMY_NUM = 4;

	// 当たり判定半径
	static constexpr float COLLISION_RADIUS = 400.0f;

	// スポナーの耐久値
	static constexpr float DURABILITY = 15.0f;

	// 軸座標から移動させる量(ローカル)
	static constexpr VECTOR LEFT_UP = {-200.0f,0.0f,200.0f };
	static constexpr VECTOR LEFT_DOWN = { -200.0f,0.0f,-200.0f };
	static constexpr VECTOR RIGHT_UP = { 200.0f,0.0f,200.0f };
	static constexpr VECTOR RIGHT_DOWN = { 200.0f,0.0f,-200.0f };

	// エフェクトの大きさ
	static constexpr float EFFECT_SCALE = 400.0f;

	// 生成する敵の種類のパターン(全て4体ずつ)
	enum class PATTERN
	{
		PATTERN_1,	// 全てゾンビ
		PATTERN_2,	// 全てコウモリ
		PATTERN_3,	// ゾンビとコウモリの半々

		MAX,
	};

	struct SpawnerIns
	{
		// エフェクトのサイズ
		float effectScale_;

		// 基準座標
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

		// スポナーの耐久力
		float durability_;

		// 再生中のエフェクトハンドルID
		int playEffectId_;
	};

	// コンストラクタ
	Spawner();
	// デストラクタ
	~Spawner(void);
	 
	// スポナー生成処理
	void Create(VECTOR pos, float interval, PATTERN pattern);
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

	// スポナーの情報
	SpawnerIns spawner_;

	// パターン決め(指定)
	void SelectPattern(PATTERN pattern);

	// パターンごとに座標を変更する
	void PatternInsInit(PATTERN pattern);

	// 座標初期化
	void PositionInit(void);

	// 基軸から座標を設定
	void SetPosition(int i,VECTOR offset);
};


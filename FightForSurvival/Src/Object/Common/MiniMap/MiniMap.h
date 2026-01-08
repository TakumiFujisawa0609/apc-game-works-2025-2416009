#pragma once

#include <DxLib.h>
#include <vector>
#include "../../../Common/Vector2/Vector2.h"

class MiniMap
{
public:

	MiniMap(VECTOR& plaPos);
	~MiniMap(void);

	void Load(void);
	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

private:

	// ミニマップの描画位置
	static constexpr int MINI_MAP_POS_X = 100.0f;
	static constexpr int MINI_MAP_POS_Y = 100.0f;

	// ミニマップのサイズ
	static constexpr int MINI_MAP_SIZE_XY = 128;

	// 描画物の半径
	static constexpr int OBJECT_RADIUS = 2;

	// ステージ自体の大きさ
	int stageModelSizeX_;
	int stageModelSizeZ_;

	// ミニマップの画像ハンドル
	int miniMapImgHandle_;

	// ミニマップを映す位置
	Vector2 miniMapPos;

	// プレイヤー座標
	VECTOR& playerPos_;
	// ミニマップ用のプレイヤー座標(実際に描画に使用する位置情報)
	VECTOR miniMapPlayerPos_;

	// 敵座標(ミニマップに映すのが確定した敵)
	std::vector<VECTOR*> enemiesPos_;
	// ミニマップ用の敵座標(実際に描画に使用する位置情報)
	std::vector<Vector2> miniMapEnemiesPos_;

	// 描画物の半径
	int objectRadis_;
};


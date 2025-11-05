#pragma once

#include <DxLib.h>

enum class TYPE_MAGIC
{
	FIRE_MAGIC,
	WIND_MAGIC,
	EXPLOSION_MAGIC
};

struct Magic
{
	// 魔法のモデルID
	int modelId_;
	// 方向
	VECTOR dir_;
	// 魔法の大きさ
	VECTOR scale_;
	// 魔法の回転
	VECTOR rotate_;
	// 魔法の座標
	VECTOR pos_;
	VECTOR prevPos_;
	// 魔法の移動速度
	float speed_;
	// 魔法の生存判定
	bool isAlive_;
	// 魔法の描画判定
	bool isDraw_;
	// 魔法の生存期間
	float cntAlive_;
	// 魔法の衝突判定用半径
	float collisionRadius_;
	// ダメージ数
	float headDamage_;
	float bodyDamage_;
};
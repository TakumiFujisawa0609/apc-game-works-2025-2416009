#pragma once

#include <DxLib.h>

enum class TYPE_MAGIC
{
	STRAIGHT_MAGIC, // まっすぐ飛ぶ炎魔法
	CHASE_MAGIC,	// 一番近くの敵を追尾する風魔法
	EXPLOSION_MAGIC,	// 着弾時に周囲を爆発で巻き込む爆発魔法

	BAT_MAGIC, // コウモリの魔法
	DRAGON_MAGIC // ドラゴンの魔法
};

// 魔法の状態
enum class MAGIC_STATE
{
	CHARGE,
	SHOT,
	BLAST,
	END
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
	// 魔法の種類
	TYPE_MAGIC typeMagic_;
	// 魔法の移動速度
	float speed_;
	// 魔法の存在判定
	bool isExists_;
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
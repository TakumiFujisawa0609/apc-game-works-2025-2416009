#pragma once

#include <DxLib.h>
#include "Common/Vector2.h"

struct Unit
{
	// モデルのハンドルId
	int modelId_;

	// 座標
	VECTOR pos_;

	// 移動前座標
	VECTOR prevPos_;

	// 加速度、重力
	float moveSpeed_;

	// モデルの向き
	VECTOR angles_;

	// モデルの大きさ
	VECTOR scales_;

	// 向き
	VECTOR moveDir_;

	// 生存判定
	bool isAlive_;

	// 体力
	int hp_;

	// ダメージを受けたか true = 受けた / false = 受けてない
	bool isDamaged_;
};

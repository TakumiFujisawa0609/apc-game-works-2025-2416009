#pragma once

#include <DxLib.h>
#include "Common/Vector2/Vector2.h"

struct Unit
{
	// モデルのハンドルId
	int modelId_;

	// 座標
	VECTOR pos_;

	// 移動前座標
	VECTOR prevPos_;

	// モデルの向き
	VECTOR angle_;

	// モデルの大きさ
	VECTOR scales_;

	// 向き
	VECTOR dir_;

	// 重力
	VECTOR velocity_;

	// 体力
	float hp_;

	// 速度
	float moveSpeed_;

	// 生存判定
	bool isAlive_;

	// ダメージを受けたか true = 受けた / false = 受けてない
	bool isDamaged_;

	// 衝突判定用半径
	float collisionRadius_;
	float collisionRadiusHead_;
	float collisionRadiusBody_;
	float collisionRadiusArm_;
	float collisionRadiusHand_;
	float collisionRadiusLeg_;
};

struct Ability
{
	// 最大HP
	float hpMax_;

	// スタミナ
	float stamina_;
	float staminaMax_;
};


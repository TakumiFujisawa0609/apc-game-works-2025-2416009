#pragma once

#include <DxLib.h>
#include "Common/Vector2/Vector2.h"

// プレイヤーや敵など、ゲーム内のキャラクターの基本情報を保持する
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

	// 生存判定 true / 生存中 , false / 死亡
	bool isAlive_;

	// ダメージを受けたか true / 受けた , false / 受けてない
	bool isDamaged_;

	// ジャンプ判定(true / ジャンプ中、false / ジャンプ中ではない)
	bool isJump_;

	// 実際にかかっている重力
	float gravity_;

	// 衝突判定用半径
	float collisionRadius_;			// 全体の当たり判定
	float collisionRadiusHead_;		// 頭
	float collisionRadiusBody_;		// 体
	float collisionRadiusArm_;		// 腕
	float collisionRadiusHand_;		// 手
	float collisionRadiusLeg_;		// 足
};

// プレイヤーの能力値
struct Ability
{
	// 最大HP
	float hpMax_;

	// スタミナ
	float stamina_;
	float staminaMax_;
};


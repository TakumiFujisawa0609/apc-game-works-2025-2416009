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

	// モデルの向き
	VECTOR angles_;

	// モデルの大きさ
	VECTOR scales_;

	// 向き
	VECTOR dir_;

	// 重力
	VECTOR velocity_;

	// 体力
	int hp_;

	// 速度
	float moveSpeed_;

	// 生存判定
	bool isAlive_;

	// ダメージを受けたか true = 受けた / false = 受けてない
	bool isDamaged_;

	// 衝突判定用半径
	float collisionRadius_;
	float collisionRadiusBody_;
	float collisionRadiusArm_;
	float collisionRadiusHand_;
	float collisionRadiusLeg_;
};

struct Ability
{
	// 最大HP
	int hpMax_;

	// スタミナ
	float stamina_;
	float staminaMax_;
};

enum class GUN_TYPE
{
	HANDGUN,
	ASSAULT_RIFLE,
	SHOTGUN,
};

struct CollisionPos
{
	// 当たり判定用の座標
	// 頭
	VECTOR posHead_;
	// 胴体(カプセル型なので上側と下側の座標)
	VECTOR posBodyTop_;
	VECTOR posBodyUnder_;
	// 右腕(カプセル型なので上側と下側の座標)
	VECTOR posArmTopR_;
	VECTOR posArmUnderR_;
	// 左腕(カプセル型なので上側と下側の座標)
	VECTOR posArmTopL_;
	VECTOR posArmUnderL_;
	// 右手
	VECTOR posHandR_;
	// 左手
	VECTOR posHandL_;
	// 右腕(カプセル型なので上側と下側の座標)
	VECTOR posLegTopR_;
	VECTOR posLegUnderR_;
	// 左腕(カプセル型なので上側と下側の座標)
	VECTOR posLegTopL_;
	VECTOR posLegUnderL_;

	// 実際のボーンからの座標微調整用のオフセット(Y軸用)
	// 頭
	float offsetHead_;
	// 体(カプセル型なので上側と下側の座標)
	float offsetBodyTop_;
	float offsetBodyUnder_;
	// 腕(カプセル型なので上側と下側の座標)
	float offsetArmTop_;
	float offsetArmUnder_;
	// 手
	float offsetHand_;
	// 脚(カプセル型なので上側と下側の座標)
	float offsetLegTop_;
	float offsetLegUnder_;

	// モデルについている部分ボーンの格納変数
	// 頭
	int headBone_;
	// 体(カプセル型なので上側と下側のボーン)
	int bodyBoneTop_;
	int bodyBoneUnder_;
	// 右腕(カプセル型なので上側のボーン、下側は手から持ってくる)
	int armBoneTopR_;
	// 左腕(カプセル型なので上側のボーン、下側は手から持ってくる)
	int armBoneTopL_;
	// 右手
	int handBoneR_;
	// 左手
	int handBoneL_;
	// 右脚
	int legBoneTopR_;
	int legBoneUnderR_;
	// 左脚
	int legBoneTopL_;
	int legBoneUnderL_;

};

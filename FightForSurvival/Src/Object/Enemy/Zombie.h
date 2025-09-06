#pragma once

#include "EnemyBase.h"

class Zombie : public EnemyBase
{
public:

	// モデルのアングル
	static constexpr VECTOR ANGLE = {0.0f,0.0f,0.0f};

	// モデルの大きさ
	static constexpr VECTOR SCALE = { 1.0f,1.0f,1.0f };

	// 体力
	static constexpr int HP = 10;

	// 速度
	static constexpr float SPEED = 3.0f;

	// 撃破スコア
	static constexpr int SCORE = 10;

	// 当たり判定用半径
	// 頭
	static constexpr float COLLISION_RADIUS = 15.0f;
	// 体
	static constexpr float COLLISION_RADIUS_BODY = 20.0f;
	// 腕
	static constexpr float COLLISION_RADIUS_ARM = 6.0f;
	// 手
	static constexpr float COLLISION_RADIUS_HAND = 10.0f;
	// 脚
	static constexpr float COLLISION_RADIUS_LEG = 10.0f;

	// モデルと頭の相対座標
	static constexpr float OFFSET_POS_HEAD = 10.0f;

	// モデルと体の相対座標(上)
	static constexpr float OFFSET_POS_BODY_TOP = -2.0f;
	// モデルと体の相対座標(下)
	static constexpr float OFFSET_POS_BODY_UNDER = 12.0f;

	// モデルと腕の相対座標(上)
	static constexpr float OFFSET_POS_ARM_TOP = -2.0f;
	// モデルと腕の相対座標(下)
	static constexpr float OFFSET_POS_ARM_UNDER = 5.0f;

	// モデルと手の相対座標
	static constexpr float OFFSET_POS_HAND = -10.0f;

	// モデルと体の相対座標(上)
	static constexpr float OFFSET_POS_LEG_TOP = -12.0f;
	// モデルと体の相対座標(下)
	static constexpr float OFFSET_POS_LEG_UNDER = -5.0f;

	Zombie(void);
	~Zombie(void)override;

protected:

	// パラメータ設定
	void SetParam(void)override;
	// アニメーション登録
	void AddAnimation(void)override;

private:

	// 待機処理
	static void Idle(EnemyBase& enemy);
	// 攻撃処理
	static void Attack(EnemyBase& enemy);

};


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

	// 当たり判定用半径
	static constexpr float COLLISION_RADIUS = 15.0f;
	static constexpr float COLLISION_RADIUS_BODY = 20.0f;

	// モデルと頭の相対座標
	static constexpr float RELATIVE_POS_HEAD = 165.0f;

	// モデルと体の相対座標
	static constexpr float RELATIVE_POS_BODY_TOP = 125.0f;

	// モデルと体の相対座標
	static constexpr float RELATIVE_POS_BODY_UNDER = 5.0f;

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


#pragma once

#include "../Base/EnemyBase.h"
#include "../EnemyManager.h"

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
	static constexpr float SPEED = 4.0f;

	// 撃破スコア
	static constexpr int SCORE = 100;

	// 当たり判定用半径
	// 押し出し用
	static constexpr float COLLISION_RADIUS = 50.0f;
	// 頭
	static constexpr float COLLISION_RADIUS_HEAD = 15.0f;
	// 体
	static constexpr float COLLISION_RADIUS_BODY = 30.0f;
	// 腕
	static constexpr float COLLISION_RADIUS_ARM = 6.0f;
	// 手
	static constexpr float COLLISION_RADIUS_HAND = 15.0f;

	// モデルと頭の相対座標
	static constexpr float OFFSET_POS_HEAD = 10.0f;

	// モデルと体の相対座標(上)
	static constexpr float OFFSET_POS_BODY_TOP = -2.0f;
	// モデルと体の相対座標(下)
	static constexpr float OFFSET_POS_BODY_UNDER = -60.0f;

	// モデルと腕の相対座標(上)
	static constexpr float OFFSET_POS_ARM_TOP = -2.0f;
	// モデルと腕の相対座標(下)
	static constexpr float OFFSET_POS_ARM_UNDER = 5.0f;

	// モデルと手の相対座標
	static constexpr float OFFSET_POS_HAND = 15.0f;

	// 攻撃可能範囲
	static constexpr float ATTACK_RANGE = 75;

	// 攻撃待ち時間
	static constexpr float ATTACK_COOLDOWN = 3.0f;

	Zombie(ENEMY_TYPE type, int baseModelId, int baseAttackEffectModelId, std::vector<int> animModelIds, Player* player);
	~Zombie(void)override;

protected:

	// パラメータ設定
	void SetParam(void)override;
	// フレーム登録
	void AddFrames(void)override;

private:

	// 待機処理
	static void Idle(EnemyBase& enemy);
	// 攻撃処理
	static void Attack(EnemyBase& enemy);

};


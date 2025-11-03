#pragma once

#include "../Base/EnemyBase.h"
#include "../EnemyManager.h"

class Bat : public EnemyBase
{
public:

	// モデルのアングル
	static constexpr VECTOR ANGLE = { 0.0f,0.0f,0.0f };

	// モデルの大きさ
	static constexpr VECTOR SCALE = { 0.4f,0.4f,0.4f };

	// 体力
	static constexpr int HP = 10;

	// 速度
	static constexpr float SPEED = 4.0f;

	// 撃破スコア
	static constexpr int SCORE = 50;

	// 当たり判定用半径
	// 押し出し用
	static constexpr float COLLISION_RADIUS = 50.0f;
	// 頭
	static constexpr float COLLISION_RADIUS_HEAD = 40.0f;
	// 体
	static constexpr float COLLISION_RADIUS_BODY = 30.0f;
	// 腕
	static constexpr float COLLISION_RADIUS_ARM = 25.0f;
	// 手
	static constexpr float COLLISION_RADIUS_HAND = 20.0f;

	// モデルと頭の相対座標
	static constexpr float OFFSET_POS_HEAD = 40.0f;

	// モデルと体の相対座標(上)
	static constexpr float OFFSET_POS_BODY_TOP = -20.0f;
	// モデルと体の相対座標(下)
	static constexpr float OFFSET_POS_BODY_UNDER = -20.0f;

	// モデルと腕の相対座標(上)
	static constexpr float OFFSET_POS_ARM_TOP = 30.0f;
	// モデルと腕の相対座標(下)
	static constexpr float OFFSET_POS_ARM_UNDER = -50.0f;

	// モデルと手の相対座標
	static constexpr float OFFSET_POS_HAND = 5.0f;

	// 攻撃可能範囲
	static constexpr float ATTACK_RANGE = 1200.0f;

	// 攻撃待ち時間
	static constexpr float ATTACK_COOLDOWN = 3.0f;

	Bat(ENEMY_TYPE type, int baseModelId, int baseAttackEffectModelId, std::vector<int> animModelIds, Player* player);
	~Bat(void)override;

protected:

	// パラメータ設定
	void SetParam(void)override;
	// フレーム登録
	void AddFrames(void)override;

	// ステート別アニメーション再生
	void PlayAnim(void)override;
private:

	// 待機処理
	static void Idle(EnemyBase& enemy);
	// 攻撃処理
	static void Attack(EnemyBase& enemy);


};


#pragma once

#include "../Base/EnemyBase.h"
#include "../EnemyInfo.h"

class Dragon : public EnemyBase
{
public:

	// モデルのアングル
	static constexpr VECTOR ANGLE = { 0.0f,0.0f,0.0f };

	// モデルの大きさ
	static constexpr VECTOR SCALE = { 3.0f,3.0f,3.0f };

	// 体力
	static constexpr int HP = 100;

	// 速度
	static constexpr float SPEED = 4.0f;

	// 撃破スコア
	static constexpr int SCORE = 10000;

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

	// ランダム用確率数値
	static constexpr int RANDOM_NUM = 10000;
	static constexpr int RANGE = 4000;
	static constexpr int FORWARD = 7000;
	static constexpr int RUSH = 10000;

	// 敵の状態（ボスの攻撃）
	enum DRAGON_ATTACK_STATE
	{
		SELECT,				// 攻撃選択
		RANGE_ATTACK,		// 範囲攻撃
		FORWARD_ATTACK,     // 前方攻撃
		RUSH_ATTACK,		// 突進攻撃
		ATTACK_END,         // 攻撃終了

		DRAGON_ATTACK_STATE_MAX,    // 敵の全状態
	};

	Dragon(ENEMY_TYPE type, int baseModelId, int baseAttackEffectModelId, std::vector<int> animModelIds, Player* player);
	~Dragon(void)override;

	void ChangeAttackState(DRAGON_ATTACK_STATE state);

protected:

	// パラメータ設定
	void SetParam(void)override;
	// フレーム登録
	void AddFrames(void)override;

	// ステート別アニメーション再生
	void PlayAnim(void)override;

private:

	DRAGON_ATTACK_STATE attackState_;
	enemieAttackStateFunction attackStateTable_[DRAGON_ATTACK_STATE_MAX];

	// 待機処理
	static void Idle(EnemyBase& enemy);

	// 攻撃処理
	static void Attack(EnemyBase& enemy);

	// 攻撃選択
	static void AttackSelect(Dragon& dragon);
	// 範囲攻撃
	static void RangeAttack(Dragon& dragon);
	// 前方攻撃
	static void ForwardAttack(Dragon& dragon);
	// 突進攻撃
	static void RushAttack(Dragon& dragon);
	// 攻撃終了
	static void AttackEnd(Dragon& dragon);

	// 攻撃時のステート別アニメーション再生
	void AttackPlayAnim(void);

	// 規定の位置から動いていたら、戻る処理を行う
	void ReturnPositon(void);
};


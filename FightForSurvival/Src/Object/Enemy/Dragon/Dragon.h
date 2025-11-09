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
	static constexpr int HP = 1;

	// 速度
	static constexpr float SPEED = 4.0f;

	// 撃破スコア
	static constexpr int SCORE = 10000;

	// 当たり判定用半径
	// 押し出し用
	static constexpr float COLLISION_RADIUS = 210.0f;
	// 頭
	static constexpr float COLLISION_RADIUS_HEAD = 15.0f;
	// 体
	static constexpr float COLLISION_RADIUS_BODY = 200.0f;
	// 腕
	static constexpr float COLLISION_RADIUS_ARM = 150.0f;
	// 手
	static constexpr float COLLISION_RADIUS_HAND = 150.0f;

	// モデルと頭の相対座標
	static constexpr float OFFSET_POS_HEAD = 10.0f;

	// モデルと体の相対座標(上)
	static constexpr float OFFSET_POS_BODY_TOP = 120.0f;
	// モデルと体の相対座標(下)
	static constexpr float OFFSET_POS_BODY_UNDER = -100.0f;

	// モデルと腕の相対座標(上)
	static constexpr float OFFSET_POS_ARM_TOP = -2.0f;
	// モデルと腕の相対座標(下)
	static constexpr float OFFSET_POS_ARM_UNDER = -300.0f;

	// モデルと手の相対座標
	static constexpr float OFFSET_POS_HAND = -200.0f;

	// 攻撃可能範囲
	static constexpr float ATTACK_RANGE = 75;

	// 攻撃待ち時間
	static constexpr float ATTACK_COOLDOWN = 5.0f;

	// ランダム用確率数値
	static constexpr int RANDOM_NUM = 10000;
	static constexpr int RANGE = 4000;
	static constexpr int FORWARD = 7000;
	static constexpr int RUSH = 10000;

	// 範囲魔法時の魔法の数
	static constexpr int MAGIC_NUM = 12;

	// 前方攻撃時の速度
	static constexpr float STARTING_TIME = 3.0f;

	// 突撃攻撃時の速度
	static constexpr float RUSH_SPEED = 10.0f;

	// 魔法の位置の相対座標(ローカル)
	static constexpr VECTOR RELATIVE_MAGIC_POS = { 0.0f,280.0f,-80.0f };

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

	Dragon(ENEMY_TYPE type, int baseModelId, std::vector<int> animModelIds, Player* player);
	~Dragon(void)override;

	void Release(void)override;

	void ChangeAttackState(DRAGON_ATTACK_STATE state);

	DRAGON_ATTACK_STATE GetAttackState(void)const { return attackState_; }

protected:

	// パラメータ設定
	void SetParam(void)override;
	// フレーム登録
	void AddFrames(void)override;

	// ステート別アニメーション再生
	void PlayAnim(void)override;

private:

	// 関数ポインタ用
	DRAGON_ATTACK_STATE attackState_;
	enemieAttackStateFunction attackStateTable_[DRAGON_ATTACK_STATE_MAX];

	// 魔法を保持する用(範囲攻撃用)
	std::vector<MagicBase*> magicsRange_;

	// 前方攻撃待ち時間
	float forwardAttackStartTime_;

	float rushSpeed_;

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

	void IsDrawMagicWhole(void);
	void CreateMagicWhole(void);

	// 規定の位置から動いていたら、戻る処理を行う
	void ReturnPositon(void);

	// 前方攻撃時の待ち時間を0に近づける
	void SubStartTime(void);
};


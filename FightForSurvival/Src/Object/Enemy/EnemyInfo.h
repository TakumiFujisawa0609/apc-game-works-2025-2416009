#pragma once

class EnemyBase; // 前方宣言
class Dragon;

// 状態関数型
typedef void (*enemieStateFunction)(EnemyBase&);
typedef void (*enemieAttackStateFunction)(Dragon&);

// 敵の状態（共通）
enum ENEMY_STATE
{
    STATE_IDLE,         // 待機
    STATE_CHASE,        // 追跡
    STATE_ATTACK,       // 攻撃
    STATE_RETREAT,      // 後退
    STATE_HIT,          // ダメージ
    STATE_DEAD,         // 死亡
    STATE_END,          // 完全死亡

    ENEMY_STATE_MAX,    // 敵の全状態
};

// アニメーション種別
enum class ANIM_TYPE
{
	CRAWL,
	DEATH,
	HITREACT,
	IDLE_COUNT,
	IDLE_ATTACK,
	JUMP_COUNT,
	JUMP_IDLE,
	JUMP_LAND,
	NO_COUNT,
	PUNCH_COUNT,
	RUN_COUNT,
	RUN_ARMS,
	RUN_ATTACK,
	WALK_COUNT,
	WAVE_COUNT,
	YES_COUNT,

	MAX,
};

// アニメーション種別
enum class ANIM_TYPE_FLY
{
	ATTACK,
	ATTACK_2,
	DEATH,
	FLYING,
	HIT,
	MAX,
};

// 当たり判定部位座標
enum COLLISION_POS
{
	// 頭
	HEAD,
	// 胴体(カプセル型なので上側と下側の座標)
	BODY_TOP,
	BODY_UNDER,
	// 右腕(カプセル型なので上側と下側の座標)
	ARM_TOP_R,
	ARM_UNDER_R,
	// 左腕(カプセル型なので上側と下側の座標)
	ARM_TOP_L,
	ARM_UNDER_L,
	// 右手
	HAND_R,
	// 左手
	HAND_L,

	MAX
};

struct EnemyCollision
{
	// 当たり判定用の座標
	VECTOR colPos_[COLLISION_POS::MAX];

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
};

struct enemieStateCtrl
{
	ENEMY_STATE state_;
	enemieStateFunction stateTable_[ENEMY_STATE_MAX];
};

struct EnemyMove
{
	bool isLeft_;
	float leftRightRate_;
};

struct EnemyAttack
{
	VECTOR rangePos_;
	float range_;
	float cooldown_;
	bool isAttacking_;
};

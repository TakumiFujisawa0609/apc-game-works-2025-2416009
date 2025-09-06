#pragma once

class EnemyBase; // 前方宣言

// 状態関数型
typedef void (*EnemyStateFunction)(EnemyBase&);

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
	// 右腕(カプセル型なので上側と下側の座標)
	LEG_TOP_R,
	LEG_UNDER_R,
	// 左腕(カプセル型なので上側と下側の座標)
	LEG_TOP_L,
	LEG_UNDER_L,

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
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
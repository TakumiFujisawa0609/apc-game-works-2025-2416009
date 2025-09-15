#pragma once

class WaveManager; // 前方宣言
class WaveBase; // 前方宣言

// 状態関数型
typedef void (*WaveFunction)(WaveManager&);
typedef void (*WaveStateFunction)(WaveBase&);

// ウェーブの状態
enum WAVE
{
    WAVE1,
    WAVE2, 
    SELECT, // 選択強化
    WAVE_END,

    WAVE_MAX,
};

enum PROCESS
{
    UPDATE,
    DRAW,

    PROCESS_MAX
};

enum WAVE_STATE
{
    CREATE,
    WAIT,
    END,

    WAVE_STATE_MAX,
};

struct ENEMY_NUMS
{
    // 敵全体の総数
    int enemyNumAll_;
    // ゾンビの総数
    int zombieNum_;
    // ゾンビの出現数
    int zombieNumCnt_;
};

struct ENEMY_INTERVAL
{
    float zombieSpawnCnt_;
};
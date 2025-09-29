#pragma once

#include "../Object/Enemy/EnemyBase.h"

#include <DxLib.h>

class WaveBase
{
public:
    // ウェーブの状態
    enum class WaveState
    {
        Prepare,   // 準備期間
        InWave,    // ウェーブ中
        Cleared    // クリア済み
    };

    // 敵のスポーンイベント
    struct SpawnEvent
    {
        int triggerTime;            // 何フレーム目にスポーンするのか
        ENEMY_TYPE type;            // スポーンする敵の種類
        VECTOR pos;                 // スポーン座標
        bool triggered = false;     // スポーンしたか
    };

    // コンストラクタで設定
    WaveBase(int prep, int wave);
    virtual ~WaveBase() = default;

    void Update(void);  // 更新
    void Draw(void);    // 描画

    // 派生先で固有の処理があるなら記述する。
    virtual void OnStart();       // システム起動
    virtual void OnPrepare();     // 準備期間中
    virtual void OnWave();        // ウェーブ中
    virtual void OnClear();       // クリア後

    // 派生先でクリア条件を変更できる
    virtual bool CheckWaveClear();

    // スポーンイベントを作成
    void AddSpawnEvent(int time, ENEMY_TYPE type, VECTOR pos);

    // クリアしているか？
    bool IsCleared() const { return state == WaveState::Cleared; }

protected:

    // ウェーブ状態
    WaveState state;

    int prepareTime; // 準備時間
    int waveTime;    // 戦闘時間
    int elapsed;     // 経過時間

    std::vector<SpawnEvent> spawnEvents;    // スポーンイベントを管理

};
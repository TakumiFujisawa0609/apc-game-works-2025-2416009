#pragma once

#include "../Object/Enemy/Base/EnemyBase.h"

#include <DxLib.h>

class WaveBase
{
public:
    // ウェーブの状態
    enum class WaveState
    {
        PREPARE,   // 準備期間
        INWAVE,    // ウェーブ中
        CLEARED    // クリア済み
    };

    // 敵のスポーンイベント
    struct SpawnEvent
    {
        int triggerTime_;            // 何フレーム目にスポーンするのか
        ENEMY_TYPE type_;            // スポーンする敵の種類
        VECTOR pos_;                 // スポーン座標
        bool triggered_ = false;     // スポーンしたか
    };

    // スポナー設置イベント
    struct SpawnerInstallation
    {
        int triggerTime_;            // 何フレーム目にスポーンするのか
        float interval_;             // スポーンの間隔
        VECTOR pos_;                 // スポーン座標
        bool triggered_ = false;     // スポーンしたか
    };

    // コンストラクタで設定
    WaveBase(int prep, int wave);
    virtual ~WaveBase() = default;

    void Update(void);  // 更新
    virtual void Draw(void);    // 描画

    // 派生先で固有の処理があるなら記述する。
    virtual void OnStart();       // システム起動
    virtual void OnPrepare();     // 準備期間中
    virtual void OnWave();        // ウェーブ中
    virtual void OnClear();       // クリア後

    // 派生先でクリア条件を変更できる
    virtual bool CheckWaveClear();

    // スポーンイベントを作成
    void AddSpawnEvent(int time, ENEMY_TYPE type, VECTOR pos);
    void AddSpawner(int time, float interval, VECTOR pos);

    int GetSpawnEventNum(void)const { return (int)spawnEvents_.size(); }

    WaveState GetState(void)const { return state_; }

    // ウェーブ開始させる
    void StartInWave(void);
protected:

    // ウェーブ状態
    WaveState state_;

    int prepareTime_; // 準備時間
    int waveTime_;    // 戦闘時間
    int elapsed_;     // 経過時間

    std::vector<SpawnEvent> spawnEvents_;    // スポーンイベントを管理
    std::vector<SpawnerInstallation> spawnerIns_;    // スポナー設置イベントを管理

};
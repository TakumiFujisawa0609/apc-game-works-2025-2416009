#include "WaveBase.h"

#include "../Object/Enemy/EnemyManager.h"


WaveBase::WaveBase(int prep, int wave)
    :state(WaveState::Prepare), prepareTime(prep), waveTime(wave), elapsed(0)
{
}


void WaveBase::Update(void)
{
    // ウェーブクリア済みだったら処理しない
    if (state == WaveState::Cleared) return;

    // 時間を更新
    elapsed++;

    // 各状態で遷移
    switch (state)
    {
    case WaveState::Prepare:
        // 準備時間中
        OnPrepare();

        // 準備時間が経過したら
        if (elapsed >= prepareTime)
        {
            // ウェーブ開始
            state = WaveState::InWave;
            elapsed = 0;
            OnStart();
        }
        break;
    case WaveState::InWave:
        // ウェーブ中
        OnWave();

        // スポーンイベントを処理
        for (auto& event : spawnEvents)
        {
            // イベントが発動していないかつイベントの発動フレームになったら
            if (!event.triggered && elapsed >= event.triggerTime)
            {
                // マネージャーが存在するなら
                // 敵をスポーンさせる
                EnemyManager::GetInstance().Spawn(event.type, event.pos);

                // イベント発動
                event.triggered = true;
            }
        }

        // ウェーブクリア条件を達成したら
        if (CheckWaveClear())
        {
            // ウェーブクリア
            state = WaveState::Cleared;
            OnClear();
        }
        break;
    case WaveState::Cleared:
        // ウェーブクリア後は特に処理なし
        break;
    }
}

void WaveBase::Draw(void)
{
    switch (state)
    {
    case WaveBase::WaveState::Prepare:
        DrawFormatString(0, 180, 0xff0000, "%d / %d", elapsed, prepareTime);
        DrawFormatString(0, 200, 0xff0000, "現在の状態：Prepare");
        break;
    case WaveBase::WaveState::InWave:
        DrawFormatString(0, 180, 0xff0000, "%d / %d", elapsed, waveTime);
        DrawFormatString(0, 200, 0xff0000, "現在の状態：InWave");
        break;
    case WaveBase::WaveState::Cleared:
        DrawFormatString(0, 200, 0xff0000, "現在の状態：Cleared");
        break;
    default:
        break;
    }

    int cnt = 0;

    // スポーンイベントを処理
    for (auto& event : spawnEvents)
    {
        DrawFormatString(0, 300 + 20 * cnt, 0xffffff, "スポーンの判定 = %d", event.triggered);
        cnt++;
    }
}

void WaveBase::OnStart()
{
}

void WaveBase::OnPrepare()
{
}

void WaveBase::OnWave()
{
}

void WaveBase::OnClear()
{
}

bool WaveBase::CheckWaveClear()
{
    if (waveTime > 0 && elapsed >= waveTime) return true;

    bool isClear_ = true;

    for (auto& event : spawnEvents)
    {
        // イベントが発動していないかつイベントの発動フレームになったら
        if (!event.triggered)
        {
            isClear_ = false;
        }
    }

    return isClear_;
}

void WaveBase::AddSpawnEvent(int time, ENEMY_TYPE type, VECTOR pos)
{
    spawnEvents.push_back({ time, type, pos, false });
}

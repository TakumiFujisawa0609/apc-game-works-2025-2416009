#include "WaveBase.h"

#include "../Object/Enemy/EnemyManager.h"
#include "../Application.h"

WaveBase::WaveBase(int prep, int wave)
    :state_(WaveState::Prepare), prepareTime_(prep), waveTime_(wave), elapsed_(0)
{
}


void WaveBase::Update(void)
{
    // ウェーブクリア済みだったら処理しない
    if (state_ == WaveState::Cleared) return;

    // 時間を更新
    elapsed_++;

    // 各状態で遷移
    switch (state_)
    {
    case WaveState::Prepare:
        // 準備時間中
        OnPrepare();

        // 準備時間が経過したら
        if (elapsed_ >= prepareTime_)
        {
            // ウェーブ開始
            state_ = WaveState::InWave;
            elapsed_ = 0;
            OnStart();
        }
        break;
    case WaveState::InWave:
        // ウェーブ中
        OnWave();

        // スポーンイベントを処理
        for (auto& event : spawnEvents_)
        {
            // イベントが発動していないかつイベントの発動フレームになったら
            if (!event.triggered_ && elapsed_ >= event.triggerTime_)
            {
                // マネージャーが存在するなら
                // 敵をスポーンさせる
                EnemyManager::GetInstance().Spawn(event.type_, event.pos_);

                // イベント発動
                event.triggered_ = true;
            }
        }

        // ウェーブクリア条件を達成したら
        if (CheckWaveClear())
        {
            // ウェーブクリア
            state_ = WaveState::Cleared;
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
    int posX = Application::SCREEN_SIZE_X / 2;


    int time = (prepareTime_ - elapsed_) / 60;
    int waveTime = (waveTime_ - elapsed_) / 60;

    switch (state_)
    {
    case WaveBase::WaveState::Prepare:
        DrawFormatString(posX - 15, 5, 0xff0000, "準備");
        DrawFormatString(posX - 5, 40, 0xffffff, "%d", time);
        //DrawFormatString(0, 180, 0xff0000, "%d / %d", elapsed, prepareTime);
        //DrawFormatString(0, 200, 0xff0000, "現在の状態：Prepare");
        break;
    case WaveBase::WaveState::InWave:
        DrawFormatString(posX - 5, 40, 0xffffff, "%d", waveTime);
        //DrawFormatString(0, 180, 0xff0000, "%d / %d", elapsed, waveTime);
        //DrawFormatString(0, 200, 0xff0000, "現在の状態：InWave");
        break;
    case WaveBase::WaveState::Cleared:
        //DrawFormatString(0, 200, 0xff0000, "現在の状態：Cleared");
        break;
    default:
        break;
    }

    //int cnt = 0;

    //// スポーンイベントを処理
    //for (auto& event : spawnEvents)
    //{
    //    DrawFormatString(0, 300 + 20 * cnt, 0xffffff, "スポーンの判定 = %d", event.triggered);
    //    cnt++;
    //}
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
    if (waveTime_ > 0 && elapsed_ >= waveTime_) return true;

    return false;

    //bool isClear_ = true;

    //for (auto& event : spawnEvents_)
    //{
    //    // イベントが発動していないかつイベントの発動フレームになったら
    //    if (!event.triggered_)
    //    {
    //        isClear_ = false;
    //    }
    //}

    //return isClear_;
}

void WaveBase::AddSpawnEvent(int time, ENEMY_TYPE type, VECTOR pos)
{
    spawnEvents_.push_back({ time, type, pos, false });
}

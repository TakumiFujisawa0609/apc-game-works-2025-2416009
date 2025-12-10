#include "WaveBase.h"

#include "../Object/Enemy/EnemyManager.h"
#include "../Object/Spawner/SpawnerManager.h"
#include "../Application.h"

WaveBase::WaveBase(int prep, int wave)
    :state_(WaveState::PREPARE), prepareTime_(prep), waveTime_(wave), elapsed_(0)
{
    font_ = CreateFontToHandle("x12y12pxMaruMinya", 32, 20, DX_FONTTYPE_ANTIALIASING);
}

WaveBase::~WaveBase()
{
    spawnEvents_.clear();
    spawnerIns_.clear();
    DeleteFontToHandle(font_);
}

void WaveBase::Update(void)
{
    // ウェーブクリア済みだったら処理しない
    if (state_ == WaveState::CLEARED) return;

    // 時間を更新
    elapsed_++;

    // 各状態で遷移
    switch (state_)
    {
    case WaveState::PREPARE:
        // 準備時間中
        OnPrepare();

        // 準備時間が経過したら
        if (elapsed_ >= prepareTime_)
        {
            // ウェーブ開始
            StartInWave();
        }
        break;
    case WaveState::INWAVE:
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

        // スポナー設置イベントを処理
        for (auto& spawner : spawnerIns_)
        {
            // イベントが発動していないかつイベントの発動フレームになったら
            if (!spawner.triggered_ && elapsed_ >= spawner.triggerTime_)
            {
                // マネージャーが存在するなら
                // 敵をスポーンさせる
                SpawnerManager::GetInstance().SpawnerInstallation(spawner.pos_, spawner.interval_);

                // イベント発動
                spawner.triggered_ = true;
            }
        }

        // ウェーブクリア条件を達成したら
        if (CheckWaveClear())
        {
            // ウェーブクリア
            state_ = WaveState::CLEARED;
            OnClear();
        }
        break;
    case WaveState::CLEARED:
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
    case WaveBase::WaveState::PREPARE:
        DrawFormatString(posX - 15, 5, 0xff0000, "準備");
        DrawFormatStringToHandle(posX - 15, 40, 0xffffff, font_, "%d", time);
        break;
    case WaveBase::WaveState::INWAVE:
        DrawFormatStringToHandle(posX - 15, 40, 0xffffff, font_, "%d", waveTime);
        break;
    case WaveBase::WaveState::CLEARED:
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

void WaveBase::AddSpawner(int time, float interval, VECTOR pos)
{
    spawnerIns_.push_back({ time, interval, pos, false });
}

void WaveBase::StartInWave(void)
{
    state_ = WaveState::INWAVE;
    elapsed_ = 0;
    OnStart();
}

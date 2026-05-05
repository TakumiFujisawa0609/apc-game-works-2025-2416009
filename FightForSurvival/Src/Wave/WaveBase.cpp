#include "WaveBase.h"

#include "../Object/Enemy/EnemyManager.h"
#include "../Object/Spawner/SpawnerManager.h"
#include "../Application.h"

WaveBase::WaveBase(int prep, int wave)
    :state_(WaveState::PREPARE), prepareTime_(prep), waveTime_(wave), elapsed_(0)
{
    // フォントのハンドル読み込み
    font_[static_cast<int>(Font::BIG)] = CreateFontToHandle("x12y12pxMaruMinya", 32, 20, DX_FONTTYPE_ANTIALIASING);
    font_[static_cast<int>(Font::SMALL)] = CreateFontToHandle("x12y12pxMaruMinya", 20, 20, DX_FONTTYPE_ANTIALIASING);
}

WaveBase::~WaveBase()
{
    // 解放
    spawnEvents_.clear();
    spawnerIns_.clear();

    // フォントハンドルの解放
    DeleteFontToHandle(font_[static_cast<int>(Font::BIG)]);
    DeleteFontToHandle(font_[static_cast<int>(Font::SMALL)]);
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
                SpawnerManager::GetInstance().SpawnerInstallation(spawner.pos_, spawner.interval_, spawner.pattern_);

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
    int time = (prepareTime_ - elapsed_) / ONE_SECOND;
    int waveTime = (waveTime_ - elapsed_) / ONE_SECOND;
    int posX = TIME_POS_X;

    switch (state_)
    {
    case WaveBase::WaveState::PREPARE:

        // 「準備」の文字
        DrawStringToHandle(PREPARE_POS_X
            , PREPARE_POS_Y
            , "準備"
            , RED_COLOR
            , font_[static_cast<int>(Font::SMALL)]);

        // 時間の桁が10以上であれば
        if (time >= DIGIT)
        {
            // 位置を少しずらす
            posX -= OFFSET;
        }

        // 時間の表示
        DrawFormatStringToHandle(posX
            , TIME_POS_Y
            , WHITE_COLOR
            , font_[static_cast<int>(Font::BIG)]
            , "%d"
            , time);

        break;
    case WaveBase::WaveState::INWAVE:

        // 時間の桁が10以上であれば
        if (waveTime >= DIGIT)
        {
            // 位置を少しずらす
            posX -= OFFSET;
        }

        // 時間の表示
        DrawFormatStringToHandle(posX
            , TIME_POS_Y
            , WHITE_COLOR
            , font_[static_cast<int>(Font::BIG)]
            , "%d"
            , waveTime);

        break;
    case WaveBase::WaveState::CLEARED:
        break;
    default:
        break;
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
    // 戦闘時間が0より大きいかつ,設定された戦闘時間を経過時間が超えたら
    if (waveTime_ > 0 && elapsed_ >= waveTime_) return true;

    return false;
}

void WaveBase::AddSpawnEvent(int time, ENEMY_TYPE type, VECTOR pos)
{
    // 指定された敵スポーンイベントを入れる
    spawnEvents_.push_back({ time, type, pos, false });
}

void WaveBase::AddSpawner(int time, float interval, VECTOR pos, Spawner::PATTERN pattern)
{
    // 指定されたスポナースポーンイベント入れる
    spawnerIns_.push_back({ time, interval, pos,pattern, false });
}

void WaveBase::StartInWave(void)
{
    // ウェーブ開始させる
    state_ = WaveState::INWAVE;

    // 経過時間の初期化
    elapsed_ = 0;

    // 初期化処理
    OnStart();
}

#pragma once

#include "WaveBase.h"

#include <memory>
#include <vector>

class WaveManager
{
public:
    // シングルトン（生成・取得・削除）
    static void CreateInstance(void) { if (instance_ == nullptr) { instance_ = new WaveManager(); } };
    static WaveManager& GetInstance(void) { return *instance_; };
    static void DeleteInstance(void) { if (instance_ != nullptr) { delete instance_; instance_ = nullptr; } }

    // ウェーブの追加
    void AddWave(std::unique_ptr<WaveBase> wave);

    // 更新
    void Update(void);

    // 描画
    void Draw(void);

    // 現在のウェーブを取得
    WaveBase* GetCurrentWave(void);
    // 次のウェーブを取得
    WaveBase* GetNextWave(void);

    // ウェーブを全てクリアしているか
    bool AllCleared(void)const;

    // 1ウェーブがクリアしたか
    bool GetWaveIsClear(void)const { return waveIsClear_; }
    void EndWaveIsClear(void) { waveIsClear_ = false; }

private:
    // 静的インスタンス
    static WaveManager* instance_;

    // デフォルトコンストラクタをprivateにして、外部から生成できない様にする
    WaveManager(void);
    // デストラクタも同様
    ~WaveManager(void);

    // コピー・ムーブ操作を禁止
    WaveManager(const WaveManager&) = delete;
    WaveManager& operator=(const WaveManager&) = delete;
    WaveManager(WaveManager&&) = delete;
    WaveManager& operator=(WaveManager&&) = delete;

    // 各ウェーブを格納
    std::vector<std::unique_ptr<WaveBase>> waves_;
    // 今現在のウェーブ
    int currentWaveIndex_;

    // 1ウェーブがクリアしたか
    bool waveIsClear_;

};


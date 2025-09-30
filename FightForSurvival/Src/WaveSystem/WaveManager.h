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

    // ウェーブを全てクリアしているか
    bool AllCleared(void)const;

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

    // 下記をコンパイルエラーさせるため 上記を追加
    // WaveManager copy = *WaveManager::GetInstance();
    // WaveManager copied(*WaveManager::GetInstance());
    // WaveManager moved = std::move(*WaveManager::GetInstance());

    std::vector<std::unique_ptr<WaveBase>> waves;
    int currentWaveIndex;

};


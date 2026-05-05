#pragma once

#include "WaveBase.h"
#include "../Application.h"

#include <DxLib.h>

class WaveFinal : public WaveBase
{
public:

    // 準備時間
    static constexpr int PREPARE_TIME = 60 * 30;

    // ウェーブ時間
    static constexpr int WAVE_TIME = 60 * 15;

    // ウェーブ名座標
    static constexpr int WAVW_NAME_POS_X = Application::SCREEN_SIZE_X / 2 - 47;
    static constexpr int WAVW_NAME_POS_Y = 5;

    // 指示文字座標
    static constexpr int INSTRUCTION_POS_X = 8;
    static constexpr int INSTRUCTION_POS_Y = 40;

    // コンストラクタ
    WaveFinal(void);
    // デストラクタ
    ~WaveFinal(void)override;

    // システム起動
    void OnStart(void) override;
    // ウェーブ中
    void OnWave(void) override;
    // クリア後
    void OnClear(void) override;

    // 描画処理
    void Draw(void)override;
};


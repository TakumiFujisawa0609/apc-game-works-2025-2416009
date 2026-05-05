#pragma once

#include "WaveBase.h"

#include <DxLib.h>

class Wave4 : public WaveBase
{
public:

    // 準備時間
    static constexpr int PREPARE_TIME = ONE_SECOND * 30;

    // ウェーブ時間
    static constexpr int WAVE_TIME = ONE_SECOND * 20;

    // ウェーブ名座標
    static constexpr int WAVE_NAME_POS_X = Application::SCREEN_SIZE_X / 2 - 25;
    static constexpr int WAVE_NAME_POS_Y = 5;

    // 指示文字座標
    static constexpr int INSTRUCTION_POS_X = 8;
    static constexpr int INSTRUCTION_POS_Y = 40;

    // コンストラクタ
    Wave4(void);
    // デストラクタ
    ~Wave4(void)override;

    // システム起動
    void OnStart(void) override;
    // ウェーブ中
    void OnWave(void) override;
    // クリア後
    void OnClear(void) override;

    // 描画処理
    void Draw(void)override;

};


#pragma once

#include "WaveBase.h"

#include <DxLib.h>

class UIManager;
class TextureManager;

class Wave1 : public WaveBase
{
public:

    // スライド時間
    static constexpr int SLIDE_TIME_1 = ONE_SECOND * 5;
    static constexpr int SLIDE_TIME_2 = ONE_SECOND * 10;
    static constexpr int SLIDE_TIME_3 = ONE_SECOND * 15;
    static constexpr int SLIDE_END_TIME = ONE_SECOND * 19;

    // 準備時間
    static constexpr int PREPARE_TIME = ONE_SECOND * 20;

    // ウェーブ時間
    static constexpr int WAVE_TIME = ONE_SECOND * 45;

    // ウェーブ名座標
    static constexpr int WAVW_NAME_POS_X = Application::SCREEN_SIZE_X / 2 - 25;
    static constexpr int WAVW_NAME_POS_Y = 5;

    // 指示文字座標
    static constexpr int INSTRUCTION_POS_X = 8;
    static constexpr int INSTRUCTION_POS_Y = 40;

    // コンストラクタ
    Wave1(void);
    // デストラクタ
    ~Wave1(void)override;

    // システム起動
    void OnStart(void) override;
    // ウェーブ中
    void OnWave(void) override;
    // クリア後
    void OnClear(void) override;

    // 更新処理
    void Update(void)override;
    // 描画処理
    void Draw(void)override;

private:

    // UI
    UIManager* uiMgr_;
    TextureManager* texMgr_;
};



#pragma once

#include <string>

// UIの種別
enum class UI_KIND
{
    HP_BAR,
    STAMINA_BAR,
    MAGIC_STATUS,
    TITLE_BG,
    TITLE_TEXT,
    CLICK_TO_START,
    GAME_START,
    END,
    CLOUD,
    GRASS,

    MAX
};

// UIリソースの情報
struct UIResourceInfo
{
    UI_KIND uiKind;         // UIの種類
    bool isDraw;            // 描画フラグ
    std::string basePath;   // 下地
    std::string mainPath;   // メイン
    std::string framePath;  // フレーム
    std::string textPath;   // テキスト
    int x;                  // x座標
    int y;                  // y座標
    int width;              // 横幅
    int height;             // 縦幅
    float scale;            // 大きさ
};

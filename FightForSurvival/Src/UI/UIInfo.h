#pragma once

#include <string>

// UIの種別
enum class UI_KIND
{
    TITLE_BG,
    TITLE_BUTTON,
    HP_BAR,
    MAGIC_STATUS,

    MAX
};

// UIリソースの情報
struct UIResourceInfo
{
    std::string basePath;   // 下地
    std::string mainPath;   // メイン
    std::string framePath;  // フレーム
    int x;                  // x座標
    int y;                  // y座標
    int width;              // 横幅
    int height;             // 縦幅
    float scale;            // 大きさ
};

#include <map>
#include "UIInfo.h"

// UIのリソース情報などをテーブル管理する
inline std::map<UI_KIND, UIResourceInfo> UIResourceTable =
{

    { UI_KIND::HP_BAR,{
        UI_KIND::HP_BAR,
        true,
        "Data/Image/UI/HpBar/HpBar_0.png", "Data/Image/UI/HpBar/HpBar_1.png", "Data/Image/UI/HpBar/HpBar_2.png","",
        15, 625, 860, 108 ,0.5f}
    },

    { UI_KIND::STAMINA_BAR,{
        UI_KIND::STAMINA_BAR,
        true,
        "Data/Image/UI/StaminaBar/StaminaBar_0.png", "Data/Image/UI/StaminaBar/StaminaBar_1.png", "Data/Image/UI/StaminaBar/StaminaBar_2.png","",
        15, 680, 860, 68 ,0.5f}
    },

    { UI_KIND::MAGIC_STATUS,{
        UI_KIND::MAGIC_STATUS,
        true,
        "", "", "","",
        1120, 590, 300, 200 ,0.25f}
    },

    { UI_KIND::TITLE_BG,{
        UI_KIND::TITLE_BG,
        true,
        "Data/Image/UI/Title/BackGround.png", "", "","",
        0, 0, 1280, 720 ,1.0f}
    },

    { UI_KIND::TITLE_TEXT,{
        UI_KIND::TITLE_TEXT,
        true,
        "Data/Image/UI/Title/TitleText.png", "", "","",
        0, 0, 1280, 720, 1.0f}
    },

    { UI_KIND::CLICK_TO_START,{
        UI_KIND::CLICK_TO_START,
        true,
        "Data/Image/UI/Title/ClickToStartText.png", "", "","",
        0, 0, 1280, 720, 1.0f}
    },

    { UI_KIND::GAME_START,{
        UI_KIND::GAME_START,
        false,
        "Data/Image/UI/Title/GameStart_0.png", "Data/Image/UI/Title/GameStart_1.png", "Data/Image/UI/Title/GameStart_2.png","",
        335, 470, 610, 60, 1.0f}
    },

    { UI_KIND::END,{
        UI_KIND::END,
        false,
        "Data/Image/UI/Title/End_0.png", "Data/Image/UI/Title/End_1.png", "Data/Image/UI/Title/End_2.png","",
        335, 560, 610, 60, 1.0f}
    },

    { UI_KIND::CLOUD,{
        UI_KIND::CLOUD,
        true,
        "Data/Image/UI/Title/Cloud.png", "", "","",
        0, 0, 1280, 720, 1.0f}
    },

    { UI_KIND::PAUSE_CONTINUE,{
        UI_KIND::PAUSE_CONTINUE,
        true,
        "Data/Image/UI/Pause/PauseContinue_0.png", "Data/Image/UI/Pause/PauseContinue_1.png", "Data/Image/UI/Pause/PauseContinue_2.png","",
        512, 200, 256, 64, 1.0f}
    },

    { UI_KIND::PAUSE_SETTING,{
        UI_KIND::PAUSE_SETTING,
        true,
        "Data/Image/UI/Pause/PauseSetting_0.png", "Data/Image/UI/Pause/PauseSetting_1.png", "Data/Image/UI/Pause/PauseSetting_2.png","",
        512, 360, 256, 64, 1.0f}
    },

    { UI_KIND::PAUSE_TITLE,{
        UI_KIND::PAUSE_TITLE,
        true,
        "Data/Image/UI/Pause/PauseTitle_0.png", "Data/Image/UI/Pause/PauseTitle_1.png", "Data/Image/UI/Pause/PauseTitle_2.png","",
        512, 520, 256, 64, 1.0f}
    },

};

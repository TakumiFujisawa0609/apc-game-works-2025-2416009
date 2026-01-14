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

    { UI_KIND::SETTING_TEXT,{
        UI_KIND::SETTING_TEXT,
        true,
        "Data/Image/UI/Pause/Setting/SettingText.png", "", "","",
        0, 0, 1280, 720, 1.0f}
    },

    { UI_KIND::SETTING_DONE,{
        UI_KIND::SETTING_DONE,
        true,
        "Data/Image/UI/Pause/Setting/Done_0.png", "Data/Image/UI/Pause/Setting/Done_1.png", "Data/Image/UI/Pause/Setting/Done_2.png","",
        840, 520, 200, 64, 1.0f}
    },

    { UI_KIND::SETTING_BAR,{
        UI_KIND::SETTING_BAR,
        true,
        "Data/Image/UI/Pause/Setting/SettingBar.png", "", "","",
        384, 350, 513, 16, 1.0f}
    },

    { UI_KIND::WAVE_BASE,{
        UI_KIND::WAVE_BASE,
        true,
        "Data/Image/UI/WaveUI.png", "", "","",
        0, 0, 1280, 720, 1.0f}
    },

    { UI_KIND::CLEAR_BG,{
        UI_KIND::CLEAR_BG,
        true,
        "Data/Image/UI/GameClear/GameClearBG.png", "", "","",
        0, 0, 1280, 720 ,1.0f}
    },

    { UI_KIND::SCORE_BOARD,{
        UI_KIND::SCORE_BOARD,
        true,
        "Data/Image/UI/GameClear/ScoreBoard.png", "", "","",
        0, 0, 1280, 720 ,1.0f}
    },

    { UI_KIND::RANK_S,{
        UI_KIND::RANK_S,
        true,
        "Data/Image/UI/GameClear/Rank_S.png", "", "","",
        0, 0, 1280, 720 ,1.0f}
    },

    { UI_KIND::RANK_A,{
        UI_KIND::RANK_A,
        false,
        "Data/Image/UI/GameClear/Rank_A.png", "", "","",
        0, 0, 1280, 720 ,1.0f}
    },

    { UI_KIND::RANK_B,{
        UI_KIND::RANK_B,
        false,
        "Data/Image/UI/GameClear/Rank_B.png", "", "","",
        0, 0, 1280, 720 ,1.0f}
    },

    { UI_KIND::RANK_C,{
        UI_KIND::RANK_C,
        false,
        "Data/Image/UI/GameClear/Rank_C.png", "", "","",
        0, 0, 1280, 720 ,1.0f}
    },

    { UI_KIND::CLEAR_ZOMBIE,{
        UI_KIND::CLEAR_ZOMBIE,
        true,
        "Data/Image/UI/GameClear/ClearZombie.png", "", "","",
        220, 160, 301, 281 ,1.0f}
    },

    { UI_KIND::CLEAR_BAT,{
        UI_KIND::CLEAR_BAT,
        true,
        "Data/Image/UI/GameClear/ClearBat.png", "", "","",
       280, 500, 388, 374 ,1.0f}
    },

    { UI_KIND::CLEAR_DRAGON,{
        UI_KIND::CLEAR_DRAGON,
        true,
        "Data/Image/UI/GameClear/ClearDragon.png", "", "","",
        1150, 190, 538, 543 ,1.0f}
    },

};

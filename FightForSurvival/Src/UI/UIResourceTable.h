#include <map>
#include "UIInfo.h"

// UIのリソース情報などをテーブル管理する
inline std::map<UI_KIND, UIResourceInfo> UIResourceTable =
{
    { UI_KIND::TITLE_BG,{
        "Data/Image/cursor.png", "", "","",
        0, 0, 200, 20 ,1.0f}
    },

    { UI_KIND::TITLE_BUTTON,{
        "Data/UI/Button/ButtonDef.png", "Data/UI/Button/ButtonHover.png", "Data/UI/Button/ButtonTrig.png","",
        200, 200, 300, 200, 1.0f}
    },

    { UI_KIND::HP_BAR,{
        "Data/Image/UI/HpBar/HpBar_0.png", "Data/Image/UI/HpBar/HpBar_1.png", "Data/Image/UI/HpBar/HpBar_2.png","",
        15, 625, 860, 108 ,0.5f}
    },

    { UI_KIND::STAMINA_BAR,{
        "Data/Image/UI/StaminaBar/StaminaBar_0.png", "Data/Image/UI/StaminaBar/StaminaBar_1.png", "Data/Image/UI/StaminaBar/StaminaBar_2.png","",
        15, 680, 860, 68 ,0.5f}
    },

    { UI_KIND::MAGIC_STATUS,{
        "", "", "","",
        1120, 590, 300, 200 ,0.25f}
    },

    { UI_KIND::HP_UP,{
        "Data/Image/UI/Upgrade/Base_0.png", "Data/Image/UI/Upgrade/Base_1.png", "Data/Image/UI/Upgrade/Base_2.png","Data/Image/UI/Upgrade/HP_UP.png",
        100, 100, 300, 200 ,0.25f}
    },

    { UI_KIND::HP_HEAL,{
        "Data/Image/UI/Upgrade/Base_0.png", "Data/Image/UI/Upgrade/Base_1.png", "Data/Image/UI/Upgrade/Base_2.png","Data/Image/UI/Upgrade/HP_HEAL.png",
        1120, 590, 300, 200 ,0.25f}
    },

    { UI_KIND::SPEED_UP,{
        "Data/Image/UI/Upgrade/Base_0.png", "Data/Image/UI/Upgrade/Base_1.png", "Data/Image/UI/Upgrade/Base_2.png","Data/Image/UI/Upgrade/SPEED_UP.png",
        1120, 590, 300, 200 ,0.25f}
    },

    { UI_KIND::STAMINA_UP,{
        "Data/Image/UI/Upgrade/Base_0.png", "Data/Image/UI/Upgrade/Base_1.png", "Data/Image/UI/Upgrade/Base_2.png","Data/Image/UI/Upgrade/STAMINA_UP.png",
        1120, 590, 300, 200 ,0.25f}
    },

    { UI_KIND::POTION_RESTOC,{
        "Data/Image/UI/Upgrade/Base_0.png", "Data/Image/UI/Upgrade/Base_1.png", "Data/Image/UI/Upgrade/Base_2.png","Data/Image/UI/Upgrade/POTION_RESTOC.png",
        1120, 590, 300, 200 ,0.25f}
    },
};

#include <map>
#include "UIInfo.h"

// UIのリソース情報などをテーブル管理する
inline std::map<UI_KIND, UIResourceInfo> UIResourceTable =
{
    { UI_KIND::TITLE_BG,{
        "Data/Image/cursor.png", "", "",
        0, 0, 200, 20 }
    },

    { UI_KIND::TITLE_BUTTON,{
        "Data/UI/Button/ButtonDef.png", "Data/UI/Button/ButtonHover.png", "Data/UI/Button/ButtonTrig.png",
        200, 200, 300, 200 }
    },

    { UI_KIND::HP_BAR,{
        "", "Data/UI/Button/ButtonHover.png", "",
        100, 100, 300, 200 }
    },
};

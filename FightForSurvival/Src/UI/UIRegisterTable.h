#pragma once

#include "Factory/UIFactory.h"
#include "Object/Sprite/Sprite.h"
#include "Object/Bar/Bar.h"
#include "Object/Bar/HPBar.h"
#include "Object/Button/Button.h"
#include "Object/MagicStatus/MagicStatus.h"

// UI‘S‚Ä‚ð‚ ‚ç‚©‚¶‚ß“o˜^‚·‚é
inline void RegisterUI(void)
{
    UIFactory::GetInstance()->Register(UI_KIND::TITLE_BG,
        [](const UIResourceInfo& info, TextureManager* texMgr) -> UIBase*
        { Sprite* titleBG = new Sprite(info, texMgr); return titleBG; });

    UIFactory::GetInstance()->Register(UI_KIND::TITLE_BUTTON,
        [](const UIResourceInfo& info, TextureManager* texMgr) -> UIBase*
        { Button* button = new Button(info, texMgr); return button; });

    UIFactory::GetInstance()->Register(UI_KIND::HP_BAR,
        [](const UIResourceInfo& info, TextureManager* texMgr) -> UIBase*
        { Bar* hpBar = new HPBar(info, texMgr); return hpBar; });

    UIFactory::GetInstance()->Register(UI_KIND::MAGIC_STATUS,
        [](const UIResourceInfo& info, TextureManager* texMgr) -> UIBase*
        { MagicStatus* magicStatus = new MagicStatus(info, texMgr); return magicStatus; });
}

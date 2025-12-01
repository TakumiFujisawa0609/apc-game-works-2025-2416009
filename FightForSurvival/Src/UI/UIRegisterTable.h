#pragma once

#include "Factory/UIFactory.h"
#include "Object/Sprite/Sprite.h"
#include "Object/Bar/HPBar.h"
#include "Object/Bar/StaminaBar.h"
#include "Object/Button/Button.h"
#include "Object/MagicStatus/MagicStatus.h"

// UI‘S‚Ä‚ð‚ ‚ç‚©‚¶‚ß“o˜^‚·‚é
inline void RegisterUI(void)
{
    UIFactory::GetInstance()->Register(UI_KIND::HP_BAR,
        [](const UIResourceInfo& info, TextureManager* texMgr) -> UIBase*
        { HPBar* hpBar = new HPBar(info, texMgr); return hpBar; });

    UIFactory::GetInstance()->Register(UI_KIND::STAMINA_BAR,
        [](const UIResourceInfo& info, TextureManager* texMgr) -> UIBase*
        { StaminaBar* staminaBar = new StaminaBar(info, texMgr); return staminaBar; });

    UIFactory::GetInstance()->Register(UI_KIND::MAGIC_STATUS,
        [](const UIResourceInfo& info, TextureManager* texMgr) -> UIBase*
        { MagicStatus* magicStatus = new MagicStatus(info, texMgr); return magicStatus; });

    UIFactory::GetInstance()->Register(UI_KIND::TITLE_BG,
        [](const UIResourceInfo& info, TextureManager* texMgr) -> UIBase*
        { Sprite* titleBG = new Sprite(info, texMgr); return titleBG; });

    UIFactory::GetInstance()->Register(UI_KIND::TITLE_TEXT,
        [](const UIResourceInfo& info, TextureManager* texMgr) -> UIBase*
        { Sprite* titleText = new Sprite(info, texMgr); return titleText; });

    UIFactory::GetInstance()->Register(UI_KIND::CLICK_TO_START,
        [](const UIResourceInfo& info, TextureManager* texMgr) -> UIBase*
        { Sprite* clickToStart = new Sprite(info, texMgr); return clickToStart; });

    UIFactory::GetInstance()->Register(UI_KIND::GAME_START,
        [](const UIResourceInfo& info, TextureManager* texMgr) -> UIBase*
        { Button* gameStart = new Button(info, texMgr); return gameStart; });

    UIFactory::GetInstance()->Register(UI_KIND::END,
        [](const UIResourceInfo& info, TextureManager* texMgr) -> UIBase*
        { Button* end = new Button(info, texMgr); return end; });

    UIFactory::GetInstance()->Register(UI_KIND::CLOUD,
        [](const UIResourceInfo& info, TextureManager* texMgr) -> UIBase*
        { Sprite* cloud = new Sprite(info, texMgr); return cloud; });

    UIFactory::GetInstance()->Register(UI_KIND::GRASS,
        [](const UIResourceInfo& info, TextureManager* texMgr) -> UIBase*
        { Sprite* grass = new Sprite(info, texMgr); return grass; });
}

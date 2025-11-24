#pragma once

#include "Factory/UIFactory.h"
#include "Object/Sprite/Sprite.h"
#include "Object/Bar/HPBar.h"
#include "Object/Bar/StaminaBar.h"
#include "Object/Button/Button.h"
#include "Object/Button/UpgradeButton/UpgradeButton.h"
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
        { HPBar* hpBar = new HPBar(info, texMgr); return hpBar; });

    UIFactory::GetInstance()->Register(UI_KIND::STAMINA_BAR,
        [](const UIResourceInfo& info, TextureManager* texMgr) -> UIBase*
        { StaminaBar* staminaBar = new StaminaBar(info, texMgr); return staminaBar; });

    UIFactory::GetInstance()->Register(UI_KIND::MAGIC_STATUS,
        [](const UIResourceInfo& info, TextureManager* texMgr) -> UIBase*
        { MagicStatus* magicStatus = new MagicStatus(info, texMgr); return magicStatus; });

    UIFactory::GetInstance()->Register(UI_KIND::HP_UP,
        [](const UIResourceInfo& info, TextureManager* texMgr) -> UIBase*
        { UpgradeButton* hpUp = new UpgradeButton(info, texMgr); return hpUp; });

    UIFactory::GetInstance()->Register(UI_KIND::HP_HEAL,
        [](const UIResourceInfo& info, TextureManager* texMgr) -> UIBase*
        { UpgradeButton* hpHeal = new UpgradeButton(info, texMgr); return hpHeal; });

    UIFactory::GetInstance()->Register(UI_KIND::SPEED_UP,
        [](const UIResourceInfo& info, TextureManager* texMgr) -> UIBase*
        { UpgradeButton* speedUp = new UpgradeButton(info, texMgr); return speedUp; });

    UIFactory::GetInstance()->Register(UI_KIND::STAMINA_UP,
        [](const UIResourceInfo& info, TextureManager* texMgr) -> UIBase*
        { UpgradeButton* staminaUp = new UpgradeButton(info, texMgr); return staminaUp; });

    UIFactory::GetInstance()->Register(UI_KIND::POTION_RESTOC,
        [](const UIResourceInfo& info, TextureManager* texMgr) -> UIBase*
        { UpgradeButton* potionRestoc = new UpgradeButton(info, texMgr); return potionRestoc; });
}

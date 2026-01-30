#pragma once

#include "Factory/UIFactory.h"
#include "Object/Sprite/Sprite.h"
#include "Object/Sprite/FlashingSprite/FlashingSprite.h"
#include "Object/Sprite/PendulumSprite/PendulumSprite.h"
#include "Object/Sprite/ToggleSprite/ToggleSprite.h"
#include "Object/Sprite/ExplaneSprite/ExplaneSprite.h"
#include "Object/Bar/HPBar.h"
#include "Object/Bar/StaminaBar.h"
#include "Object/Button/Button.h"
#include "Object/Button/TitleButton/TitleButton.h"
#include "Object/Button/PauseButton/PauseButton.h"
#include "Object/Button/SettingButton/SettingButtonDone.h"
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
        { FlashingSprite* clickToStart = new FlashingSprite(info, texMgr); return clickToStart; });

    UIFactory::GetInstance()->Register(UI_KIND::GAME_START,
        [](const UIResourceInfo& info, TextureManager* texMgr) -> UIBase*
        { TitleButton* gameStart = new TitleButton(info, texMgr); return gameStart; });

    UIFactory::GetInstance()->Register(UI_KIND::END,
        [](const UIResourceInfo& info, TextureManager* texMgr) -> UIBase*
        { TitleButton* end = new TitleButton(info, texMgr); return end; });

    UIFactory::GetInstance()->Register(UI_KIND::CLOUD,
        [](const UIResourceInfo& info, TextureManager* texMgr) -> UIBase*
        { Sprite* cloud = new Sprite(info, texMgr); return cloud; });

    UIFactory::GetInstance()->Register(UI_KIND::PAUSE_CONTINUE,
        [](const UIResourceInfo& info, TextureManager* texMgr) -> UIBase*
        { PauseButton* pauseContinue = new PauseButton(info, texMgr); return pauseContinue; });

    UIFactory::GetInstance()->Register(UI_KIND::PAUSE_SETTING,
        [](const UIResourceInfo& info, TextureManager* texMgr) -> UIBase*
        { PauseButton* pauseSetting = new PauseButton(info, texMgr); return pauseSetting; });

    UIFactory::GetInstance()->Register(UI_KIND::PAUSE_TITLE,
        [](const UIResourceInfo& info, TextureManager* texMgr) -> UIBase*
        { PauseButton* pauseTitle = new PauseButton(info, texMgr); return pauseTitle; });

    UIFactory::GetInstance()->Register(UI_KIND::SETTING_TEXT,
        [](const UIResourceInfo& info, TextureManager* texMgr) -> UIBase*
        { Sprite* settingText = new Sprite(info, texMgr); return settingText; });

    UIFactory::GetInstance()->Register(UI_KIND::SETTING_DONE,
        [](const UIResourceInfo& info, TextureManager* texMgr) -> UIBase*
        { SettingButtonDone* settingButtonDone = new SettingButtonDone(info, texMgr); return settingButtonDone; });

    UIFactory::GetInstance()->Register(UI_KIND::SETTING_BAR,
        [](const UIResourceInfo& info, TextureManager* texMgr) -> UIBase*
        { Sprite* settingBar = new Sprite(info, texMgr); return settingBar; });

    UIFactory::GetInstance()->Register(UI_KIND::WAVE_BASE,
        [](const UIResourceInfo& info, TextureManager* texMgr) -> UIBase*
        { Sprite* waveBase = new Sprite(info, texMgr); return waveBase; });

    UIFactory::GetInstance()->Register(UI_KIND::CLEAR_SPRITE,
        [](const UIResourceInfo& info, TextureManager* texMgr) -> UIBase*
        { Sprite* clearSprite = new Sprite(info, texMgr); return clearSprite; });

    UIFactory::GetInstance()->Register(UI_KIND::CLEAR_BG,
        [](const UIResourceInfo& info, TextureManager* texMgr) -> UIBase*
        { Sprite* clearBG = new Sprite(info, texMgr); return clearBG; });

    UIFactory::GetInstance()->Register(UI_KIND::SCORE_BOARD,
        [](const UIResourceInfo& info, TextureManager* texMgr) -> UIBase*
        { Sprite* scoreBoard = new Sprite(info, texMgr); return scoreBoard; });

    UIFactory::GetInstance()->Register(UI_KIND::RANK_S,
        [](const UIResourceInfo& info, TextureManager* texMgr) -> UIBase*
        { Sprite* rankS = new Sprite(info, texMgr); return rankS; });

    UIFactory::GetInstance()->Register(UI_KIND::RANK_A,
        [](const UIResourceInfo& info, TextureManager* texMgr) -> UIBase*
        { Sprite* rankA = new Sprite(info, texMgr); return rankA; });

    UIFactory::GetInstance()->Register(UI_KIND::RANK_B,
        [](const UIResourceInfo& info, TextureManager* texMgr) -> UIBase*
        { Sprite* rankB = new Sprite(info, texMgr); return rankB; });

    UIFactory::GetInstance()->Register(UI_KIND::RANK_C,
        [](const UIResourceInfo& info, TextureManager* texMgr) -> UIBase*
        { Sprite* rankC = new Sprite(info, texMgr); return rankC; });

    UIFactory::GetInstance()->Register(UI_KIND::CLEAR_ZOMBIE,
        [](const UIResourceInfo& info, TextureManager* texMgr) -> UIBase*
        { PendulumSprite* clearZombie = new PendulumSprite(info, texMgr); return clearZombie; });

    UIFactory::GetInstance()->Register(UI_KIND::CLEAR_BAT,
        [](const UIResourceInfo& info, TextureManager* texMgr) -> UIBase*
        { PendulumSprite* clearBat = new PendulumSprite(info, texMgr); return clearBat; });

    UIFactory::GetInstance()->Register(UI_KIND::CLEAR_DRAGON,
        [](const UIResourceInfo& info, TextureManager* texMgr) -> UIBase*
        { PendulumSprite* clearDragon = new PendulumSprite(info, texMgr); return clearDragon; });

    UIFactory::GetInstance()->Register(UI_KIND::OVER_SPRITE,
        [](const UIResourceInfo& info, TextureManager* texMgr) -> UIBase*
        { Sprite* overSprite = new Sprite(info, texMgr); return overSprite; });

    UIFactory::GetInstance()->Register(UI_KIND::OVER_BG,
        [](const UIResourceInfo& info, TextureManager* texMgr) -> UIBase*
        { Sprite* overBG = new Sprite(info, texMgr); return overBG; });

    UIFactory::GetInstance()->Register(UI_KIND::MANUAL,
        [](const UIResourceInfo& info, TextureManager* texMgr) -> UIBase*
        { ToggleSprite* manual = new ToggleSprite(info, texMgr); return manual; });

    UIFactory::GetInstance()->Register(UI_KIND::EXPLANE,
        [](const UIResourceInfo& info, TextureManager* texMgr) -> UIBase*
        { ExplaneSprite* explane = new ExplaneSprite(info, texMgr); return explane; });

}

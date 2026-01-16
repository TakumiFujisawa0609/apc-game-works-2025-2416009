#pragma once

#include "WaveBase.h"

#include <DxLib.h>

class UIManager;
class TextureManager;

class Wave1 : public WaveBase
{
public:
    Wave1(void);
    ~Wave1(void)override;

    void OnStart(void) override;
    void OnWave(void) override;
    void OnClear(void) override;

    void Update(void)override;
    void Draw(void)override;

private:

    // UI
    UIManager* uiMgr_;
    TextureManager* texMgr_;
};



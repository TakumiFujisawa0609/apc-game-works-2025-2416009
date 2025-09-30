#pragma once

#include "../WaveBase.h"

#include <DxLib.h>

class Wave1 : public WaveBase
{
public:
    Wave1(void);

    void OnStart(void) override;
    void OnWave(void) override;
    void OnClear(void) override;

    void Draw(void)override;
};



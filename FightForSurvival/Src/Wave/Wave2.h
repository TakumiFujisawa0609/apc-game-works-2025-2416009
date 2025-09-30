#pragma once

#include "WaveBase.h"

#include <DxLib.h>

class Wave2 : public WaveBase
{
public:
    Wave2(void);

    void OnStart(void) override;
    void OnWave(void) override;
    void OnClear(void) override;

    void Draw(void)override;
};


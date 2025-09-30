#pragma once

#include "WaveBase.h"

#include <DxLib.h>

class WaveFinal : public WaveBase
{
public:
    WaveFinal(void);

    void OnStart(void) override;
    void OnWave(void) override;
    void OnClear(void) override;

    void Draw(void)override;
};


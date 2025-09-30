#pragma once

#include "WaveBase.h"

#include <DxLib.h>

class Wave1 : public WaveBase
{
public:
    Wave1(void);

    void OnStart() override;
    void OnWave() override;
    void OnClear() override;

    void Draw(void)override;
};



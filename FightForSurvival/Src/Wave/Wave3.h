#pragma once

#include "WaveBase.h"

#include <DxLib.h>

class Wave3 : public WaveBase
{
public:

    Wave3(void);
    ~Wave3(void)override;

    void OnStart(void) override;
    void OnWave(void) override;
    void OnClear(void) override;

    void Draw(void)override;

};



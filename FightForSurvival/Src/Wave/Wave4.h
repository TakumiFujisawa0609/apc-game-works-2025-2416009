#pragma once

#include "WaveBase.h"

#include <DxLib.h>

class Wave4 : public WaveBase
{
public:

    Wave4(void);
    ~Wave4(void)override;

    void OnStart(void) override;
    void OnWave(void) override;
    void OnClear(void) override;

    void Draw(void)override;

};


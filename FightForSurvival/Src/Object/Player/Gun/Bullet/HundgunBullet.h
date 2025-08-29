#pragma once

#include "BulletBase.h"

class HundgunBullet : public BulletBase
{
public:

	// 速度
	static constexpr float SPEED = 10.0f;

	// 存在可能時間
	static constexpr float CNT_ALIVE = 10.0f;

	HundgunBullet(int baseModelId);
	~HundgunBullet(void);

protected:

	// パラメータ設定
	void SetParam(void) override;

};


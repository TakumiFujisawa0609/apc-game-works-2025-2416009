#pragma once

#include "BulletBase.h"

class HundgunBullet : public BulletBase
{
public:

	// 速度
	static constexpr float SPEED = 100.0f;

	// 存在可能時間
	static constexpr float CNT_ALIVE = 1.0f;
	
	// 当たり判定用サイズ
	static constexpr float COLLISION_RAD = 5.0f;
	
	// 当たり判定用サイズ
	static constexpr int DAMAGE = 10;

	HundgunBullet(int baseModelId);
	~HundgunBullet(void);

protected:

	// パラメータ設定
	void SetParam(void) override;

};


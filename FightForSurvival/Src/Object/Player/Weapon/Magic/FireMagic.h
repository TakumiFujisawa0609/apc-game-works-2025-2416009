#pragma once

#include "MagicBase.h"

class FireMagic : public MagicBase
{
public:

	// 速度
	static constexpr float SPEED = 100.0f;

	// 存在可能時間
	static constexpr float CNT_ALIVE = 1.0f;
	
	// 当たり判定用サイズ
	static constexpr float COLLISION_RAD = 3.0f;
	
	// 当たり判定用サイズ
	static constexpr int HEAD_DAMAGE = 10;
	static constexpr int BODY_DAMAGE = 5;

	FireMagic(int baseModelId);
	~FireMagic(void);

protected:

	// パラメータ設定
	void SetParam(void) override;

};


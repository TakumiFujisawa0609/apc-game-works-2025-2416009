#pragma once

#include "../../../Vector2/Vector2.h"

class Bar
{
public:

	Bar(Vector2 pos, Vector2 size, unsigned int color, float max, float min, float now);
	virtual ~Bar(void);

	void Init(void);
	virtual void Draw(void);
	void Release(void);

	void SetNowValue(float now) { now_ = now; }

protected:

	// バーの始点
	Vector2 pos_;
	// バーのサイズ
	Vector2 size_;
	// バーの色
	unsigned int color_;

	// 最大値
	float max_;
	// 最小値
	float min_;
	// 現在値
	float now_;

	float range_;
	float pixelBerUnit_;
};


#pragma once

#include "../../../Vector2/Vector2.h"

class Bar
{
public:

	Bar(void);
	~Bar(void);

	void Init(void);
	void Update(float now);
	void Draw(void);
	void Release(void);

	void SetNowValue(float now) { now_ = now; }

protected:

	Vector2 pos_;

	Vector2 size_;

	float max_;
	float min_;
	float now_;

	float dis_;
	float value_;

	virtual void SetParam(void) = 0;

};


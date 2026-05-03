#pragma once

#include "../../Common/Vector2/Vector2.h"

class Reticle
{
public:

	Reticle(void);
	~Reticle(void);

	void Load(void);
	void Init(void);
	void Draw(void);
	void Release(void);

private:
	int img_;
	Vector2 pos_;
};


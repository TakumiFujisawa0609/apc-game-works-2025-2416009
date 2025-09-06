#pragma once

#include "../Vector2.h"
#include "../../Application.h"

class Score
{
public:

	static constexpr int POS_X = Application::SCREEN_SIZE_X - 130;
	static constexpr int POS_Y = 0;

	Score(void);
	~Score(void);

	void Init(void);
	void Draw(void);
	void Release(void);

private:

	Vector2 pos_;

};


#pragma once

#include "../Vector2/Vector2.h"
#include "../../Application.h"

class Score
{
public:

	// 座標
	static constexpr int POS_X = Application::SCREEN_SIZE_X - 250;
	static constexpr int POS_Y = 0;

	Score(void);
	~Score(void);

	void Load(void);
	void Init(void);
	void Draw(void);
	void Release(void);

private:

	// 座標
	Vector2 pos_;

	// フォントハンドル
	int font_;
};


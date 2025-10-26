#pragma once

#include "../Base/SceneBase.h"

class OverScene : public SceneBase
{
public:

	// コンストラクタ
	OverScene(void);

	// デストラクタ
	~OverScene(void);

	void Load(void) override;
	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:

	// スコア
	int score_;

};


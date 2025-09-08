#pragma once

#include "SceneBase.h"

class OverScene : public SceneBase
{
public:

	// コンストラクタ
	OverScene(void);

	// デストラクタ
	~OverScene(void);

	void Init(void) override;
	void Load(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:

	// スコア
	int score_;

};


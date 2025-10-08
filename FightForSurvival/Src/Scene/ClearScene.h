#pragma once

#include "SceneBase.h"

class ClearScene : public SceneBase
{
public:

	// コンストラクタ
	ClearScene(void);

	// デストラクタ
	~ClearScene(void);

	void Load(void) override;
	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:

	// スコア
	int score_;

};

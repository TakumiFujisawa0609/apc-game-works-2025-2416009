#pragma once
#include "SceneBase.h"

class Camera;

class TitleScene : public SceneBase
{

public:

	// コンストラクタ
	TitleScene(void);

	// デストラクタ
	~TitleScene(void);

	void Load(void) override;
	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:
	// カメラ
	Camera* camera_;

	// 画像
	int imgTitle_;

};

#pragma once

#include"../Button.h"

#include"../../../../Scene/Title/TitleScene.h"

class TitleButton :public Button
{
public:
	// コンストラクタ
	TitleButton(const UIResourceInfo& info, TextureManager* texMgr);

	// デストラクタ
	~TitleButton()override;

	void Update(void) override;		// 更新
	void Draw(void) override;		// 描画

	// 現在のステートをもらう
	void SetNowState(TitleScene::STATE* state) { state_ = state; }

private:

	TitleScene::STATE* state_;
};


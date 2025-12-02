#pragma once

#include"../Button.h"

#include"../../../../Common/Pause/Pause.h"

class PauseButton:public Button
{
public:
	// コンストラクタ
	PauseButton(const UIResourceInfo& info, TextureManager* texMgr);

	// デストラクタ
	~PauseButton()override;

	void Update(void) override;		// 更新
	void Draw(void) override;		// 描画

	// 現在のポーズモードをもらう
	void SetNowPause(Pause::PAUSE* pause) { pause_ = pause; }

private:

	Pause::PAUSE* pause_;
};


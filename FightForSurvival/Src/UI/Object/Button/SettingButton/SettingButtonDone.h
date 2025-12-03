#pragma once

#include"../Button.h"

#include"../../../../Common/Pause/Setting/Setting.h"

class SettingButtonDone :public Button
{
public:
	// コンストラクタ
	SettingButtonDone(const UIResourceInfo& info, TextureManager* texMgr);

	// デストラクタ
	~SettingButtonDone()override;

	void Update(void) override;		// 更新
	void Draw(void) override;		// 描画

	void SetIsDone(bool* isDone) { isDone_ = isDone; }

private:

	bool* isDone_;

};

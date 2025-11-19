#pragma once

#include "Bar.h"

// Barを継承
class HPBar : public Bar
{
public:

	// コンストラクタ そのままベースに入れる
	HPBar(const UIResourceInfo& info, TextureManager* texMgr) :
		Bar(info, texMgr) {}

	// デストラクタ
	~HPBar();

	void Update(void) override;		// 更新
	void Draw(void) override;		// 描画

private:
};

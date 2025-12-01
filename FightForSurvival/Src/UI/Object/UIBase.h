#pragma once

#include "../UIInfo.h"

class UIBase
{
public:
	UIBase();				// コンストラクタ
	virtual ~UIBase();		// デストラクタ

	virtual void Draw(void) = 0;		// 描画
	virtual void Update(void) = 0;		// 更新

	// UIの種類を返す
	UI_KIND GetUIKind(void)const { return uiKind_; }

	// UIの種類を返す
	void SetIsDraw (bool flg) { isDraw_ = flg; }

protected:

	bool isDraw_;	// 描画フラグ
	int x_;	// x座標
	int y_;	// y座標
	int w_;	// 横幅
	int h_;	// 縦幅
	float scale_;	// 大きさ

	UI_KIND uiKind_;
};

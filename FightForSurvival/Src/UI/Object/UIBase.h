#pragma once

class UIBase
{
public:
	UIBase();				// コンストラクタ
	virtual ~UIBase();		// デストラクタ

	virtual void Draw(void) = 0;		// 描画
	virtual void Update(void) = 0;		// 更新

protected:
	int x_;	// x座標
	int y_;	// y座標
	int w_;	// 横幅
	int h_;	// 縦幅
	float scale_;	// 大きさ
};

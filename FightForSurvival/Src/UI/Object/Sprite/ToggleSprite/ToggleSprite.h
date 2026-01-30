#pragma once

#include <DxLib.h>
#include"../Sprite.h"

class ToggleSprite :public Sprite
{
public:

	// コンストラクタ
	ToggleSprite(const UIResourceInfo& info, TextureManager* texMgr);

	// デストラクタ
	~ToggleSprite()override;

	void Update(void) override;		// 更新
	void Draw(void) override;		// 描画

	void SetIsPicture1Flg(bool flg) { isPicture1Flg_ = flg; }

private:

	bool isPicture1Flg_;	// true / 1枚目の画像, false / 2枚目の画像

	int picture1_;	// 画像１枚目のハンドル
	int picture2_;	// 画像２枚目のハンドル

};


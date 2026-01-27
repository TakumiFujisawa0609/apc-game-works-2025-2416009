#pragma once

#include <DxLib.h>
#include"../Sprite.h"

class ToggleSprite :public Sprite
{
public:

	// 回転の最小値
	static constexpr float ANGLE_MIN = -15.0f * (DX_PI_F / 180.0f);
	// 回転の最大値
	static constexpr float ANGLE_MAX = 15.0f * (DX_PI_F / 180.0f);

	// 回転の速度
	static constexpr float ROTATION_SPEED = 0.8f * (DX_PI_F / 180.0f);

	// コンストラクタ
	ToggleSprite(const UIResourceInfo& info, TextureManager* texMgr);

	// デストラクタ
	~ToggleSprite()override;

	void Update(void) override;		// 更新
	void Draw(void) override;		// 描画

	void SetDraw1Flg(bool flg) { draw1Flg_ = flg; }

private:

	bool draw1Flg_;	// true / 1枚目の画像, false / 2枚目の画像


};


#pragma once

#include <DxLib.h>
#include"../Sprite.h"

// スプライトを継承
class PendulumSprite :public Sprite
{
public:
	// 回転の最小値
	static constexpr float ANGLE_MIN = -20.0f * (DX_PI_F / 180.0f);
	// 回転の最大値
	static constexpr float ANGLE_MAX = 20.0f * (DX_PI_F / 180.0f);

	// 回転の速度
	static constexpr float ROTATION_SPEED = 1.0f * (DX_PI_F / 180.0f);

	// コンストラクタ
	PendulumSprite(const UIResourceInfo& info, TextureManager* texMgr);

	// デストラクタ
	~PendulumSprite()override;

	void Update(void) override;		// 更新
	void Draw(void) override;		// 描画

private:

	bool isLeft_;			//　回転の左右を変更するためのフラグ
	float angle_;			//　回転値

};


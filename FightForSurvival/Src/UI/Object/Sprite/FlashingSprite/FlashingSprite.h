#pragma once

#include"../Sprite.h"

// スプライトを継承
class FlashingSprite:public Sprite
{
public:
	// アルファ値をかえるスピード
	static constexpr float ALPHA_SPEED = 4.0f;
	static constexpr float ALPHA_MAX = 255.0f;

	// コンストラクタ
	FlashingSprite(const UIResourceInfo& info, TextureManager* texMgr);

	// デストラクタ
	~FlashingSprite()override;

	void Update(void) override;		// 更新
	void Draw(void) override;		// 描画

private:
	bool isAlpha_;			//　アルファ値を変化させるためのフラグ
	float alpha_;			//　アルファ値

};


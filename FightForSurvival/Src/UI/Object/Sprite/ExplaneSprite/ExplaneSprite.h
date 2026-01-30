#pragma once

#include <DxLib.h>
#include"../Sprite.h"

class ExplaneSprite :public Sprite
{
public:

	// 画像の種類
	enum EXPLANE
	{
		EXPLANE_1,		// 画像1
		EXPLANE_2,		// 画像2
		EXPLANE_3,		// 画像3
		EXPLANE_4,		// 画像4

		MAX,		// 最大数
	};

public:

	// コンストラクタ
	ExplaneSprite(const UIResourceInfo& info, TextureManager* texMgr);

	// デストラクタ
	~ExplaneSprite()override;

	void Draw(void) override;		// 描画

	void SetDrawPictureKind(EXPLANE i) { explaneKind_ = i; }

private:

	EXPLANE explaneKind_;	// 選択中の添字番号

	int handle_[EXPLANE::MAX];	// 画像ハンドル

};


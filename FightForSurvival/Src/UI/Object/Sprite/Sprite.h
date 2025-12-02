#pragma once

#include "../UIBase.h"
#include "../../UIInfo.h"

// クラスの前方宣言
class TextureManager;

// ベースを継承
class Sprite : public UIBase
{
public:
	// コンストラクタ
	Sprite(const UIResourceInfo& info, TextureManager* texMgr);

	// デストラクタ
	virtual ~Sprite()override;

	virtual void Update(void) override;		// 更新
	virtual void Draw(void) override;		// 描画

protected:
	int handle_;
};

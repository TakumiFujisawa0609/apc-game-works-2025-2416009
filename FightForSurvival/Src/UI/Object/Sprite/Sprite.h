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
	~Sprite();

	void Update(void) override;		// 更新
	void Draw(void) override;		// 描画
	void Delete(void) override;		// 削除

private:
	int handle_;
};

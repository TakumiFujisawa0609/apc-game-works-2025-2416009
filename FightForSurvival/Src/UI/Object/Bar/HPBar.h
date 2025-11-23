#pragma once

#include "../UIBase.h"
#include "../../UIInfo.h"

// クラスの前方宣言
class TextureManager;
class Player;

class HPBar : public UIBase
{
public:

	// コンストラクタ
	HPBar(const UIResourceInfo& info, TextureManager* texMgr);
	~HPBar();		// デストラクタ

	// 情報のアドレスをセット
	void SetPlayer(Player* player);

	void Update(void) override;		// 更新
	void Draw(void) override;		// 描画

protected:
	// 描画のゲージ率
	float barRate_;		// 0.0f ～ 1.0f の範囲で割合が変わる

	// バー本体のハンドル
	int baseHandle_;		// 下地
	int barHandle_;			// 本体
	int frameHandle_;		// フレーム

	Player* player_;
};

#pragma once

#include "../UIBase.h"
#include "../../UIInfo.h"

// クラスの前方宣言
class TextureManager;

// ベースを継承
class Bar : public UIBase
{
public:

	// コンストラクタ
	Bar(const UIResourceInfo& info, TextureManager* texMgr);
	~Bar();		// デストラクタ

	// 情報のアドレスをセット
	void SetValue(int* currentNum, int* maxNum);

	void Update(void) override;		// 更新
	void Draw(void) override;		// 描画
	void Delete(void) override;		// 削除

protected:
	// 描画のゲージ率
	float barRate_;		// 0.0f ～ 1.0f の範囲で割合が変わる

	// バー本体のハンドル
	int baseHandle_;		// 下地
	int barHandle_;			// 本体
	int frameHandle_;		// フレーム

	int* currentNum_;	// 現在の数値
	int* maxNum_;		// 最大の数値
};

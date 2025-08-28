#pragma once

#include "GunBase.h"

class Handgun :public GunBase
{
public:

	// モデルの大きさ
	static constexpr VECTOR SCALE = { 0.5f,0.5f,0.5f };

	// 使用時のプレイヤーとの相対座標
	static constexpr VECTOR RELATIVE_POS = { 0.0f,90.0f,0.0f };

	// コンストラクタ
	Handgun(void);
	// デストラクタ
	~Handgun(void)override;

protected:

	// 画像やモデルなどのロード
	void Load(void) override;
	// パラメータ設定
	void SetParam(void) override;

};


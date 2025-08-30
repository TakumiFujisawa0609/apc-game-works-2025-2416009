#pragma once

#include "GunBase.h"

class Handgun :public GunBase
{
public:

	// モデルの大きさ
	static constexpr VECTOR SCALE = { 0.5f,0.5f,0.5f };

	// モデルの向き
	static constexpr VECTOR ROTATE = { 0.0f,0.0f,0.0f };

	// 弾発射後の硬直時間
	static constexpr float STOP_DELAY = 0.4f;

	// コンストラクタ
	Handgun(Player* player);
	// デストラクタ
	~Handgun(void)override;

protected:

	// 画像やモデルなどのロード
	void Load(void) override;
	// パラメータ設定
	void SetParam(void) override;

};


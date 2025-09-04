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
	static constexpr float RECOIL = 0.01f;

	// 最大装填数
	static constexpr int BULLET_CAPACITY = 10;

	// 最大弾数
	static constexpr int BULLET_NUM_MAX = 50;

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


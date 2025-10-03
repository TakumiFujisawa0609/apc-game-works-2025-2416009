#pragma once

#include "WeaponBase.h"

class Stick :public WeaponBase
{
public:

	// モデルの大きさ
	static constexpr VECTOR SCALE = { 0.5f,0.5f,0.5f };

	// モデルの向き
	static constexpr VECTOR ROTATE = { 0.0f,0.0f,0.0f };

	// 魔法発射後の硬直時間
	static constexpr float RECOIL = 0.01f;

	// 最大魔法攻撃回数
	static constexpr int MAGIC_CAPACITY = 15;

	// MPポーション数
	static constexpr int MP_POTION_NUM = 10;

	// コンストラクタ
	Stick(Player* player);
	// デストラクタ
	~Stick(void)override;

protected:

	// 画像やモデルなどのロード
	void Load(void) override;
	// パラメータ設定
	void SetParam(void) override;

};


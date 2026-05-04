#pragma once

#include <DxLib.h>
#include "../Base/MagicBase.h"

class StraightMagic : public MagicBase
{
public:
	// 速度
	static constexpr float SPEED = 40.0f;

	// 存在可能時間
	static constexpr float CNT_ALIVE = 1.0f;
	
	// 当たり判定用サイズ
	static constexpr float COLLISION_RAD = 3.0f;
	
	// 当たり判定用ダメージ
	// 頭
	static constexpr int HEAD_DAMAGE = 10;
	// 体
	static constexpr int BODY_DAMAGE = 5;

	// エフェクトの大きさ
	static constexpr float EFFECT_SCALE = 10.0f;

	StraightMagic(TYPE_MAGIC typeMagic, VECTOR* weponPos);
	~StraightMagic(void)override;

protected:

	// パラメータ設定
	void SetParam(void) override;

	// ステート変更時の設定事項
	void ChangeCharge(void)override;
	void ChangeShot(void)override;
	void ChangeBlast(void)override;
};


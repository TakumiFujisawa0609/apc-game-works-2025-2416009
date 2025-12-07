#pragma once

#include <DxLib.h>
#include "../Base/MagicBase.h"

class ExplosionMagic : public MagicBase
{
public:
	// 大きさ
	static constexpr VECTOR SCALE = { 1.0f,1.0f,1.0f };

	// 向き
	static constexpr VECTOR ROTATE = { 0.0f,0.0f,0.0f };

	// 速度
	static constexpr float SPEED = 40.0f;

	// 存在可能時間
	static constexpr float CNT_ALIVE = 1.0f;

	// 着弾前の当たり判定サイズ
	static constexpr float COLLISION_RAD = 3.0f;
	// 着弾後の当たり判定サイズ
	static constexpr float HIT_COLLISION_RAD = 300.0f;

	// 当たり判定用ダメージ
	// 頭
	static constexpr int HEAD_DAMAGE = 10;
	// 体
	static constexpr int BODY_DAMAGE = 5;

	ExplosionMagic(TYPE_MAGIC typeMagic, int baseModelId, VECTOR* weponPos);
	~ExplosionMagic(void)override;

protected:

	// パラメータ設定
	void SetParam(void) override;

	// ステート変更時の設定事項
	void ChangeCharge(void)override;
	void ChangeShot(void)override;
	void ChangeBlast(void)override;

};


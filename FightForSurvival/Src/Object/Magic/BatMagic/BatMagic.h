#pragma once

#include <DxLib.h>
#include "../Base/MagicBase.h"

class BatMagic : public MagicBase
{
public:

	// 大きさ
	static constexpr VECTOR SCALE = { 1.0f,1.0f,1.0f };

	// 向き
	static constexpr VECTOR ROTATE = { 0.0f,0.0f,0.0f };

	// 速度
	static constexpr float SPEED = 12.0f;

	// 存在可能時間
	static constexpr float CNT_ALIVE = 3.0f;

	// 当たり判定用サイズ
	static constexpr float COLLISION_RAD = 30.0f;

	// 当たり判定用サイズ
	static constexpr int DAMAGE = 1;

	BatMagic(TYPE_MAGIC typeMagic, int baseModelId,VECTOR* pos);
	~BatMagic(void)override;

protected:

	// パラメータ設定
	void SetParam(void) override;

	// ステート変更時の設定事項
	void ChangeCharge(void)override;
	void ChangeShot(void)override;
	void ChangeBlast(void)override;
};


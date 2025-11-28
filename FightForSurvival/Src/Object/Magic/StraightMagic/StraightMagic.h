#pragma once

#include <DxLib.h>
#include "../Base/MagicBase.h"

class StraightMagic : public MagicBase
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
	
	// 当たり判定用サイズ
	static constexpr float COLLISION_RAD = 3.0f;
	
	// 当たり判定用ダメージ
	// 頭
	static constexpr int HEAD_DAMAGE = 10;
	// 体
	static constexpr int BODY_DAMAGE = 5;

	StraightMagic(TYPE_MAGIC typeMagic, int baseModelId);
	~StraightMagic(void)override;

protected:

	// パラメータ設定
	void SetParam(void) override;

	// ステート変更時の設定事項
	void ChangeShot(void)override;
};


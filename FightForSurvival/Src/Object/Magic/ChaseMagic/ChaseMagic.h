#pragma once

#include <DxLib.h>
#include "../Base/MagicBase.h"

class ChaseMagic : public MagicBase
{
public:

	// 速度
	static constexpr float SPEED = 30.0f;

	// 存在可能時間
	static constexpr float CNT_ALIVE = 1.0f;

	// 当たり判定用サイズ
	static constexpr float COLLISION_RAD = 3.0f;

	// 当たり判定用ダメージ
	// 頭
	static constexpr int HEAD_DAMAGE = 10;
	// 体
	static constexpr int BODY_DAMAGE = 5;

	// 視野の広さ
	static constexpr float VIEW_RANGE = 3000.0f;
	// 視野角
	static constexpr float VIEW_ANGLE = 20.0f;

	// エフェクトの大きさ
	static constexpr float EFFECT_SCALE = 10.0f;

	ChaseMagic(TYPE_MAGIC typeMagic, VECTOR* weponPos);
	~ChaseMagic(void)override;

protected:

	// パラメータ設定
	void SetParam(void) override;

	// 魔法の移動処理
	void UpdateShot(void)override;

	// ステート変更時の設定事項
	void ChangeCharge(void)override;
	void ChangeShot(void)override;
	void ChangeBlast(void)override;

private:

	// ターゲットの座標
	VECTOR targetPos_;

	// ターゲットが見つかったか
	bool targetFound_;

	// 最短距離にいる敵までの向きを計算
	void LookTargetEnemy(void);
};


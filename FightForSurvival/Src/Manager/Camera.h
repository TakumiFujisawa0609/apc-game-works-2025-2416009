#pragma once

#include <DxLib.h>
#include "../Object/Player/Player.h"

class Camera
{
public:

	// カメラクリップ：NEAR
	static constexpr float CAMERA_NEAR = 3.0f;
	// カメラクリップ：FAR
	static constexpr float CAMERA_FAR = 30000.0f;

	// ヒットストップで止める時間
	static constexpr int HIT_STOP_TIME = 20;

	// カメラモード
	enum class MODE
	{
		NONE,
		FIXED_POINT,	// 定点カメラ
		FREE,			// フリーモード
	};

	// コンストラクタ
	Camera(void);
	Camera(const Player* player);
	// デストラクタ
	~Camera(void);

	// 初期化
	void Init(void);
	// 更新
	void Update(void);
	// 描画前のカメラ設定
	void SetBeforeDraw(void);
	// デバッグ用描画
	void DrawDebug(void);
	// 解放
	void Release(void);

	// カメラモードの変更
	void ChangeMode(MODE mode);

	// ゲッター関数
	const VECTOR& GetPos(void)const { return pos_; }

	// セッター関数
	void SetHitStop(void) { hitStopCounter_ = HIT_STOP_TIME; }

private:

	const Player* player_;

	// カメラモード
	MODE mode_;

	// カメラの位置
	VECTOR pos_;

	// カメラの角度
	VECTOR angles_;

	// カメラ揺らし用のカウンタ
	int hitStopCounter_;

	// カメラモード別の制御処理
	void SetBeforeDrawFixedPoint(void);
	void SetBeforeDrawFree(void);

	// カメラの移動
	void MoveCamera(void);
	// カメラ振動
	void ShakeCamera(void);
};


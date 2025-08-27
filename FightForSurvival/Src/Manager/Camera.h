#pragma once

#include <DxLib.h>
#include "../Object/Player/Player.h"

class Camera
{
public:

	// カメラの初期位置
	static constexpr VECTOR DEFAULT_POS =
	{ 3.0f, 305.0f, 300.0f };
	// カメラクリップ：NEAR
	static constexpr float CAMERA_NEAR = 3.0f;
	// カメラクリップ：FAR
	static constexpr float CAMERA_FAR = 15000.0f;

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


	int hitStopCounter_;

	// カメラモード別の制御処理
	void SetBeforeDrawFixedPoint(void);
	void SetBeforeDrawFree(void);

	// カメラの移動
	void MoveCamera(void);
	// カメラ振動
	void ShakeCamera(void);
};


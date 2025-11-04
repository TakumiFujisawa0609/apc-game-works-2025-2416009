#pragma once

#include "../../../Application.h"
#include "../../Vector2/Vector2.h"

class Setting
{
public:

	// 衝突用サイズ
	static constexpr int COL_SIZE_X = 90;
	static constexpr int COL_SIZE_Y = 30;

	//　「High」の座標
	static constexpr int HIGH_POS_X = Application::SCREEN_SIZE_X / 2 + 200;
	static constexpr int HIGH_POS_Y = Application::SCREEN_SIZE_Y / 2;

	//　「Low」の座標
	static constexpr int LOW_POS_X = Application::SCREEN_SIZE_X / 2 - 200;
	static constexpr int LOW_POS_Y = Application::SCREEN_SIZE_Y / 2;

	//　「Done」の座標
	static constexpr int DONE_POS_X = Application::SCREEN_SIZE_X / 2 + 150;
	static constexpr int DONE_POS_Y = Application::SCREEN_SIZE_Y / 2 + 50;

	//　感度の最高設定
	static constexpr float SENSITIVITY_MAX_MOUSE = 0.01f;
	static constexpr float SENSITIVITY_MAX_PAD = 0.1f;

	//　感度の最低設定
	static constexpr float SENSITIVITY_MIN_MOUSE = 0.001f;
	static constexpr float SENSITIVITY_MIN_PAD = 0.01f;

	//　加算したり減算する感度の量
	static constexpr float SENSITIVITY_MOUSE = 0.0001f;
	static constexpr float SENSITIVITY_PAD = 0.001f;

	// バーの始まりの位置
	static constexpr int BAR_START_POS_X = Application::SCREEN_SIZE_X / 2 - 160;
	static constexpr int BAR_START_POS_Y = Application::SCREEN_SIZE_Y / 2 - 5;
	// バーの終わりの位置
	static constexpr int BAR_END_POS_X = Application::SCREEN_SIZE_X / 2 + 180;
	static constexpr int BAR_END_POS_Y = Application::SCREEN_SIZE_Y / 2 + 5;

	// バーの円のY座標
	static constexpr int CIRCLE_POS_Y = 310;
	// バーの円の半径
	static constexpr int CIRCLE_RAD = 15;

	Setting(void);
	~Setting(void);

	void Load(void);
	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	bool GetIsSetting(void) { return isSetting_; }
	void SetIsSetting(bool flg) { isSetting_ = flg; }

private:

	// 「Done」の座標
	Vector2 pos_;

	// マウス座標
	Vector2 mousePos_;

	bool isSetting_;

	//	「Done」を選択中か
	bool isDone_;

	// 格納する感度
	float mouseSensitivity_;
	float padSensitivity_;

	bool isDrag_;

	// 確定させる
	void Confirm(void);

	// カーソルの選択処理
	void MouseSelect(void);

	// パッドの選択処理
	void PadSelect(void);

	// 感度設定のバーの処理
	void BarUpdate(void);

	// マウスで感度設定
	void MouseBarUpdate(void);

	// ゲームパッドで感度設定
	void PadBarUpdate(void);
};


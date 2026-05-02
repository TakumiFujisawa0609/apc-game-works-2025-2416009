#pragma once

#include "../../../Application.h"
#include "../../Vector2/Vector2.h"

class UIManager;
class TextureManager;

class Setting
{
public:

	// 衝突用サイズ
	static constexpr int COL_SIZE_X = 200;
	static constexpr int COL_SIZE_Y = 64;

	// 「Done」の座標
	static constexpr int DONE_POS_X = 840;
	static constexpr int DONE_POS_Y = 520;

	// 感度の最高設定
	static constexpr float SENSITIVITY_MAX_MOUSE = 0.01f;
	static constexpr float SENSITIVITY_MAX_PAD = 0.1f;

	// 感度の最低設定
	static constexpr float SENSITIVITY_MIN_MOUSE = 0.001f;
	static constexpr float SENSITIVITY_MIN_PAD = 0.01f;

	// 感度の範囲
	static constexpr float SENSITIVITY_RANGE_MOUSE = SENSITIVITY_MAX_MOUSE - SENSITIVITY_MIN_MOUSE;
	static constexpr float SENSITIVITY_RANGE_PAD = SENSITIVITY_MAX_PAD - SENSITIVITY_MIN_PAD;

	// 感動調整数値
	static constexpr int SENSITIVITY_ADJUST_PAD = 10;

	// 加算したり減算する感度の量
	static constexpr float SENSITIVITY_PAD = 0.001f;

	// バーの始まりの位置
	static constexpr int BAR_START_POS_X = 384;
	static constexpr int BAR_START_POS_Y = 350;
	// バーの終わりの位置
	static constexpr int BAR_END_POS_X = 897;
	static constexpr int BAR_END_POS_Y = 366;

	// バーの横軸間の距離
	static constexpr int BAR_DISTANCE_X = BAR_END_POS_X - BAR_START_POS_X;

	// バーの横軸間のオフセット
	static constexpr int BAR_DISTANCE_OFFSET = BAR_DISTANCE_X / 9;

	// バーの円のY座標
	static constexpr int CIRCLE_POS_Y = 356;
	// バーの円の半径
	static constexpr int CIRCLE_RAD = 18;

	// つまみの色
	// 選択中の時の色
	static constexpr unsigned int SELECTED_COLOR = 0x3b3b3b;

	// 選択中ではない時の色
	static constexpr unsigned int UNSELECTED_COLOR = 0x7c7c7c;

	Setting(void);
	~Setting(void);

	void Load(void);
	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	bool GetIsSetting(void) { return isSetting_; }
	void SetIsSetting(bool flg);

private:

	// UI
	UIManager* uiMgr;
	TextureManager* texMgr;

	// 「Done」の座標
	Vector2 pos_;

	// マウス座標
	Vector2 mousePos_;
	Vector2 circlePos_;

	// 感度設定中か　true / 設定中 , false / 設定中じゃない
	bool isSetting_;

	// 「Done」を選択中か
	bool isDone_;

	// 格納する感度
	float mouseSensitivity_;
	float padSensitivity_;

	// つまみを掴んでるか
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

	// 「Done」の変更が行われていたら処理を行う
	void CompDone(const bool prevDone);
};


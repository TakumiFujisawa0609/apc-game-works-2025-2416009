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

	//　「Done」の座標
	static constexpr int DONE_POS_X = 840;
	static constexpr int DONE_POS_Y = 520;

	//　感度の最高設定
	static constexpr float SENSITIVITY_MAX_MOUSE = 0.01f;
	static constexpr float SENSITIVITY_MAX_PAD = 0.1f;

	//　感度の最低設定
	static constexpr float SENSITIVITY_MIN_MOUSE = 0.001f;
	static constexpr float SENSITIVITY_MIN_PAD = 0.01f;

	//　加算したり減算する感度の量
	static constexpr float SENSITIVITY_PAD = 0.001f;

	// バーの始まりの位置
	static constexpr int BAR_START_POS_X = 384;
	static constexpr int BAR_START_POS_Y = 350;
	// バーの終わりの位置
	static constexpr int BAR_END_POS_X = 897;
	static constexpr int BAR_END_POS_Y = 366;

	// バーの円のY座標
	static constexpr int CIRCLE_POS_Y = 356;
	// バーの円の半径
	static constexpr int CIRCLE_RAD = 18;

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


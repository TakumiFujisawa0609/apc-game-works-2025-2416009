#pragma once

#include "../Vector2/Vector2.h"
#include "../../Application.h"

class Setting;

class Pause
{
public:

	// 衝突用サイズ
	static constexpr int COL_SIZE_X = 256;
	static constexpr int COL_SIZE_Y = 64;

	// 基準座標
	static constexpr int POS_X = Application::SCREEN_SIZE_X / 2 - COL_SIZE_X / 2;
	static constexpr int POS_Y = Application::SCREEN_SIZE_Y / 2 - 130;

	// 画像間(余白)の大きさ
	static constexpr int SPACE_Y = COL_SIZE_Y + 50;

	// 描画枚数
	static constexpr int DRAW_NUM = 3;

	// アルファ値
	static constexpr int ALPHA = 200;

	enum class PAUSE
	{
		CONTINUE,
		SETTING,
		TITLE,
		NON,

	};

	// ボタンの状態
	enum BUTTON_STATE
	{
		DEFAULE,		// 通常
		HOVER,			// 選択状態
		TRIGGER_DOWN,	// ボタン押下

		MAX,		// 最大数
	};

	// コンストラクタ
	Pause(void);
	// デストラクタ
	~Pause(void);

	// ロード関連
	void Load(void);
	// 初期化
	void Init(void);
	// 更新
	void Update(void);
	// 描画
	void Draw(void);
	// 解放
	void Release(void);

	bool GetPauseMode(void)const { return pauseMode_; }

	void ChangePause(PAUSE pause) { pause_ = pause; }

private:
	// 座標
	Vector2 pos_[DRAW_NUM];

protected:

	// 設定のインスタンスポインタ
	Setting* setting_;

	// ポーズモードの種類
	PAUSE pause_;

	// 画像
	// 下地
	int baseHandle_[static_cast<int>(BUTTON_STATE::MAX)];
	// テキスト
	int textHandle_[static_cast<int>(PAUSE::NON)];
	// ボタンの状態
	BUTTON_STATE buttonState_[static_cast<int>(PAUSE::NON)];
	// 今押されたか
	bool isTrgDown_[static_cast<int>(PAUSE::NON)];

	bool pauseMode_;

	bool isSetting_;

	void StartPause(void);

	// カーソルの選択処理
	virtual void MouseSelect(void);

	// パッドの選択処理
	virtual void PadSelect(void);
	
	// 確定時の遷移処理
	void Confirm(void);
};


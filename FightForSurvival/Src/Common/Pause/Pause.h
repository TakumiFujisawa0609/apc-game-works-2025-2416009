#pragma once

#include "../Vector2/Vector2.h"
#include "../../Application.h"

class Setting;
class UIManager;
class TextureManager;

class Pause
{
public:

	// 衝突用サイズ
	static constexpr int COL_SIZE_X = 256;
	static constexpr int COL_SIZE_Y = 64;

	// 基準座標
	static constexpr int POS_X = 512;
	static constexpr int POS_Y = 200;

	// 画像間(余白)の大きさ
	static constexpr int SPACE_Y = 160;

	// 描画枚数
	static constexpr int DRAW_NUM = 3;

	// アルファ値
	static constexpr int ALPHA = 200;

	// ポーズモードの選択できる種類
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

	// ポーズモード中か　true / ポーズモード中 , false / ポーズモード中ではない
	bool GetPauseMode(void)const { return pauseMode_; }

	// ポーズモードの選択種類変更
	void ChangePause(PAUSE pause) { pause_ = pause; }

private:
	// 座標
	Vector2 pos_[DRAW_NUM];

protected:

	// 設定のインスタンスポインタ
	Setting* setting_;

	// UI
	UIManager* uiMgr;
	TextureManager* texMgr;

	// ポーズモードの種類
	PAUSE pause_;

	// ポーズモードかどうか　true / ポーズ中 , false / ポーズ中でない
	bool pauseMode_;

	// 感度設定中かどうか
	bool isSetting_;

	// ダウントリガー判定用
	bool isTrgDown_[static_cast<int>(PAUSE::NON)];

	// ポーズモードを起動する
	void ChangePauseMode(void);

	// カーソルの選択処理
	virtual void MouseSelect(void);

	// パッドの選択処理
	virtual void PadSelect(void);
	
	// 確定時の遷移処理
	void Confirm(void);

	// 選択中のものの変更が行われているかつ何かを選択していたら処理を行う
	void CompPause(const PAUSE prevPause);

	// デバック用描画
	void DebugDraw(void);
};


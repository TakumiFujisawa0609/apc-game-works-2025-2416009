#pragma once

#include "../Vector2/Vector2.h"
#include "../../Application.h"

class Pause
{
public:

	// 画像サイズ
	static constexpr int IMAGE_SIZE_X = 300;
	static constexpr int IMAGE_SIZE_Y = 200;
	// 衝突用サイズ
	static constexpr int COL_SIZE_X = 90;
	static constexpr int COL_SIZE_Y = 30;

	// コンテニューの座標
	static constexpr int CONTINUE_POS_X = Application::SCREEN_SIZE_X / 2 - 30;
	static constexpr int CONTINUE_POS_Y = Application::SCREEN_SIZE_Y / 2 - 15;

	// タイトルの座標
	static constexpr int TITLE_POS_X = Application::SCREEN_SIZE_X / 2 - 30;
	static constexpr int TITLE_POS_Y = Application::SCREEN_SIZE_Y / 2 + 15;

	// 描画枚数
	static constexpr int DRAW_NUM = 3;

	enum class PAUSE
	{
		CONTINUE,
		TITLE,
		NON,

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
	// ポーズモードの種類
	PAUSE pause_;

	// 画像
	//int images_[DRAW_NUM];

	bool pauseMode_;

	void StartPause(void);

	// 確定させる
	virtual void Confirm(void);

	// カーソルの選択処理
	virtual void Collision(void);

	// パッドの選択処理
	virtual void PadSelect(void);
};


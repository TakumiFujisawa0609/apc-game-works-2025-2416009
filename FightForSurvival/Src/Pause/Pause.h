#pragma once

#include "../Base.h"
#include "../Vector2F.h"
#include "../Vector2.h"
#include "../Application/Application.h"

class Pause
{
public:

	// 画像サイズ
	static constexpr int IMAGE_SIZE_X = 300;
	static constexpr int IMAGE_SIZE_Y = 200;
	// 衝突用サイズ
	static constexpr int COL_SIZE_X = 300;
	static constexpr int COL_SIZE_Y = 100;

	// コンテニューの座標
	static constexpr float CONTINUE_POS_X = Application::SCREEN_SIZE_X / 2 - IMAGE_SIZE_X / 2;
	static constexpr float CONTINUE_POS_Y = Application::SCREEN_SIZE_Y / 2 - IMAGE_SIZE_Y / 2;

	// タイトルの座標
	static constexpr float TITLE_POS_X = Application::SCREEN_SIZE_X / 2 - IMAGE_SIZE_X / 2;
	static constexpr float TITLE_POS_Y = Application::SCREEN_SIZE_Y / 2;

	// 描画枚数
	static constexpr int DRAW_NUM = 3;

	enum class PAUSE
	{
		CONTINUE,
		TITLE,
		NON,

		SELECT,
	};

	// コンストラクタ
	Pause(void);
	// デストラクタ
	~Pause(void);

	// 初期化
	virtual void Init(void);
	// ロード関連
	virtual void Load(void);
	// 更新
	virtual void Update(void);
	// 描画
	virtual void Draw(void);
	// 解放
	virtual void Release(void);

	// ゲッター関数
	virtual bool GetPauseMode(void)const { return pauseMode_; }

	Vector2 GetJoystickPos(void)const { return joystickPos_; }

	void SetJoystickPos(Vector2 pos) { joystickPos_ = pos; }

private:
	// 座標
	Vector2F pos_[DRAW_NUM];

protected:
	// ポーズモードの種類
	PAUSE pause_;

	// 操作タイプ
	OPERATION_TYPE operationType_;

	// マウス座標
	Vector2 mousePos_;
	// ジョイスティックの座標
	Vector2 joystickPos_;	

	// 画像
	int images_[DRAW_NUM];

	// ポーズモード中か確認
	bool pauseMode_;

	int cursorImage_;

	// 矩形と点の当たり判定
	bool RectangleAndPoint(Vector2F pos1, int wid1, int hig1, Vector2 pos2);

	// 確定させる
	virtual void Confirm(void);

	// ポーズモードを開始させる
	void StartPause(void);

	// カーソルの選択処理
	virtual void InputDevisUpdate(Vector2 pos);

	// カーソルの描画
	void DrawCursor(Vector2 pos);
};


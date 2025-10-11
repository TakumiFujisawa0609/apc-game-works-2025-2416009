#pragma once

#include "SceneBase.h"
#include "../Common/Vector2.h"
#include "../Application.h"

class Camera;

class TitleScene : public SceneBase
{
public:

	// 衝突用サイズ
	static constexpr int COL_SIZE_X = 90;
	static constexpr int COL_SIZE_Y = 30;

	// コンテニューの座標
	static constexpr int GAMESTART_POS_X = Application::SCREEN_SIZE_X / 2 - 50;
	static constexpr int GAMESTART_POS_Y = Application::SCREEN_SIZE_Y - 210;

	// タイトルの座標
	static constexpr int EXIT_POS_X = Application::SCREEN_SIZE_X / 2 - 50;
	static constexpr int EXIT_POS_Y = Application::SCREEN_SIZE_Y - 180;

	enum STATE
	{
		GAMESTART,
		EXIT,
		NON,

	};

	// コンストラクタ
	TitleScene(void);

	// デストラクタ
	~TitleScene(void);

	void Load(void) override;
	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:
	STATE state_;

	// カメラ
	Camera* camera_;

	// 画像
	int imgTitle_;
	// 座標
	Vector2 pos_[STATE::NON];

	// ゲームスタートフラグ
	bool isPrevStart_;
	bool isNowStart_;

	// 確定させる
	void Confirm(void);

	// カーソルの選択処理
	void Collision(void);
};

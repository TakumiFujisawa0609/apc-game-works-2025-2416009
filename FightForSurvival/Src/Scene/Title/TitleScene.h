#pragma once

#include "../Base/SceneBase.h"
#include "../../Common/Vector2/Vector2.h"
#include "../../Application.h"

class Camera;
class UIManager;
class TextureManager;

class TitleScene : public SceneBase
{
public:

	// 衝突用サイズ
	static constexpr int COL_SIZE_X = 610;
	static constexpr int COL_SIZE_Y = 60;

	// コンテニューの座標
	static constexpr int GAMESTART_POS_X = 335;
	static constexpr int GAMESTART_POS_Y = 470;

	// タイトルの座標
	static constexpr int EXIT_POS_X = 335;
	static constexpr int EXIT_POS_Y = 560;

	enum STATE
	{
		CLICK,
		GAMESTART,
		END,
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

	void ChangeState(STATE state) { state_ = state; }

private:
	STATE state_;

	// カメラ
	Camera* camera_;

	// UI
	UIManager* uiMgr_;
	TextureManager* texMgr_;

	// 座標
	Vector2 pos_[STATE::NON];

	// 確定させる
	void Confirm(void);

	// マウスの選択処理
	void MouseSelect(void);

	// パッドの選択処理
	void PadSelect(void);
};

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

	// 動画へ進む秒数
	static constexpr int MOVIE_START_TIME = 10 * 60;

	enum STATE
	{
		CLICK,		// PushStartKeyのみの時
		GAMESTART,	// GameStartとEndが選択できる時
		END,
		NON,

	};

	// コンストラクタ
	TitleScene(void);

	// デストラクタ
	~TitleScene(void);

	// 読み込み処理
	void Load(void) override;
	// 初期化処理
	void Init(void) override;
	// 更新処理
	void Update(void) override;
	// 描画処理
	void Draw(void) override;
	// 解放処理
	void Release(void) override;

	// 指定された状態へ変更
	void ChangeState(STATE state) { state_ = state; }

private:

	// ステート
	STATE state_;

	// UI
	UIManager* uiMgr_;
	TextureManager* texMgr_;

	// 座標
	Vector2 pos_[STATE::NON];

	// 操作が行われていないときの動画を流すまでのカウント
	int idleFrameCount_;

	// 確定させる
	void Confirm(void);

	// マウスの選択処理
	void MouseSelect(void);

	// パッドの選択処理
	void PadSelect(void);

	// 何も選択されていない状態から選択されたらSEを流す
	void PlaySelectSE(STATE prevState);

	// 動画を流す用の処理
	bool StartMovie(void);

	// ステートによって描画物の表示の有無を変更
	void StateDraw(void);

	// デバッグ用描画
	void DebugDraw(void);
};

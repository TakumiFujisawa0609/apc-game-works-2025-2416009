#pragma once

#include "SceneBase.h"

class Camera;
class Grid;
class Player;
class Cursor;
class Score;
class Pause;

class GameScene : public SceneBase
{

public:

	// コンストラクタ
	GameScene(void);

	// デストラクタ
	~GameScene(void);

	void Init(void) override;
	void Load(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

	Camera* GetCameraPoint(void) { return camera_; }

private:
	// カメラ
	Camera* camera_;

	// グリッド線
	Grid* grid_;

	// オブジェクト
	// プレイヤー
	Player* player_;
	
	// カーソル
	Cursor* cursor_;

	// スコア
	Score* score_;

	// ポーズモード
	Pause* pause_;

	// ポーズモード確認用
	bool nowPause_;
	bool prevPause_;

	void CheckCollisions(void);

	// ゲームクリア・ゲームオーバー遷移条件
	void IsClear(void);
	void IsOver(void);
};


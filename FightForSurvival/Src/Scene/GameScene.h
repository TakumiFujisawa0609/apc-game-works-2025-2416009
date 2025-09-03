#pragma once

#include "SceneBase.h"

class Camera;
class Grid;
class Player;
class Cursor;
class EnemyBase;

class GameScene : public SceneBase
{

public:

	// コンストラクタ
	GameScene(void);

	// デストラクタ
	~GameScene(void);

	void Load(void) override;
	void Init(void) override;
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

	// 敵
	EnemyBase* enemy_;
	int enemyId_;

	void CheckCollisions(void);
};


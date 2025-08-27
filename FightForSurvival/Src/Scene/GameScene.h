#pragma once

#include "SceneBase.h"

class Camera;
class Grid;
class ObjectBase;

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

private:
	// カメラ
	Camera* camera_;

	// グリッド線
	Grid* grid_;

	// オブジェクト
	// プレイヤー
	ObjectBase* player_;


};


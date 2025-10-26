#pragma once

#include "../Base/SceneBase.h"

class Camera;
class Player;
class Cursor;
class Score;
class Pause;
class RedDamageEffect;
class SkyDome;

class GameScene : public SceneBase
{

public:

	enum class STATE
	{
		PLAY,
		UPGRADE,
	};

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

	void ChangeState(STATE state) { state_ = state; }

private:
	// カメラ
	Camera* camera_;

	// オブジェクト
	// プレイヤー
	Player* player_;
	
	// カーソル
	Cursor* cursor_;

	// スコア
	Score* score_;

	// ポーズモード
	Pause* pause_;

	// エフェクト
	RedDamageEffect* redEffect_;

	// スカイドーム
	SkyDome* skydome_;

	// ポーズモード確認用
	bool nowPause_;
	bool prevPause_;

	// ステート
	STATE state_;

	// 当たり判定
	void Collisions(void);
	// 敵やプレイヤーにダメージが入る当たり判定
	void DamageCollision(void);
	// 敵やプレイヤーの押し出し判定
	void ExtrusionCollision(void);

	// ゲームクリア・ゲームオーバー遷移条件
	void IsClear(void);
	void IsOver(void);

	// アップグレードモードにする条件
	void StartUpgrade(void);
	// アップグレードモードを終了させる条件
	void StopUpgrade(void);

};


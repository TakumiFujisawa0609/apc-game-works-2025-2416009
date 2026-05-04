#pragma once

#include "../Base/SceneBase.h"

class Camera;
class Player;
class Reticle;
class Score;
class Pause;
class DamageEffect;
class SkyDome;
class EnemyBase;
class Dragon;
class Stage;
class UIManager;
class TextureManager;

class GameScene : public SceneBase
{

public:

	// ダメージエフェクトのアルファ値
	static constexpr int DAMAGE_EFFECT_ALPHA = 128;
	// ダメージエフェクトのカラー値
	static constexpr unsigned int DAMAGE_EFFECT_COLOR= 0xff0000;

	// ステート
	enum class STATE
	{
		PLAY,		// 通常時(戦っている状態)
		UPGRADE,	// 自己強化時(自己強化中の状態)
	};

	// コンストラクタ
	GameScene(void);
	// デストラクタ
	~GameScene(void)override;

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
	// カメラ
	Camera* camera_;

	// オブジェクト
	// プレイヤー
	Player* player_;
	
	// カーソル
	Reticle* reticle_;

	// スコア
	Score* score_;

	// ポーズモード
	Pause* pause_;

	// エフェクト
	DamageEffect* redEffect_;

	// スカイドーム
	SkyDome* skydome_;

	// ステージ
	Stage* stage_;

	// UI
	UIManager* uiMgr_;
	TextureManager* texMgr_;

	// ポーズモード確認用
	bool nowPause_;
	bool prevPause_;

	// ステート
	STATE state_;

	// 当たり判定
	void Collisions(void);

	// 魔法の当たり判定
	void MagicCollision(void);
	// 爆発魔法の当たり判定
	void ExplosionMagicCollision(void);

	// 敵の攻撃の当たり判定
	void EnemiesAttackCollision(void);
	// ゾンビの攻撃当たり判定
	void ZombieAttackCollision(EnemyBase* enemy);
	// ドラゴンの攻撃当たり判定
	void DragonAttackCollision(EnemyBase* enemy);
	// 敵の魔法の攻撃当たり判定
	void EnemyMagicCollision(void);
	// ドラゴンの前方攻撃当たり判定
	void DragonForwardAttackCollision(EnemyBase* enemy);
	// ドラゴンの突進攻撃当たり判定
	void DragonRushAttackCollision(EnemyBase* enemy);

	// 敵同士の押し出し判定
	void EnemiesExtrusionCollision(void);

	// ステージとプレイヤーの当たり判定
	void StageAndPlayerCollision(void);
	// ステージとプレイヤーの攻撃の当たり判定(スポナーも)
	void StageAndAttackCollision(void);

	// ステージと敵の当たり判定
	void StageAndEnemiesCollision(void);

	// ゲームクリア・ゲームオーバー遷移条件
	void IsClear(void);
	void IsOver(void);

	// アップグレードモードにする条件
	void StartUpgrade(void);
	// アップグレードモードを終了させる条件
	void StopUpgrade(void);

	// ダメージ時エフェクトやSE処理
	void Damage(void);

	// 敵からの物理攻撃を受けた場合の処理
	void EnemyPhysicalAttackDamage(EnemyBase* enemy);

	// デバッグ時の更新
	void DebugUpdate(void);
};


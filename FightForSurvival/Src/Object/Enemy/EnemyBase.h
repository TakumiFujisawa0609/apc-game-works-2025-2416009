#pragma once

#include "../../Base.h"
#include "EnemyState.h"

class AnimationController;
class Player;

class EnemyBase
{
public:

	// 攻撃が確定するフレーム数
	static constexpr int ATTACK_FRAME_TIMMING = 10;

	// エネミー種別
	enum class ENEMY_TYPE
	{
		ZOMBIE,
		WIZARD,
		GIANT,
		MAX,
	};

	// コンストラクタ
	EnemyBase(void);

	// デストラクタ
	virtual ~EnemyBase(void);

	// 初期化処理
	virtual void Init(ENEMY_TYPE type, int baseModelId, int baseAttackEffectModelId, VECTOR pos, Player* player);

	// 更新
	virtual void Update(void);

	// 描画処理
	virtual void Draw(void);

	// 解放処理
	void Release(void);

	// 敵の情報を渡す
	const Unit GetEnemy(void)const { return enemy_; }
	// 敵の当たり判定座標の情報を渡す
	CollisionPos GetColPos(void)const { return colPos_; }

	// 死亡していなかったらtrueを返す
	bool IsCollisionState(void);

	// HPにダメージを与える
	void SubHp(int hp);

	// 状態遷移
	void ChangeState(ENEMY_STATE newState);

	// 状態を返却
	ENEMY_STATE GetState() const { return state_; }

protected:
	AnimationController* animationController_;
	Player* player_;

	// プレイヤーの情報
	Unit enemy_;

	// エフェクト用モデルハンドルID
	int baseAttackEffectModelId_;

	// 敵の種類
	ENEMY_TYPE type_;

	// ステート
	ENEMY_STATE state_;

	// 状態のテーブル（派生クラスでセットする）
	EnemyStateFunction stateTable_[ENEMY_STATE_MAX];

	// 当たり判定用(頭、体)
	CollisionPos colPos_;

	// 状態別更新
	// 移動処理
	static void Chase(EnemyBase& enemy);
	// 後退処理
	static void Retreat(EnemyBase& enemy);
	// ダメージ時のリアクション処理
	static void Hit(EnemyBase& enemy);
	// 死亡時のリアクション処理
	static void Dead(EnemyBase& enemy);
	// 完全死亡
	static void End(EnemyBase& enemy);

	// プレイヤー側に向く処理
	void LookPlayer(void);

	// 当たり判定用の座標を更新
	void UpdateCollisionPositions(void);

	// パラメータ設定
	virtual void SetParam(void) = 0;
	// アニメーション登録
	virtual void AddAnimation(void) = 0;
};


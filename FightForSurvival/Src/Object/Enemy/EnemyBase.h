#pragma once

#include "../../Base.h"

class AnimationController;
class Player;

class EnemyBase
{
public:

	// 最大HP
	static constexpr int DEFAULT_HP = 10;

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

	// アニメーション種別
	enum class ANIM_TYPE
	{
		IDLE,
		WALK,
		PUNCH,
		HIT_REACT,
		DEATH,
	};

	enum class STATE
	{
		IDLE,
		MOVE,
		PUNCH,
		MAGIC,
		AREA,
		HIT_REACT,
		DEAD_REACT,
		END,
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

	const Unit GetEnemy(void)const { return enemy_; }

	void SetHp(int hp) { enemy_.hp_ = hp; }

	void ChangeState(STATE state);

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
	STATE state_;

	// 攻撃フレームカウント
	int attackFrameCnt_;

	// パラメータ設定
	virtual void SetParam(void) = 0;

	// 状態遷移
	virtual void ChangeIdle(void);
	virtual void ChangeMove(void);
	virtual void ChangePunch(void);
	virtual void ChangeMagic(void);
	virtual void ChangeArea(void);
	virtual void ChangeHitReact(void);
	virtual void ChangeDeadReact(void);
	virtual void ChangeEnd(void);

	// 状態別更新
	// 待機処理
	virtual void IdleUpdate(void);
	// 移動処理
	virtual void MoveUpdate(void);
	// 攻撃処理
	virtual void PunchUpdate(void);
	virtual void MagicUpdate(void);
	virtual void AreaUpdate(void);
	// ダメージ時のリアクション処理
	void HitReactUpdate(void);
	// 死亡時のリアクション処理
	void DeadReactUpdate(void);
	// 完全死亡
	void EndUpdate(void);

	// プレイヤー側に向く処理
	void LookPlayer(void);

	virtual void AddAnimation(void) = 0;
};


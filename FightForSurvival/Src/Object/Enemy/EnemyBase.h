#pragma once

#include "../../Base.h"
#include "EnemyState.h"
#include "EnemyManager.h"
#include <string>

class AnimationController;
class Player;


class EnemyBase
{
public:

	static constexpr float ATTACK_RANGE_POS_OFFSET = 80.0f;

	// 左右移動の左右を変更するタイミング
	static constexpr float CHANGE_RATE_MAX = 1.0f;
	static constexpr float CHANGE_RATE_MIN = -1.0f;

	// 座標更新のタイミング(5フレームごとに1回)
	static constexpr int COLLISION_UPDATE_INTERVAL = 5;

	// コンストラクタ
	EnemyBase(ENEMY_TYPE type, int baseModelId, int baseAttackEffectModelId, Player* player);

	// デストラクタ
	virtual ~EnemyBase(void);

	// 初期化処理
	virtual void CreateEnemy(VECTOR pos);

	// 更新
	virtual void Update(void);

	// 描画処理
	virtual void Draw(void);

	// 解放処理
	void Release(void);

	// 敵の情報を渡す
	const Unit GetEnemy(void)const { return enemy_; }
	// 敵の当たり判定座標の情報を渡す
	EnemyCollision GetColPos(void)const { return collision_; }

	// 敵の種類を渡す
	ENEMY_TYPE GetType(void)const{return type_;}

	// 死亡していなかったらtrueを返す
	bool IsCollisionState(void);
	// 攻撃状態かつ、当たり判定を終えていなかったらtrueを返す
	bool IsAttack(void)const { return state_ == STATE_ATTACK && isAttack_ == true; }
	// 攻撃有効フラグ
	void SetIsAttack(bool flg) { isAttack_ = flg; }

	// HPにダメージを与える
	void SubHp(int hp);

	// 状態遷移
	void ChangeState(ENEMY_STATE newState);

	AnimationController* GetAnimationController(void)const { return animationController_; }

	// 状態を返却
	ENEMY_STATE GetState() const { return state_; }

	// 攻撃範囲内か確認用処理
	bool SearchAttackRange(void);

	// 攻撃待ち時間を確認
	float GetAttackCooldown(void) const{ return attackCooldown_; }
	// 攻撃待ち時間のセットする
	void SetAttackCooldown(float cooldown) { attackCooldown_ = cooldown; }

	void Extrusion(VECTOR overlap);

protected:
	AnimationController* animationController_;
	Player* player_;

	// プレイヤーの情報
	Unit enemy_;

	// エフェクト用モデルハンドルID
	int baseAttackEffectModelId_;

	// 撃破スコア
	int score_;

	// 敵の種類
	ENEMY_TYPE type_;

	// ステート
	ENEMY_STATE state_;

	// 状態のテーブル（派生クラスでセットする）
	EnemyStateFunction stateTable_[ENEMY_STATE_MAX];

	// 当たり判定用(頭、体)
	EnemyCollision collision_;

	// 攻撃可能範囲用座標
	VECTOR attackRangePos_;
	float attackRange_;

	// 攻撃待ち時間
	float attackCooldown_;

	// 攻撃中か
	bool isAttack_;

	// 左右フラグ(true / 敵から見て左進むようにする, false / 敵から見て右に進むようにする)
	bool isLeftFlg_;

	// 左右移動量
	float leftAndRightRate_;

	// 座標更新のタイミング
	int updateCollPosCounter_;

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

	// ボーンのワールド座標を取得する
	VECTOR GetBoneWorldPosition(int bone,float offset);

	// ボーンフレーム取得
	int SearchFrame(const std::string& boneName);

	// パラメータ設定
	virtual void SetParam(void) = 0;
	// アニメーション登録
	virtual void AddAnimation(void) = 0;
	// フレーム登録
	virtual void AddFrames(void) = 0;

	// 左右移動
	void MoveLeftAndRight(void);
};


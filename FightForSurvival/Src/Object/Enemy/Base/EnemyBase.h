#pragma once

#include "../../../Base.h"
#include "../EnemyInfo.h"
#include "../EnemyManager.h"
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

	// 重力
	static constexpr float GRAVITATION = -0.3f;
	// 最大重力
	static constexpr float MAX_GRAVITATION = -9.8f;

	// コンストラクタ
	EnemyBase(ENEMY_TYPE type, int baseModelId, Player* player);

	// デストラクタ
	virtual ~EnemyBase(void);

	// 初期化処理
	virtual void CreateEnemy(VECTOR pos);

	// 更新
	virtual void Update(void);

	// 描画処理
	virtual void Draw(void);

	// 解放処理
	virtual void Release(void);

	// 敵の情報を渡す
	const Unit GetEnemy(void)const { return enemy_; }
	// 敵の当たり判定座標の情報を渡す
	EnemyCollision GetColPos(void)const { return collision_; }

	// 敵の種類を渡す
	ENEMY_TYPE GetType(void) const { return type_;}

	// 死亡していなかったらtrueを返す
	bool IsCollisionState(void);
	// 攻撃状態かつ、当たり判定を終えていなかったらtrueを返す
	bool IsAttack(void)const { return state_.state_ == STATE_ATTACK && attack_.isAttacking_ == true; }
	// 攻撃有効フラグ
	void SetIsAttack(bool flg) { attack_.isAttacking_ = flg; }

	// HPにダメージを与える
	void SubHp(float damage);

	// 状態遷移
	void ChangeState(ENEMY_STATE newState);

	AnimationController* GetAnimationController(void)const { return animationController_; }

	// 状態を返却
	enemieStateCtrl GetStateInfo(void) const { return state_; }

	// 攻撃範囲内か確認用処理
	virtual bool SearchAttackRange(void);

	// 攻撃待ち時間を確認
	float GetAttackCooldown(void) const{ return attack_.cooldown_; }
	// 攻撃待ち時間のセットする
	void SetAttackCooldown(float cooldown) { attack_.cooldown_ = cooldown; }

	// 衝突判定
	virtual void CollisionStage(VECTOR pos);
	virtual void CollisionStage(float posY);
	// 押し出し処理
	void Extrusion(VECTOR overlap);

	// プレイヤー側に向く処理
	void LookPlayer(void);
	// 前方方向に攻撃をする
	void CreateMagicForward(void);

protected:
	// アニメーションハンドル
	AnimationController* animationController_;

	// プレイヤーハンドル
	Player* player_;

	// プレイヤーの情報
	Unit enemy_;

	// 撃破スコア(敵ごとにスコア数を変更するため)
	int score_;

	// 敵の種類
	ENEMY_TYPE type_;

	// 敵の状態情報
	enemieStateCtrl state_;

	// 当たり判定用(頭、体)
	EnemyCollision collision_;

	// 攻撃用の情報
	EnemyAttack attack_;

	// 左右移動用の情報
	EnemyMove move_;

	// 座標更新のタイミング
	int updateCollPosCounter_;

	// アニメーション用のスピード(アニメーションと連動させる必要ある)
	std::vector<float> speed_;

	// 魔法の相対座標
	VECTOR relativeMagicPos_;

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

	// 当たり判定用の座標を更新
	void UpdateCollisionPositions(void);

	// ボーンのワールド座標を取得する
	VECTOR GetBoneWorldPosition(int bone,float offset);

	// アニメーション登録
	virtual void DuplicateAnimation(std::vector<float> speed, std::vector<int> animModelIds);
	// パラメータ設定
	virtual void SetParam(void) = 0;
	// フレーム登録
	virtual void AddFrames(void) = 0;

	// 左右移動
	void MoveLeftAndRight(void);

	// ステート別アニメーション再生
	virtual void PlayAnim(void);

	// 重力処理
	void Gravity(void);

	// デバック用描画処理
	void DebugDraw(void);
};


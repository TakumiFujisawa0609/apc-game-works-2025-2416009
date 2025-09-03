#pragma once

#include "../GunBase.h"
class GunBase;

class BulletBase
{
protected:

	struct Bullet
	{
		// 弾のモデルID
		int modelId_;
		// 方向
		VECTOR dir_;
		// 弾の大きさ
		VECTOR scale_;
		// 弾の回転
		VECTOR rotate_;
		// 弾の座標
		VECTOR pos_;
		VECTOR prevPos_;
		// 弾の移動速度
		float speed_;
		// 弾の生存判定
		bool isAlive_;
		// 弾の生存期間
		float cntAlive_;
		// 弾の衝突判定用半径
		float collisionRadius_;
		// ダメージ数
		int headDamage_;
		int bodyDamage_;
	};

public:

	// 弾の状態
	enum class STATE
	{
		NONE,
		SHOT,
		BLAST,
		END
	};

	// コンストラクタ(銃種別、元となるモデルのハンドルID)
	BulletBase(int baseModelId);
	// デストラクタ
	virtual ~BulletBase(void);

	// 弾の生成(表示開始座標、弾の進行方向)
	virtual void CreateShot(VECTOR pos, VECTOR dir);
	// 更新ステップ
	virtual void Update(void);
	// 描画
	void Draw(void);
	// 解放処理
	void Release(void);

	// SHOT中のみtrueを返す
	bool IsCollisionState(void);

	void ChangeState(STATE state);

	Bullet GetBullet(void)const { return bullet_; }

	// 爆発させる
	//void Blast(EFFECT_TYPE type);

protected:
	Player* player_;

	// 弾の状態
	STATE state_;

	Bullet bullet_;

	// 爆発エフェクトのプレイハンドル
	//int effectBlastPlayId_;
	// エフェクトの種類
	//EFFECT_TYPE effectType_;

	// パラメータ設定
	virtual void SetParam(void) = 0;
	// 弾の生存期間の減少
	void ReduceCntAlive(void);

	void UpdateShot(void);
	void UpdateBlast(void);
	void UpdateEnd(void);

	void ChangeNon(void);
	void ChangeShot(void);
	void ChangeBlast(void);
	void ChangeEnd(void);

};


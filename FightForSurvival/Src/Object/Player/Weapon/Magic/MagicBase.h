#pragma once

#include "../WeaponBase.h"
class WeaponBase;

class MagicBase
{
protected:

	struct Magic
	{
		// 魔法のモデルID
		int modelId_;
		// 方向
		VECTOR dir_;
		// 魔法の大きさ
		VECTOR scale_;
		// 魔法の回転
		VECTOR rotate_;
		// 魔法の座標
		VECTOR pos_;
		VECTOR prevPos_;
		// 魔法の移動速度
		float speed_;
		// 魔法の生存判定
		bool isAlive_;
		// 魔法の生存期間
		float cntAlive_;
		// 魔法の衝突判定用半径
		float collisionRadius_;
		// ダメージ数
		int headDamage_;
		int bodyDamage_;
	};

public:

	// 魔法の状態
	enum class STATE
	{
		NONE,
		SHOT,
		BLAST,
		END
	};

	// コンストラクタ(杖種別、元となるモデルのハンドルID)
	MagicBase(int baseModelId);
	// デストラクタ
	virtual ~MagicBase(void);

	// 魔法の生成(表示開始座標、魔法の進行方向)
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

	Magic GetMagic(void)const { return magic_; }

	void SetCollisionRad(float col) { magic_.collisionRadius_ = col; }

	// 爆発させる
	//void Blast(EFFECT_TYPE type);

protected:
	Player* player_;

	// 魔法の状態
	STATE state_;

	Magic magic_;

	// 爆発エフェクトのプレイハンドル
	//int effectBlastPlayId_;
	// エフェクトの種類
	//EFFECT_TYPE effectType_;

	// パラメータ設定
	virtual void SetParam(void) = 0;
	// 魔法の生存期間の減少
	void ReduceCntAlive(void);

	void UpdateShot(void);
	void UpdateBlast(void);
	void UpdateEnd(void);

	void ChangeNon(void);
	void ChangeShot(void);
	void ChangeBlast(void);
	void ChangeEnd(void);

};


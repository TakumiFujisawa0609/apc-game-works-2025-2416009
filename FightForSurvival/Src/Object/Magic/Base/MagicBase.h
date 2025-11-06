#pragma once

#include "../../Player/Weapon/WeaponBase.h"
#include "../MagicInfo.h"
class WeaponBase;

class MagicBase
{
public:

	// 魔法をチャージする量
	static constexpr float CHARGE_POW = 0.1f;
	// 魔法の最大チャージ量
	static constexpr float CHARGE_MAX = 20.0f;

	// 最大チャージに加算する攻撃力
	static constexpr float ADD_DAMEGE = 5.0f;

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
	void Init(void);
	virtual void CreateShot(VECTOR pos, VECTOR dir);
	// 更新ステップ
	virtual void Update(void);
	// 描画
	void Draw(void);
	// 解放処理
	void Release(void);

	// 現在のステートを返す
	STATE GetState(void)const { return state_; }

	void ChangeState(STATE state);

	// ゲッター関数
	Magic GetMagic(void)const { return magic_; }
	// セッター関数
	void SetCollisionRad(float col) { magic_.collisionRadius_ = col; }
	void SetIsExists(bool flg) { magic_.isExists_ = flg; }

	// 魔法を大きくする
	void ChargeMagic(void);

	// 座標を更新
	void UpdatePos(VECTOR pos);

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

	// 魔法の更新(移動)処理
	virtual void UpdateShot(void);
	// 魔法の爆発処理
	void UpdateBlast(void);
	// 爆発後の魔法の処理
	void UpdateEnd(void);

	// ステート変更時の設定事項
	virtual void ChangeShot(void) = 0;
	virtual void ChangeBlast(void);
	void ChangeEnd(void);

};


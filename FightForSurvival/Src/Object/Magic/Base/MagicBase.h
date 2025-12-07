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
		CHARGE,
		SHOT,
		BLAST,
		END
	};

	// コンストラクタ(杖種別、元となるモデルのハンドルID)
	MagicBase(TYPE_MAGIC typeMagic, int baseModelId);
	// デストラクタ
	virtual ~MagicBase(void);

	// 魔法の初期化
	void Init(void);
	// 魔法の生成
	virtual void CreateShot(VECTOR pos, VECTOR dir);
	// 更新
	virtual void Update(void);
	// 描画
	void Draw(void);
	// 解放処理
	void Release(void);

	// ステートの変更
	void ChangeState(STATE state);

	// ゲッター関数
	STATE GetState(void)const { return state_; }
	Magic GetMagic(void)const { return magic_; }

	// セッター関数
	void SetCollisionRad(float col) { magic_.collisionRadius_ = col; }
	void SetIsExists(bool flg) { magic_.isExists_ = flg; }

	// 座標を更新
	void UpdatePos(VECTOR pos);
	// 向きを更新
	void UpdateDir(VECTOR dir);

	// 魔法の種類を返す
	TYPE_MAGIC GetTypeMagic(void)const {return magic_.typeMagic_;}

protected:
	Player* player_;

	// 魔法の状態
	STATE state_;

	Magic magic_;

	// 魔法をチャージする量
	float chargePow_ = 0.1f;
	// 魔法の最大チャージ量
	float chargeMax_ = 20.0f;

	// 攻撃エフェクトのプレイハンドル
	int effectPlayId_;
	// エフェクトのサイズ
	float effectScale_;

	// パラメータ設定
	virtual void SetParam(void) = 0;
	// ステート
	virtual void ChangeCharge(void) = 0;
	virtual void UpdateCharge(void);

	// 魔法の更新(移動)処理
	virtual void ChangeShot(void) = 0;
	virtual void UpdateShot(void);
	
	// 魔法の爆発処理
	virtual void ChangeBlast(void) = 0;
	void UpdateBlast(void);
	
	// 爆発後の魔法の処理
	void ChangeEnd(void);

	// 魔法の生存期間の減少
	void ReduceCntAlive(void);
};


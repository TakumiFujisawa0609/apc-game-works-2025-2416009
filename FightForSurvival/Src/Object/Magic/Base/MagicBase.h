#pragma once

#include "../../Player/Weapon/WeaponBase.h"
#include "../MagicInfo.h"
class WeaponBase;

class MagicBase
{
public:

	// コンストラクタ(杖種別、杖の座標ポインタ)
	MagicBase(TYPE_MAGIC typeMagic, VECTOR* weponPos);
	// デストラクタ
	virtual ~MagicBase(void);

	// 魔法の初期化
	void Init(void);
	// 魔法の生成
	virtual void ChargeShot(VECTOR pos, VECTOR dir);
	virtual void CreateShot(VECTOR pos, VECTOR dir);
	// 更新
	virtual void Update(void);
	// 描画
	void Draw(void);
	// 解放処理
	void Release(void);

	// ステートの変更
	void ChangeState(MAGIC_STATE state);

	// ゲッター関数
	MAGIC_STATE GetState(void)const { return state_; }
	Magic GetMagic(void)const { return magic_; }

	// セッター関数
	void SetCollisionRad(float col) { magic_.collisionRadius_ = col; }
	void SetIsExists(bool flg) { magic_.isExists_ = flg; }

	// 座標を更新
	void UpdateEffectPos(void);
	void UpdateEffectPos(VECTOR pos);
	// 向きを更新
	void UpdateEffectDir(VECTOR dir);

	// 魔法の種類を返す
	TYPE_MAGIC GetTypeMagic(void)const {return magic_.typeMagic_;}

protected:
	Player* player_;

	// 魔法の状態
	MAGIC_STATE state_;

	Magic magic_;

	// 魔法をチャージする量
	float chargePow_ = 0.1f;
	// 魔法の最大チャージ量
	float chargeMax_ = 20.0f;
	// 魔法の威力
	float addDamage_ = 5.0f;

	// 武器の座標ポインタ
	VECTOR* weponPos_;

	// 魔法を貯めが最大か
	bool isMaxCharge_;

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


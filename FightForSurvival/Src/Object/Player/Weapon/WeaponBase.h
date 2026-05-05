 #pragma once

#include <vector>
#include "../../../Base.h"
#include "../../Magic/MagicInfo.h"

class Player;
class MagicBase;

class WeaponBase
{
public:

	// プレイヤーとの杖の相対座標
	static constexpr VECTOR RELATIVE_POS_STICK = {10.0f,-70.0f,60.0f };

	// 杖との魔法の発射位置の相対座標
	static constexpr VECTOR RELATIVE_POS_MAGIC = { 0.0f,50.0f,15.0f };

	// プレイヤーとの狙う場所の相対座標
	static constexpr float RELATIVE_POS_TARGET = 1000.0f;

	// リコイルを戻す際の回復速度
	static constexpr float RECOVERY_SPEED = 0.001f;

	// 狙う場所の補正値
	static constexpr float TARGET_POS_OFFSET = 8.0f;

	// 杖の状態
	enum class STATE
	{
		IDLE,			// 待ち状態
		CREATE_MAGIC,	// 魔法生成状態
		CHARGE_MAGIC,	// チャージ状態
		ATTACK,			// 攻撃状態
		WAIT,			// 硬直状態
	};

	// コンストラクタ
	WeaponBase(Player* player);
	// デストラクタ
	virtual ~WeaponBase(void);

	// ロード処理
	virtual void Load(void) = 0;
	// 初期化処理
	void Init(void);
	// 更新処理
	virtual void Update(void);
	// 描画処理
	virtual void Draw(void);
	// 解放処理
	virtual void Release(void);

	// 魔法の取得
	std::vector<MagicBase*> GetMagics(void)const { return magics_; }

	// ステータス渡し
	STATE GetState(void)const { return state_; }

	// リコイル中か
	bool GetIsRecoil(void)const { return isRecoil_; }
	// リコイル中にする
	void SetIsRecoil(bool flg) { isRecoil_ = flg; }

	// 指定された状態へ変更
	void ChangeState(STATE state);

protected:

	// プレイヤーのハンドル
	Player* player_;

	// モデルハンドル
	int modelId_;

	// モデルの大きさ
	VECTOR scales_;
	// モデルの向き
	VECTOR rotate_;
	// モデルの座標
	VECTOR pos_;

	// 現在の状態
	STATE state_;

	// ショット(ポインタ)
	std::vector<MagicBase*> magics_;
	MagicBase* magic_;

	// 魔法が出る場所
	VECTOR magicPos_;

	// 杖を撃つときの狙う場所（杖によって狙う場所の位置が変化する）
	VECTOR targetPos_;

	// 魔法発射後の硬直時間計算用
	float pitchAngle_;
	float pitch_;
	float recoil;
	bool isRecoil_;

	// 使用中の魔法の種類
	TYPE_MAGIC typeMagic_;

	// パラメーター設定
	virtual void SetParam(void) = 0;

	// 待ち中の更新処理
	void IdleUpdate(void);
	// 魔法の生成処理
	void GenerateMagicUpdate(void);
	// 魔法のチャージ処理
	void ChargeMagicUpdate(void);
	// 攻撃処理
	void AttackUpdate(void);
	// 硬直処理
	void WaitUpdate(void);

	// 魔法の更新
	void UpdateMagic(void);
	// 魔法の描画
	void DrawMagic(void);

	// 未使用の魔法かつ魔法が種別が同じだったら使用、未使用の魔法が無かった場合新しく魔法を生成
	virtual MagicBase* GetValidMagic(void);
	
	// アップデート処理
	void UpdatePos(void);

	// 魔法の種類確認
	void CheckMagicType(void);

	// デバッグ用描画
	void DebugDraw(void);
};

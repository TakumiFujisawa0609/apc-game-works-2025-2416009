#pragma once

#include <vector>
#include "../../../Base.h"

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

	// プレイヤーとの狙う場所の相対座標
	static constexpr float RECOVERY_SPEED = 0.001f;

	// リロードする時間
	static constexpr float RELOAD_TIME = 2.0f;

	enum class STATE
	{
		IDLE,
		GENERATE_MAGIC,
		CHARGE_MAGIC,
		ATTACK,
		WAIT,
		RELOAD,
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

	STATE GetState(void)const { return state_; }
	int NowMagicNum(void) { return magicNum_; }

	bool GetIsRecoil(void)const { return isRecoil_; }
	void SetIsRecoil(bool flg) { isRecoil_ = flg; }

	void ChangeState(STATE state);

protected:

	Player* player_;

	int modelId_;

	VECTOR scales_;
	VECTOR rotate_;
	VECTOR pos_;

	STATE state_;

	// ショット(ポインタ)
	std::vector<MagicBase*> magics_;
	MagicBase* magic_;

	// 魔法のモデルID
	int magicModelId_;

	// 魔法が出る場所
	VECTOR magicPos_;

	// 杖を撃つときの狙う場所（杖によって狙う場所の位置が変化する）
	VECTOR targetPos_;

	// 魔法発射後の硬直時間計算用
	float pitchAngle_;
	float pitch_;
	float recoil;
	bool isRecoil_;

	// MPポーション数
	int MPPotionNum_;
	// 魔法攻撃回数
	int magicNum_;
	// 最大魔法攻撃回数
	int magicCapacity_;
	// リロード時間
	float reloadTime_;


	// パラメーター設定
	virtual void SetParam(void) = 0;

	void IdleUpdate(void);
	void GenerateMagicUpdate(void);
	void ChargeMagicUpdate(void);
	void AttackUpdate(void);
	void WaitUpdate(void);
	void ReloadUpdate(void);

	// 魔法の更新
	void UpdateMagic(void);
	// 魔法の描画
	void DrawMagic(void);

	virtual MagicBase* GetValidMagic(void);
	
	void UpdatePos(void);
};

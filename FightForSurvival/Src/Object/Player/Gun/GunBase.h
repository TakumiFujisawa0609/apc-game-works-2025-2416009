#pragma once

#include <vector>
#include "../../../Base.h"

class Player;
class BulletBase;

class GunBase
{
public:

	// プレイヤーとの銃の相対座標
	static constexpr VECTOR RELATIVE_POS_GUN = { 22.0f,-28.0f,65.0f };

	// 銃との弾の発射位置の相対座標
	static constexpr VECTOR RELATIVE_POS_BULLET = { 0.0f,0.0f,15.0f };

	// プレイヤーとの狙う場所の相対座標
	static constexpr VECTOR RELATIVE_POS_TARGET = { 0.0f,0.0f,2000.0f };


	enum class STATE
	{
		IDLE,
		ATTACK,
		WAIT,
		RELOAD,
	};

	// コンストラクタ
	GunBase(Player* player);
	// デストラクタ
	virtual ~GunBase(void);

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

	// 弾の取得
	std::vector<BulletBase*> GetBullets(void)const { return bullets_; }

	bool GetCanShot(void)const { return state_ == STATE::IDLE; }

	void ChangeState(STATE state);

protected:

	Player* player_;

	int modelId_;

	VECTOR gunScales_;
	VECTOR gunRotate_;
	VECTOR gunPos_;

	STATE state_;

	// ショット(ポインタ)
	std::vector<BulletBase*> bullets_;

	// 弾のモデルID
	int bulletModelId_;

	// 弾が出る場所
	VECTOR bulletPos_;

	// 銃を撃つときの狙う場所（銃によって狙う場所の位置が変化する）
	VECTOR targetPos_;

	// 弾発射後の硬直時間計算用
	float stopDelayCnt_;
	float stopDelay_;

	// パラメーター設定
	virtual void SetParam(void) = 0;

	void IdleUpdate(void);
	void AttackUpdate(void);
	void WaitUpdate(void);
	void ReloadUpdate(void);

	// 弾の更新
	void UpdateBullet(void);
	// 弾の描画
	void DrawBullet(void);

	virtual BulletBase* GetValidBullet(void);
	
	void UpdatePos(void);
};

#pragma once

#include <vector>
#include "../../../Base.h"

class Player;
class BulletBase;

class GunBase
{
public:

	// 弾発射後の硬直時間
	static constexpr VECTOR RELATIVE_POS = { 1.0f,-1.0f,1.8f };

	// 弾発射後の硬直時間
	static constexpr float STOP_DELAY = 1.0f;

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

	VECTOR bulletPos_;

	// 弾発射後の硬直時間計算用
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

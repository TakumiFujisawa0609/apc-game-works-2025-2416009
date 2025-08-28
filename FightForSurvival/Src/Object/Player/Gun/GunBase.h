#pragma once

class Player;
class BulletBase;

class GunBase
{
public:

	enum class GUN_TYPE
	{
		HANDGUN,
		ASSAULT_RIFLE,
		SHOTGUN,
	};

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

	// 初期化処理
	void Init(GUN_TYPE type);
	// 更新処理
	virtual void Update(void);
	// 描画処理
	virtual void Draw(void);
	// 解放処理
	virtual void Release(void);

	const GUN_TYPE GetWeappnType(void)const { return weapontype_; }

protected:

	Player* player_;

	int modelId_;

	VECTOR dir_;
	VECTOR pos_;

	GUN_TYPE weapontype_;

	STATE state_;

	// ロード処理
	virtual void Load(void) = 0;
	// パラメーター設定
	virtual void SetParam(void) = 0;

	//BulletBase* GetValidShot(void);
};

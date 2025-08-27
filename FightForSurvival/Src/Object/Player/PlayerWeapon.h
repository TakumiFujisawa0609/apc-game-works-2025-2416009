#pragma once

class PlayerWeapon
{
public:

	enum class WEAPON_TYPE
	{
		HANDGUN,
		ASSAULT_RIFLE,
		SHOTGUN,
	};

	// コンストラクタ
	PlayerWeapon(void);
	// デストラクタ
	virtual ~PlayerWeapon(void);

	// 初期化処理
	void Init(WEAPON_TYPE type);
	// 更新処理
	virtual void Update(void);
	// 描画処理
	virtual void Draw(void) = 0;
	// 解放処理
	virtual void Release(void) = 0;

	// 武器を使用する(攻撃が行われた時呼ばれる)
	virtual void Use(VECTOR pos, VECTOR dir) = 0;

	const WEAPON_TYPE GetWeappnType(void)const { return weapontype_; }

protected:

	WEAPON_TYPE weapontype_;

	// ロード処理
	virtual void Load(void) = 0;

	// パラメーター設定
	virtual void SetParam(void) = 0;
};

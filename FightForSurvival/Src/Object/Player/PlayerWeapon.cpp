#include <DxLib.h>
#include "PlayerWeapon.h"

PlayerWeapon::PlayerWeapon(void)
{
}

PlayerWeapon::~PlayerWeapon(void)
{
}

void PlayerWeapon::Init(WEAPON_TYPE type)
{
	weapontype_ = type;

	// 画像やモデルなどのロード
	Load();

	// パラメータ設定
	SetParam();

}

void PlayerWeapon::Update(void)
{
}

void PlayerWeapon::Draw(void)
{
}

void PlayerWeapon::Release(void)
{
}


#include <DxLib.h>
#include "../../Magic/Base/MagicBase.h"
#include "../../../Application.h"
#include "Stick.h"

Stick::Stick(Player* player):WeaponBase(player)
{
}

Stick::~Stick(void)
{
}

void Stick::Load(void)
{
	// –‚–@‚Ìƒ‚ƒfƒ‹‚ğ“Ç‚İ‚Ş
	modelId_ = MV1LoadModel(
		(Application::PATH_PLAYER + "Weapon/WoodenStaff.mv1").c_str());
}

void Stick::SetParam(void)
{
	// ‘å‚«‚³İ’è
	scales_ = SCALE;
	// Œü‚«İ’è
	rotate_ = ROTATE;
	// ”½“®’l‚ğİ’è
	recoil = RECOIL;
}


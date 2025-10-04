#include <DxLib.h>
#include "Magic/MagicBase.h"
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

	if (modelId_ == -1)
	{
		// ƒ‚ƒfƒ‹‚Ì“Ç‚İ‚İ‚É¸”s‚µ‚½ê‡‚Ìˆ—
		DrawString(0, 0, "CannonBarrel model load failed", GetColor(255, 0, 0));
	}
}

void Stick::SetParam(void)
{
	scales_ = SCALE;
	rotate_ = ROTATE;
	recoil = RECOIL;
	magicNum_ = magicCapacity_ = MAGIC_CAPACITY;
	MPPotionNum_ = MP_POTION_NUM;
}


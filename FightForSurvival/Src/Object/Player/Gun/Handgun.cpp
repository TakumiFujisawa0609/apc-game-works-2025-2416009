#include <DxLib.h>
#include "Bullet/BulletBase.h"
#include "../../../Application.h"
#include "Handgun.h"

Handgun::Handgun(Player* player):GunBase(player)
{
}

Handgun::~Handgun(void)
{
}

void Handgun::Load(void)
{
	// ’e‚Ìƒ‚ƒfƒ‹‚ğ“Ç‚İ‚Ş
	modelId_ = MV1LoadModel(
		(Application::PATH_MODEL + "Cannon/Barrel.mv1").c_str());

	if (modelId_ == -1)
	{
		// ƒ‚ƒfƒ‹‚Ì“Ç‚İ‚İ‚É¸”s‚µ‚½ê‡‚Ìˆ—
		DrawString(0, 0, "CannonBarrel model load failed", GetColor(255, 0, 0));
	}
}

void Handgun::SetParam(void)
{
	gunScales_ = SCALE;
	gunRotate_ = ROTATE;
	recoil = RECOIL;
	bulletNum_ = bulletCapacity_ = BULLET_CAPACITY;
	bulletNumMax_ = BULLET_NUM_MAX;
}


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
	// ñÇñ@ÇÃÉÇÉfÉãÇì«Ç›çûÇﬁ
	modelId_ = MV1LoadModel(
		(Application::PATH_PLAYER + "Weapon/WoodenStaff.mv1").c_str());
}

void Stick::SetParam(void)
{
	scales_ = SCALE;
	rotate_ = ROTATE;
	recoil = RECOIL;
}


#include "MagicStatus.h"

#include <DxLib.h>

#include "../../TextrueManager/TextureManager.h"
#include "../../../Object/Player/Player.h"

MagicStatus::MagicStatus(const UIResourceInfo& info, TextureManager* texMgr)
{
	x_ = info.x;
	y_ = info.y;
	w_ = info.width;
	h_ = info.height;
	scale_ = info.scale;

	std::string STATUS = "Data/Image/UI/MagicStatus/Mp_";
	std::string KIND = "Data/Image/UI/MagicStatus/Kind_";

	// ÉçÅ[Éh
	for (int i = 0; i < STATUS_NUM; i++)
	{
		std::string num = std::to_string(i);
		statusHandle_[i] = texMgr->LoadTexture(STATUS + num + ".png");
	}

	for (int i = 0; i < BULLET_KIND_NUM; i++)
	{
		std::string num = std::to_string(i);
		bulletKindHandle_[i] = texMgr->LoadTexture(KIND + num + ".png");
	}
}

MagicStatus::~MagicStatus()
{
}

void MagicStatus::SetPlayer(Player* player)
{
	player_ = player;
}

void MagicStatus::Update(void)
{
}

void MagicStatus::Draw(void)
{
	DrawRotaGraph(x_, y_, scale_, 0.0, statusHandle_[player_->GetMagicNum()], true);
	DrawRotaGraph(x_, y_, scale_, 0.0, bulletKindHandle_[player_->GetMagicType()], true);
}

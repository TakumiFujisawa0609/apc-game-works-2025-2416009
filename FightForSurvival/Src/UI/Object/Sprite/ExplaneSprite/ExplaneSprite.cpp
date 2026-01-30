#include "ExplaneSprite.h"

#include <DxLib.h>

#include "../../../TextrueManager/TextureManager.h"
#include "../../../../Manager/InputManager.h"

ExplaneSprite::ExplaneSprite(const UIResourceInfo& info, TextureManager* texMgr)
	:Sprite(info, texMgr),
	explaneKind_(EXPLANE::EXPLANE_1)
{
	uiKind_ = info.uiKind;
	isDraw_ = info.isDraw;
	x_ = info.x;
	y_ = info.y;
	w_ = info.width;
	h_ = info.height;

	// ƒ[ƒh
	handle_[EXPLANE::EXPLANE_1] = texMgr->LoadTexture(info.basePath);
	handle_[EXPLANE::EXPLANE_2] = texMgr->LoadTexture(info.mainPath);
	handle_[EXPLANE::EXPLANE_3] = texMgr->LoadTexture(info.framePath);
	handle_[EXPLANE::EXPLANE_4] = texMgr->LoadTexture(info.textPath);
}

ExplaneSprite::~ExplaneSprite()
{
}

void ExplaneSprite::Draw(void)
{
	if (!isDraw_)
	{
		return;
	}

	DrawGraph(x_, y_, handle_[explaneKind_], true);
}

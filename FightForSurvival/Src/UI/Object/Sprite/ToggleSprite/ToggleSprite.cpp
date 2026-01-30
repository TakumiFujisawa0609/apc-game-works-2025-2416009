#include "ToggleSprite.h"

#include "../../../../Manager/InputManager.h"
#include "../../../TextrueManager/TextureManager.h"
#include "../../../../Manager/InputManager.h"

ToggleSprite::ToggleSprite(const UIResourceInfo& info, TextureManager* texMgr)
	:Sprite(info, texMgr),
	isPicture1Flg_(true)
{
	uiKind_ = info.uiKind;
	isDraw_ = info.isDraw;
	x_ = info.x;
	y_ = info.y;
	w_ = info.width;
	h_ = info.height;

	picture1_ = texMgr->LoadTexture(info.basePath);
	picture2_ = texMgr->LoadTexture(info.mainPath);
}

ToggleSprite::~ToggleSprite()
{
}

void ToggleSprite::Update(void)
{
	// 特定のキーを押されたら表示を切り替える(今回は操作説明：表示/非表示）
	if (InputManager::GetInstance().ManualKeys())
	{
		isPicture1Flg_ = !isPicture1Flg_;
	}
}

void ToggleSprite::Draw(void)
{
	if (isPicture1Flg_)
	{
		DrawGraph(x_, y_, picture1_, true);
	}
	else
	{
		DrawGraph(x_, y_, picture2_, true);
	}
}

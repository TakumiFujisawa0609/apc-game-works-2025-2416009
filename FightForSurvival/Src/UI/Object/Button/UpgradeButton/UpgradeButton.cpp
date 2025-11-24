#include "UpgradeButton.h"

#include <DxLib.h>

#include "../../../TextrueManager/TextureManager.h"

UpgradeButton::UpgradeButton(const UIResourceInfo& info, TextureManager* texMgr)
	: Button(info,texMgr)
{
	texMgr_ = texMgr;
}

UpgradeButton::~UpgradeButton()
{
}

void UpgradeButton::Update(void)
{
	Button::Update();
}

void UpgradeButton::Draw(void)
{
	Button::Draw();
}

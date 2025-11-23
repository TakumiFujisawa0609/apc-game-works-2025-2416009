#include "StaminaBar.h"

#include <DxLib.h>
#include <algorithm>

#include "../../TextrueManager/TextureManager.h"
#include "../../../Object/Player/Player.h"

StaminaBar::StaminaBar(const UIResourceInfo& info, TextureManager* texMgr)
	:
	barRate_(1.0f),
	baseHandle_(-1),
	barHandle_(-1),
	frameHandle_(-1)
{
	x_ = info.x;
	y_ = info.y;
	w_ = info.width;
	h_ = info.height;
	scale_ = info.scale;

	// ロード
	baseHandle_ = texMgr->LoadTexture(info.basePath);
	barHandle_ = texMgr->LoadTexture(info.mainPath);
	frameHandle_ = texMgr->LoadTexture(info.framePath);
}

StaminaBar::~StaminaBar()
{
}

void StaminaBar::SetPlayer(Player* player)
{
	// プレイヤーセット(ポインタ)
	player_ = player;
}

void StaminaBar::Update(void)
{
	float stamina = player_->GetPlayerAblity().stamina_;
	float maxStamina = player_->GetPlayerAblity().staminaMax_;
	barRate_ = stamina / maxStamina;

}

void StaminaBar::Draw(void)
{
	int x = x_ + (w_ / 4);
	int y = y_ + (h_ / 4);

	// 下地
	DrawRotaGraph(x, y, scale_, 0.0, baseHandle_, true);

	// HP 割合の安全クランプ (オーバーフロー防止)
	float rate = std::clamp(barRate_, 0.0f, 1.0f);

	int right = x_ + static_cast<int>(w_ * rate * scale_);

	// bar の描画
	if (rate > 0.0f) {
		DrawExtendGraphF(
			x_,
			y_,
			right,
			y_ + static_cast<int>(h_ * scale_),
			barHandle_,
			true
		);
	}

	// フレーム
	DrawRotaGraph(x, y, scale_, 0.0, frameHandle_, true);
}

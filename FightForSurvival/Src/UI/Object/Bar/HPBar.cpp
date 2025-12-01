#include "HPBar.h"

#include <DxLib.h>
#include <algorithm>

#include "../../TextrueManager/TextureManager.h"
#include "../../../Object/Player/Player.h"

HPBar::HPBar(const UIResourceInfo& info, TextureManager* texMgr)
	:
	barRate_(1.0f),
	baseHandle_(-1),
	barHandle_(-1),
	frameHandle_(-1)
{
	uiKind_ = info.uiKind;
	isDraw_ = info.isDraw;
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

HPBar::~HPBar()
{
}

void HPBar::SetPlayer(Player* player)
{
	// プレイヤーセット(ポインタ)
	player_ = player;
}

void HPBar::Update(void)
{
	// 更新
	float hp = player_->GetPlayerStatus().hp_;
	float maxHp = player_->GetPlayerAblity().hpMax_;
	barRate_ = hp / maxHp;

}

void HPBar::Draw(void)
{
	if (!isDraw_)
	{
		return;
	}

	// 描画

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

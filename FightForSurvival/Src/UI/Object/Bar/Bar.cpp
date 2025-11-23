#include "Bar.h"

#include <DxLib.h>

#include "../../TextrueManager/TextureManager.h"

Bar::Bar(const UIResourceInfo& info, TextureManager* texMgr)
	:
	barRate_(1.0f),
	baseHandle_(-1),
	barHandle_(-1),
	frameHandle_(-1),
	currentNum_(0),
	maxNum_(0)
{
	x_ = info.x;
	y_ = info.y;
	w_ = info.width;
	h_ = info.height;

	// ロード
	baseHandle_ = texMgr->LoadTexture(info.basePath);
	barHandle_ = texMgr->LoadTexture(info.mainPath);
	frameHandle_ = texMgr->LoadTexture(info.framePath);
}

Bar::~Bar()
{
}

void Bar::SetValue(int* currentNum, int* maxNum)
{
	// 数値をセット(ポインタ)
	currentNum_ = currentNum;
	maxNum_ = maxNum;
}

void Bar::Update(void)
{
	float hp = static_cast<float>(*currentNum_);
	float maxHp = static_cast<float>(*maxNum_);

	barRate_ = hp / maxHp;
}

void Bar::Draw(void)
{
	// 下地の描画
	DrawGraph(x_, y_, baseHandle_, true);

	// barの描画
	int right = x_ + static_cast<int>(w_ * barRate_);

	// bar が 0 以上 なら
	if (barRate_ > 0)
	{
		// bar の描画
		DrawExtendGraph(x_, y_, right, y_ + h_, barHandle_, true);
	}

	// フレームの描画
	DrawGraph(x_, y_, frameHandle_, true);
}

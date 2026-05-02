#include <DxLib.h>
#include "../../Manager/SystemManager.h"
#include "Score.h"

Score::Score(void)
	:pos_(0,0)
	,font_(-1)
{
}

Score::~Score(void)
{
}

void Score::Load(void)
{
	// フォントハンドル取得
	font_= CreateFontToHandle("x12y12pxMaruMinya", 32, 16, DX_FONTTYPE_ANTIALIASING);
}

void Score::Init(void)
{
	// 座標の初期化
	pos_.x = POS_X;
	pos_.y = POS_Y;
}

void Score::Draw(void)
{
	// フォントハンドルを使用した文字出力
	DrawFormatStringToHandle(pos_.x, pos_.y, 0xffffff, font_, "Score : %d", SystemManager::GetInstance().GetScore());
}

void Score::Release(void)
{
	// フォントハンドルの解放
	DeleteFontToHandle(font_);
}

#include "WaveFinal.h"

#include "../Application.h"

WaveFinal::WaveFinal(void)
	: WaveBase(60 * 30, 60 * 15)
{
}
void WaveFinal::OnStart()
{

}

void WaveFinal::OnWave()
{
}

void WaveFinal::OnClear()
{
}

void WaveFinal::Draw()
{
	int posX = Application::SCREEN_SIZE_X / 2;

	// 親クラスの共通描画物
	WaveBase::Draw();

	if (state_ != WaveBase::WaveState::PREPARE)
	{
		DrawStringToHandle(posX - 47, 5, "WaveFinal", 0xffffff, font_[static_cast<int>(Font::SMALL)]);
	}
	DrawStringToHandle(8, 40, "敵を全て倒すか、時間いっぱいまで逃げろ！", 0xff0000, font_[static_cast<int>(Font::SMALL)]);
}
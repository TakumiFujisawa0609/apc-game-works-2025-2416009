#include "WaveFinal.h"

#include "../Application.h"

WaveFinal::WaveFinal(void)
	: WaveBase(60 * 30, 60 * 30)
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
		DrawString(posX - 20, 5, "WaveFinal", 0xffffff);
	}
	DrawString(5, 40, "敵を全て倒すか、時間いっぱいまで逃げろ！", 0xff0000);
}
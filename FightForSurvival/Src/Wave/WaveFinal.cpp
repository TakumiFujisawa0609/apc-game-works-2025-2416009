#include "WaveFinal.h"


WaveFinal::WaveFinal(void)
	: WaveBase(PREPARE_TIME, WAVE_TIME)
{
}

WaveFinal::~WaveFinal(void)
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
	// 親クラスの共通描画物
	WaveBase::Draw();

	// 準備状態じゃなければ描画
	if (state_ != WaveBase::WaveState::PREPARE)
	{
		DrawStringToHandle(WAVW_NAME_POS_X
			, WAVW_NAME_POS_Y
			, "WaveFinal"
			, WHITE_COLOR
			, font_[static_cast<int>(Font::SMALL)]);
	}

	// 指示の文字の描画
	DrawStringToHandle(INSTRUCTION_POS_X
		, INSTRUCTION_POS_Y
		, "敵を全て倒すか、時間いっぱいまで逃げろ！"
		, RED_COLOR
		, font_[static_cast<int>(Font::SMALL)]);
}
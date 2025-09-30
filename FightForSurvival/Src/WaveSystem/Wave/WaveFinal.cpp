#include "WaveFinal.h"
#include <DxLib.h>

WaveFinal::WaveFinal(void)
	: WaveBase(60 * 0, 60 * 20)
{
}

void WaveFinal::Draw(void)
{
	DrawString(5, 20, "Final Wave", 0xffffff);
	DrawString(5, 40, "“G‚ğ‚·‚×‚Ä“|‚·‚©AŠÔI—¹‚Ü‚Å“¦‚°‚ëI", 0xffffff);
}

#include "Wave4.h"

#include "../Application.h"

Wave4::Wave4(void)
	: WaveBase(PREPARE_TIME, WAVE_TIME)
{
	// スポーンタイミング、敵種別、座標
	AddSpawnEvent(60 * 3, ENEMY_TYPE::ZOMBIE, VGet(-3000.0f, 5.0f, 200.0f));
	AddSpawnEvent(60 * 5, ENEMY_TYPE::ZOMBIE, VGet(-300.0f, 5.0f, -500.0f));
	AddSpawnEvent(60 * 10, ENEMY_TYPE::BAT, VGet(600.0f, 5.0f, -500.0f));
	AddSpawnEvent(60 * 15, ENEMY_TYPE::BAT, VGet(200.0f, 5.0f, 0.0f));
	AddSpawnEvent(60 * 15, ENEMY_TYPE::ZOMBIE, VGet(1500.0f, 5.0f, -600.0f));

}

Wave4::~Wave4(void)
{
}

void Wave4::OnStart(void)
{
}

void Wave4::OnWave(void)
{
}

void Wave4::OnClear(void)
{
}

void Wave4::Draw(void)
{
	// 親クラスの共通描画物
	WaveBase::Draw();

	// 準備状態じゃなければ描画
	if (state_ != WaveBase::WaveState::PREPARE)
	{
		DrawStringToHandle(WAVE_NAME_POS_X
			, WAVE_NAME_POS_Y
			, "Wave4"
			, WHITE_COLOR
			, font_[static_cast<int>(Font::SMALL)]);
	}

	// 指示の文字の描画
	DrawStringToHandle(INSTRUCTION_POS_X,
		INSTRUCTION_POS_Y
		, "敵を倒せ！"
		, RED_COLOR
		, font_[static_cast<int>(Font::SMALL)]);
}

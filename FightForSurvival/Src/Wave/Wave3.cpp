#include "Wave3.h"

Wave3::Wave3(void)
	: WaveBase(PREPARE_TIME, WAVE_TIME)
{
	// スポーンタイミング、スポナーの敵スポーン間隔、座標
	AddSpawner(60 * 5, 10, VGet(-300.0f, 5.0f, -500.0f), Spawner::PATTERN::PATTERN_2);

	// スポーンタイミング、スポナーの敵スポーン間隔、座標
	AddSpawner(60 * 20, 20, VGet(600.0f, 5.0f, -500.0f), Spawner::PATTERN::PATTERN_3);
	AddSpawner(60 * 20, 20, VGet(200.0f, 5.0f, 0.0f), Spawner::PATTERN::PATTERN_2);
	AddSpawner(60 * 30, 20, VGet(1500.0f, 5.0f, -600.0f), Spawner::PATTERN::PATTERN_3);

	// ドラゴン
	AddSpawnEvent(60 * 0, ENEMY_TYPE::DRAGON, VGet(0.0f, -5.0f, 0.0f));
}

Wave3::~Wave3(void)
{
}

void Wave3::OnStart(void)
{
}

void Wave3::OnWave(void)
{
}

void Wave3::OnClear(void)
{
}

void Wave3::Draw(void)
{
	// 親クラスの共通描画物
	WaveBase::Draw();

	// 準備状態じゃなければ描画
	if (state_ != WaveBase::WaveState::PREPARE)
	{
		DrawStringToHandle(WAVE_NAME_POS_X
			, WAVE_NAME_POS_Y
			, "Wave3"
			, WHITE_COLOR
			, font_[static_cast<int>(Font::SMALL)]);
	}

	// 指示の文字の描画
	DrawStringToHandle(INSTRUCTION_POS_X
		, INSTRUCTION_POS_Y
		, "敵を倒せ！"
		, RED_COLOR
		, font_[static_cast<int>(Font::SMALL)]);
}

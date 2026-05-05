#include "Wave2.h"

Wave2::Wave2(void)
	: WaveBase(PREPARE_TIME, WAVE_TIME)
{
	// スポーンタイミング、敵種別、座標
	AddSpawnEvent(60 * 5, ENEMY_TYPE::ZOMBIE, VGet(400.0f, 5.0f, 1500.0f));
	AddSpawnEvent(60 * 6, ENEMY_TYPE::ZOMBIE, VGet(380.0f, 5.0f, 1500.0f));
	AddSpawnEvent(60 * 7, ENEMY_TYPE::ZOMBIE, VGet(360.0f, 5.0f, 1500.0f));
	AddSpawnEvent(60 * 8, ENEMY_TYPE::ZOMBIE, VGet(340.0f, 5.0f, 1500.0f));
	AddSpawnEvent(60 * 9, ENEMY_TYPE::ZOMBIE, VGet(360.0f, 5.0f, 1500.0f));

	AddSpawnEvent(60 * 10, ENEMY_TYPE::BAT, VGet(400.0f, 5.0f, 1500.0f));
	AddSpawnEvent(60 * 10, ENEMY_TYPE::BAT, VGet(450.0f, 5.0f, 1500.0f));
	AddSpawnEvent(60 * 10, ENEMY_TYPE::BAT, VGet(500.0f, 5.0f, 1500.0f));

	// スポーンタイミング、スポナーの敵スポーン間隔、座標
	AddSpawner(60 * 20, 10, VGet(-400.0f, 5.0f, 1500.0f), Spawner::PATTERN::PATTERN_3);

	// スポーンタイミング、スポナーの敵スポーン間隔、座標
	AddSpawner(60 * 20, 10, VGet(-400.0f, 5.0f, -1500.0f), Spawner::PATTERN::PATTERN_3);
}

Wave2::~Wave2(void)
{
}

void Wave2::OnStart(void)
{
}

void Wave2::OnWave(void)
{
}

void Wave2::OnClear(void)
{
}

void Wave2::Draw(void)
{
	// 親クラスの共通描画物
	WaveBase::Draw();

	// 準備状態じゃなければ描画
	if (state_ != WaveBase::WaveState::PREPARE)
	{
		DrawStringToHandle(WAVE_NAME_POS_X
			, WAVE_NAME_POS_Y
			, "Wave2"
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

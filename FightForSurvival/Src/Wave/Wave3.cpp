#include "Wave3.h"

#include "../Application.h"

Wave3::Wave3(void)
	: WaveBase(60 * 30, 60 * 60)
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
	int posX = Application::SCREEN_SIZE_X / 2;

	// 親クラスの共通描画物
	WaveBase::Draw();

	if (state_ != WaveBase::WaveState::PREPARE)
	{
		DrawStringToHandle(posX - 25, 5, "Wave3", 0xffffff, font_[static_cast<int>(Font::SMALL)]);
	}
	DrawStringToHandle(8, 40, "敵を倒せ！", 0xff0000, font_[static_cast<int>(Font::SMALL)]);
}

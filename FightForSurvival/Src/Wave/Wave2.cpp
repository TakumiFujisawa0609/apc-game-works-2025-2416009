#include "Wave2.h"

#include "../Application.h"

Wave2::Wave2(void)
	: WaveBase(60 * 30, 60 * 45)
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
	int posX = Application::SCREEN_SIZE_X / 2;

	// 親クラスの共通描画物
	WaveBase::Draw();

	if (state_ != WaveBase::WaveState::PREPARE)
	{
		DrawStringToHandle(posX - 25, 5, "Wave2", 0xffffff, font_[static_cast<int>(Font::SMALL)]);
	}
	DrawStringToHandle(8, 40, "敵を倒せ！", 0xff0000, font_[static_cast<int>(Font::SMALL)]);
}

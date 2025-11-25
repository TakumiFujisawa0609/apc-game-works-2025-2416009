#include "Wave2.h"

#include "../Application.h"

Wave2::Wave2(void)
	: WaveBase(60 * 30, 60 * 20)
{
	// ※数値や敵の種別を外部ファイルから取得するようにすると評価〇！
	// スポーンタイミング、敵種別、座標
	AddSpawnEvent(60 * 5, ENEMY_TYPE::ZOMBIE, VGet(-3000.0f, 5.0f, 200.0f));
	AddSpawnEvent(60 * 5, ENEMY_TYPE::ZOMBIE, VGet(-300.0f, 5.0f, -500.0f));
	AddSpawnEvent(60 * 10, ENEMY_TYPE::BAT, VGet(600.0f, 5.0f, -500.0f));
	AddSpawnEvent(60 * 15, ENEMY_TYPE::BAT, VGet(200.0f, 5.0f, 0.0f));
	AddSpawnEvent(60 * 15, ENEMY_TYPE::ZOMBIE, VGet(1500.0f, 5.0f, -600.0f));

	// ドラゴン
	AddSpawnEvent(60 * 3, ENEMY_TYPE::DRAGON, VGet(0.0f, -5.0f, 0.0f));

	// スポーンタイミング、スポナーの敵スポーン間隔、座標
	AddSpawner(60 * 18, 10, VGet(2000.0f, -5.0f, 500.0f));
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
		DrawString(posX - 20, 5, "Wave2", 0xffffff);
	}
	DrawString(5, 40, "敵を倒せ！", 0xff0000);
}

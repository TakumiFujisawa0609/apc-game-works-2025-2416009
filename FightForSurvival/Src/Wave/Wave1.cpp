#include "Wave1.h"

#include "../Application.h"

// 準備30秒 → 戦闘120秒
Wave1::Wave1(void)
	: WaveBase(60 * 3, 60 * 20)
{
	// ※数値や敵の種別を外部ファイルから取得するようにすると評価〇！
	// スポーンタイミング、敵種別、座標
	AddSpawnEvent(60 * 3, ENEMY_TYPE::ZOMBIE, VGet(0.0f, 0.0f, 0.0f));
	AddSpawnEvent(60 * 7, ENEMY_TYPE::ZOMBIE, VGet(0.0f, 0.0f, 0.0f));
	AddSpawnEvent(60 * 10, ENEMY_TYPE::BAT, VGet(0.0f, 0.0f, 0.0f));
	AddSpawnEvent(60 * 15, ENEMY_TYPE::ZOMBIE, VGet(0.0f, 0.0f, 0.0f));
	AddSpawnEvent(60 * 18, ENEMY_TYPE::BAT, VGet(0.0f, 0.0f, 0.0f));

	// スポーンタイミング、スポナーの敵スポーン間隔、座標
	AddSpawner(60 * 2, 15, VGet(200.0f, 0.0f, 200.0f));
}

void Wave1::OnStart()
{
}

void Wave1::OnWave()
{
}

void Wave1::OnClear()
{
}

void Wave1::Draw()
{
	int posX = Application::SCREEN_SIZE_X / 2;

	// 親クラスの共通描画物
	WaveBase::Draw();

	if (state_ != WaveBase::WaveState::PREPARE)
	{
		DrawString(posX - 20, 5, "Wave1", 0xffffff);
	}
	DrawString(5, 40, "敵を倒せ！", 0xff0000);
}
#include "Wave1.h"

// 準備30秒 → 戦闘120秒
Wave1::Wave1(void)
	: WaveBase(60 * 3, 60 * 120)
{
	// ※数値や敵の種別を外部ファイルから取得するようにすると評価〇！
	// スポーンタイミング、敵種別、座標
	AddSpawnEvent(60 * 5, ENEMY_TYPE::ZOMBIE, VGet(0.0f, 0.0f, 0.0f));
	AddSpawnEvent(60 * 20, ENEMY_TYPE::ZOMBIE, VGet(0.0f, 0.0f, 0.0f));
	AddSpawnEvent(60 * 30, ENEMY_TYPE::ZOMBIE, VGet(0.0f, 0.0f, 0.0f));
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
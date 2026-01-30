#include "Wave4.h"

#include "../Application.h"

Wave4::Wave4(void)
	: WaveBase(60 * 30, 60 * 20)
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
	int posX = Application::SCREEN_SIZE_X / 2;

	// 親クラスの共通描画物
	WaveBase::Draw();

	if (state_ != WaveBase::WaveState::PREPARE)
	{
		DrawStringToHandle(posX - 25, 5, "Wave4", 0xffffff, font_[static_cast<int>(Font::SMALL)]);
	}
	DrawStringToHandle(8, 40, "敵を倒せ！", 0xff0000, font_[static_cast<int>(Font::SMALL)]);
}

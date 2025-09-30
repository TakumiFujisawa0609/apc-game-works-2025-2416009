#include "Wave1.h"
#include "../Application.h"
#include "WaveManager.h"

WaveManager* WaveManager::instance_ = nullptr;

WaveManager::WaveManager(void)
	: currentWaveIndex(0)
{
}

WaveManager::~WaveManager(void)
{
}

void WaveManager::AddWave(std::unique_ptr<WaveBase> wave)
{
	// ウェーブの追加
	waves.push_back(std::move(wave));
}

void WaveManager::Update()
{
	// 全てのウェーブが終わっていたら処理しない
	if (currentWaveIndex >= (int)waves.size())
		return;

	// 現在のウェーブを取得して更新
	auto& wave = waves[currentWaveIndex];
	wave->Update();

	// 現在のウェーブがクリアしていたら
	if (wave->IsCleared())
	{
		// ウェーブを加算
		currentWaveIndex++;
	}
}

void WaveManager::Draw(void)
{
	// 全てのウェーブが終わっていたら処理しない
	if (currentWaveIndex >= (int)waves.size())
		return;

	// 現在のウェーブを取得して更新
	auto& wave = waves[currentWaveIndex];
	wave->Draw();
}

WaveBase* WaveManager::GetCurrentWave()
{
	// 全てのウェーブが終わっていないなら(WaveFinalだったらnullptrを返すために、+1)
	if (currentWaveIndex + 1 < (int)waves.size())
	{
		// 現在のウェーブを取得
		return waves[currentWaveIndex].get();
	}

	return nullptr;
}

bool WaveManager::AllCleared(void) const
{
	// 全てのウェーブが終わっていたらtrue
	return currentWaveIndex >= (int)waves.size();
}


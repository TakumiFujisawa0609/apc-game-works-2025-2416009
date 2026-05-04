#include "SpawnerManager.h"
#include "Spawner.h"

SpawnerManager* SpawnerManager::instance_ = nullptr;

void SpawnerManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new SpawnerManager();
	}
}

SpawnerManager& SpawnerManager::GetInstance(void)
{
	return *instance_;
}

void SpawnerManager::Destroy()
{
	// 解放
	for (auto& spawner : spawners_)
	{
		spawner->Release();
	}

	// 可変長配列の解放
	spawners_.clear();

	delete instance_;
	instance_ = nullptr;
}

void SpawnerManager::Load(void)
{
}

void SpawnerManager::Update(void)
{
	for (auto& spawner : spawners_)
	{
		// スポナーが破壊され存在していなかったら処理を行わず次の処理へ移行
		if (!spawner->GetSpawnerIns().isExists_)
		{
			continue;
		}

		// スポナーの更新を行う
		spawner->Update();
	}
}

void SpawnerManager::Draw(void)
{
	for (auto& spawner : spawners_)
	{
		// スポナーが破壊され存在していなかったら処理を行わず次の処理へ移行
		if (!spawner->GetSpawnerIns().isExists_)
		{
			continue;
		}

		// スポナーの描画を行う
		spawner->Draw();
	}
}

void SpawnerManager::SpawnerInstallation(VECTOR pos,float interval, Spawner::PATTERN pattern)
{
	// 未使用のスポナーを再利用する。未使用のものが無ければ生成を行う
	auto* spawner = GetValidSpawner();

	// 初期化を行う
	spawner->Create(pos, interval, pattern);
}

SpawnerManager::SpawnerManager(void)
{
}

void SpawnerManager::AddSpawner(Spawner* spawner)
{
	// スポナーを追加する
	spawners_.emplace_back(std::move(spawner));
}

Spawner* SpawnerManager::GetValidSpawner(void)
{
	// すでに生成されているインスタンス
	for (auto& spawner : spawners_)
	{
		// 存在していなかったらこのインスタンスを再利用する
		if (!spawner->GetSpawnerIns().isExists_)
		{
			return spawner;
		}
	}

	// 未使用のスポナーが無かった場合の新しいスポナーを生成
	Spawner* spawner = nullptr;
	spawner = new Spawner();

	// 可変長配列に追加
	AddSpawner(spawner);

	return spawner;
}

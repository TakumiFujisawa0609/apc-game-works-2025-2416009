#include "UpgradeManager.h"

UpgradeManager* UpgradeManager::instance_ = nullptr;

void UpgradeManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new UpgradeManager();
	}
}

UpgradeManager& UpgradeManager::GetInstance(void)
{
	if (instance_ == nullptr)
	{
		UpgradeManager::CreateInstance();
	}
	return *instance_;
}

void UpgradeManager::Init(Player* player)
{
	player_ = player;
}

void UpgradeManager::Destroy(void)
{
	if (instance_ != nullptr)
	{
		// インスタンスのメモリ解放
		delete instance_;
		instance_ = nullptr;
	}
}

UpgradeManager::UpgradeManager(void)
{
}

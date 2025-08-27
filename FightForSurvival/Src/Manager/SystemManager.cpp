#include "SystemManager.h"

SystemManager* SystemManager::instance_ = nullptr;

void SystemManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new SystemManager();
	}
}

SystemManager& SystemManager::GetInstance(void)
{
    return *instance_;
}

void SystemManager::Destroy()
{
	delete instance_;
	instance_ = nullptr;
}

void SystemManager::ResetGame()
{
	score_ = 0; // 各プレイヤーのスコアを0で初期化
}

SystemManager::SystemManager(void)
{
	sensitivity = 0.005f;
	score_ = 0;
}

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
	score_ = 0; // ƒXƒRƒA‚ğ0‚Å‰Šú‰»
}

SystemManager::SystemManager(void)
{
	sensitivity = SENSITIVITY;
	score_ = 0;
	gunType_ = weapon_TYPE::Stick;
}

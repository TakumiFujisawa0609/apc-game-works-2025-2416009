#include "EndManager.h"

EndManager* EndManager::instance_ = nullptr;

EndManager::EndManager(void)
{
}

void EndManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new EndManager();
	}
	instance_->Init();
}


EndManager& EndManager::GetInstance(void)
{
	if (instance_ == nullptr)
	{
		EndManager::CreateInstance();
	}
	return *instance_;
}

void EndManager::Init(void)
{
	isEnd_ = false;
}

void EndManager::Destroy(void)
{
	if (instance_ != nullptr)
	{
		// インスタンスのメモリ解放
		delete instance_;
		instance_ = nullptr;
	}
}

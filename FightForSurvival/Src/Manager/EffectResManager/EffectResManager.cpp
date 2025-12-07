#include <EffekseerForDXLib.h>
#include "../../Application.h"
#include "EffectResManager.h"

// 中身をnullptrで初期化
EffectResManager* EffectResManager::instance_ = nullptr;

void EffectResManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		// 中身がnullptrで何も入っていなかったら行う
		instance_ = new EffectResManager();
	}
	instance_->Init();
}

EffectResManager& EffectResManager::GetInstance(void)
{
	return *instance_;
}

void EffectResManager::Init(void)
{
	// リソースのロード
	Load();
}

void EffectResManager::Destroy(void)
{
	for (const auto& pair : resourceIds_)
	{
		// 爆発エフェクトのメモリ解放
		DeleteEffekseerEffect(pair.second);
	}

	// インスタンスのメモリ解放
	delete instance_;

}

int EffectResManager::GetResourceId(TYPE type)
{
	// 指定されたタイプがなかったら-1を返す
	if (resourceIds_.count(type) == 0)
	{
		return -1;
	}

	// 指定したタイプに対応するIdを返す
	return resourceIds_[type];
}

EffectResManager::EffectResManager(void)
{
}

void EffectResManager::Load(void)
{

	// 爆発地面エフェクトのロード
	resourceIds_.emplace(TYPE::BLAST_GROUND,
		LoadEffekseerEffect(
			(Application::PATH_EFFECT + "Blast/Blast.efkefc").c_str()));
	
	// 爆発ヒットエフェクトのロード
	resourceIds_.emplace(TYPE::BLAST_HIT,
		LoadEffekseerEffect(
			(Application::PATH_EFFECT + "BlastHit/BlastHit.efkefc").c_str()));

}

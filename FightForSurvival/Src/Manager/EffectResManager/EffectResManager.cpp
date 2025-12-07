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
}

EffectResManager& EffectResManager::GetInstance(void)
{
	return *instance_;
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

int EffectResManager::PlayEffect(float scale, VECTOR dir, VECTOR pos, EffectResManager::TYPE effectType)
{
	// エフェクトの再生
	int resId = EffectResManager::GetInstance().GetResourceId(effectType);
	int effectPlayId = PlayEffekseer3DEffect(resId);

	// エフェクトの大きさ
	SetScalePlayingEffekseer3DEffect(
		effectPlayId, scale, scale, scale);

	// エフェクトの回転
	dir.y += 90.0f * 180 / DX_PI_F;
	SetRotationPlayingEffekseer3DEffect(
		effectPlayId, dir.x, dir.y, dir.z);

	// エフェクトの位置
	SetPosPlayingEffekseer3DEffect(
		effectPlayId, pos.x, pos.y, pos.z);

	return effectPlayId;
}

EffectResManager::EffectResManager(void)
{
}

void EffectResManager::Load(void)
{
	
	// プレイヤー攻撃チャージ時のエフェクトのロード
	resourceIds_.emplace(TYPE::PLAYER_MAGIC_CHARGE,
		LoadEffekseerEffect(
			(Application::PATH_EFFECT + "SonicBoom.efkefc").c_str()));
	
	// プレイヤー攻撃エフェクトのロード
	resourceIds_.emplace(TYPE::PLAYER_MAGIC_SHOT,
		LoadEffekseerEffect(
			(Application::PATH_EFFECT + "SonicBoom.efkefc").c_str()));

	// プレイヤー攻撃の爆発エフェクトのロード
	resourceIds_.emplace(TYPE::BLAST,
		LoadEffekseerEffect(
			(Application::PATH_EFFECT + "Blast/Blast.efkefc").c_str()));

	// コウモリ攻撃エフェクトのロード
	resourceIds_.emplace(TYPE::BAT_MAGIC,
		LoadEffekseerEffect(
			(Application::PATH_EFFECT + "BlastHit/BlastHit.efkefc").c_str()));

	// ドラゴン攻撃エフェクトのロード
	resourceIds_.emplace(TYPE::DRAGON_MAGIC,
		LoadEffekseerEffect(
			(Application::PATH_EFFECT + "BlastHit/BlastHit.efkefc").c_str()));

}

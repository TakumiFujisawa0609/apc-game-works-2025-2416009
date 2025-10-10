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

	// 外部ファイルでやりたい
	for (int i = 0; i < static_cast<int>(PLAYER_UPGRADE::MAX); i++)
	{
		upgrades_[i].type_ = static_cast<PLAYER_UPGRADE>(i);
		switch (upgrades_[i].type_)
		{
		case PLAYER_UPGRADE::NON:
			upgrades_[i].upNum_ = 0.0f;
			upgrades_[i].name = "NON";
			upgrades_[i].desc = "何も強化しない";
			break;
		case PLAYER_UPGRADE::RESTOCK_POTION:
			upgrades_[i].upNum_ = RESTOCK_POTION_NUM;
			upgrades_[i].name = "RESTOCK_POTION";
			upgrades_[i].desc = "ポーションを補充する";
			break;
		case PLAYER_UPGRADE::SPEED_UP:
			upgrades_[i].upNum_ = SPPED_UP_NUM;
			upgrades_[i].name = "SPEED_UP";
			upgrades_[i].desc = "移動速度を上げる";
			break;
		case PLAYER_UPGRADE::STAMINA_UP:
			upgrades_[i].upNum_ = STAMINA_UP_NUM;
			upgrades_[i].name = "STAMINA_UP";
			upgrades_[i].desc = "スタミナの最大値を上げる";
			break;
		case PLAYER_UPGRADE::HP_UP:
			upgrades_[i].upNum_ = HP_UP_NUM;
			upgrades_[i].name = "HP_UP";
			upgrades_[i].desc = "HPの最大値を上げる";
			break;
		case PLAYER_UPGRADE::HEAL_HP:
			upgrades_[i].upNum_ = HEAL_HP_NUM;
			upgrades_[i].name = "HEAL_HP";
			upgrades_[i].desc ="HPを回復する";
			break;
		default:
			break;
		}
	}

}

void UpgradeManager::Update(void)
{
	// アップグレードの選択
	SelectUpgrade();
}

void UpgradeManager::Draw(void)
{


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

void UpgradeManager::SelectUpgrade(void)
{


	//if(// 確定したら,)
	//	{
	//		ApplyUpgrade(i);
	//	}
}

void UpgradeManager::ApplyUpgrade(int selectIndex)
{
	// アップグレードが確定した種類情報をもらう
	UpgradeData upgrade = upgrades_[selectIndex];
	// プレイヤーに反映する
	player_->Upgrade(upgrade.type_, upgrade.upNum_);
}

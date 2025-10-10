#include "UpgradeManager.h"

#include <random>

#include "../../../Manager/CollisionManager.h"
#include "../../../Application.h"

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

	for (int i = 0; i < static_cast<int>(PLAYER_UPGRADE::MAX); i++)
	{
		switch (i)
		{
		case 0:
			upgradeData_[i].upNum_ = 0.0f;
			upgradeData_[i].name = "NON";
			upgradeData_[i].desc = "何も強化しない";
			break;
		case 1:
			upgradeData_[i].upNum_ = RESTOCK_POTION_NUM;
			upgradeData_[i].name = "RESTOCK_POTION";
			upgradeData_[i].desc = "ポーションを補充する";
			break;
		case 2:
			upgradeData_[i].upNum_ = SPPED_UP_NUM;
			upgradeData_[i].name = "SPEED_UP";
			upgradeData_[i].desc = "移動速度を上げる";
			break;
		case 3:
			upgradeData_[i].upNum_ = STAMINA_UP_NUM;
			upgradeData_[i].name = "STAMINA_UP";
			upgradeData_[i].desc = "スタミナの最大値を上げる";
			break;
		case 4:
			upgradeData_[i].upNum_ = HP_UP_NUM;
			upgradeData_[i].name = "HP_UP";
			upgradeData_[i].desc = "HPの最大値を上げる";
			break;
		case 5:
			upgradeData_[i].upNum_ = HEAL_HP_NUM;
			upgradeData_[i].name = "HEAL_HP";
			upgradeData_[i].desc ="HPを回復する";
			break;
		default:
			break;
		}
	}

	// vectorの上限値を決める
	selectUpgrades_.reserve(SELECT_UPGRADES_NUM);

	//pos_[]

	isSelect_ = false;
}

void UpgradeManager::Update(void)
{
	// アップグレードの選択
	SelectUpgrade();
}

void UpgradeManager::Draw(void)
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, ALPHA);
	DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, 0xa9a9a9, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0.0f);

	for (int i = 0; i < SELECT_UPGRADES_NUM; ++i)
	{
		DrawBox(pos_[i].x, pos_[i].y, pos_[i].x + 100, pos_[i].y + 80, 0x808080, true);

		// 表示名
		auto name = (upgradeData_[static_cast<int>(selectUpgrades_[i])].name).c_str();
		DrawString(pos_[i].x + 40, pos_[i].y + 40, name, 0xffffff);

		// 説明文
		auto desc = (upgradeData_[static_cast<int>(selectUpgrades_[i])].desc).c_str();
		DrawString(pos_[i].x + 40, pos_[i].y + 60, desc, 0xffffff);

		// 画像描画
		//DrawRotaGraph(pos_[i].x, pos_[i].y, 1.0f, 0.0f, upgrades_[i].image_, true, false);
	}
}

void UpgradeManager::Destroy(void)
{
	// 使い終わったらクリア
	allUpgrades_.clear();
	selectUpgrades_.clear();

	if (instance_ != nullptr)
	{
		// インスタンスのメモリ解放
		delete instance_;
		instance_ = nullptr;
	}
}

void UpgradeManager::SetIsSelect(bool isSelect)
{
	isSelect_ = isSelect;

	// 選択中の本の情報を削除する
	selectUpgrades_.clear();
}

UpgradeManager::UpgradeManager(void)
{
}

void UpgradeManager::SelectUpgrade(void)
{
	if (isSelect_)
	{
		// 乱数生成器
		std::random_device rd; // ハードウェア乱数からシードを生成
		std::mt19937 gen(rd()); // メルセンヌ・ツイスタ乱数生成器

		// allUpgrades_をシャッフル
		std::shuffle(allUpgrades_.begin(), allUpgrades_.end(), gen);

		// 先頭の４つをselectUpgrades_にコピーする
		for (int i = 0; i < SELECT_UPGRADES_NUM; ++i) {
			selectUpgrades_.push_back(allUpgrades_[i]);
		}

		isSelect_ = false;
	}
	// 当たり判定取る
	//finalizeUpgrade_ = selectUpgrades_[i];
	// state = APPLY;

	//if(upgrade_->GetState == APPLY)
	//	{
	//		auto finalizeUpgrade = upgrade_->GetFinalizeUpgrade();
	// 
	//		if(finalizeUpgrade == PLAYER_UPGRADE::NON)
	//		{
	//			isUpgradeEnd_ = true;
	//		}
	//		else
	//		{
	//			ApplyUpgrade(finalizeUpgrade);
	//			isUpgradeEnd_ = true;
	//		}
	//		// isUpgradeEnd_のゲット関数作ってWAVE側で見て、trueだったら、準備時間を強制的に終わらせる
	//	}
}

void UpgradeManager::ApplyUpgrade(PLAYER_UPGRADE finalizeUpgrade)
{
	// プレイヤーに強化指示を出し能力強化を反映する
	player_->Upgrade(finalizeUpgrade, upgradeData_[static_cast<int>(finalizeUpgrade)].upNum_);
	//player_->Upgrade(finalizeUpgrade, upgrade_->GetUpNum(finalizeUpgrade));
}


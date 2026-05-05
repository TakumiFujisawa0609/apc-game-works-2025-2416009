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

void UpgradeManager::Load(Player* player)
{
	// プレイヤーのポインタを取得
	player_ = player;

	// アップグレードクラスを生成
	upgrade_ = new Upgrade();
	upgrade_->Load();
}

void UpgradeManager::Init(void)
{
	// 始めはアップグレードを行わないためEndフラグを立てる
	isUpgradeEnd_ = true;

	// アップグレードクラス初期化
	upgrade_->Init();
}

void UpgradeManager::Update(void)
{
	// アップグレードモードが始まっていなければ処理を行わない
	if (isUpgradeEnd_)
	{
		return;
	}

	// アップグレード更新処理
	upgrade_->Update();

	// アップグレードの状態が確定状態だったら
	if (upgrade_->GetState() == Upgrade::STATE::APPLY)
	{
		// 確定したアップグレード情報を受け取る
		auto finalizeUpgrade = upgrade_->GetFinalizeUpgrade();

		// アップグレードを適用
		ApplyUpgrade(finalizeUpgrade);

		// アップグレードモードを終了
		isUpgradeEnd_ = true;
	}
}

void UpgradeManager::Draw(void)
{
	// アップグレードモードが始まっていなければ処理を行わない
	if (isUpgradeEnd_)
	{
		return;
	}

	// アップグレード描画処理
	upgrade_->Draw();
}

void UpgradeManager::Destroy(void)
{
	// アップグレードクラスの削除
	upgrade_->Release();
	delete upgrade_;
	upgrade_ = nullptr;

	if (instance_ != nullptr)
	{
		// インスタンスのメモリ解放
		delete instance_;
		instance_ = nullptr;
	}
}

void UpgradeManager::StartIsUpgrade(void)
{
	// アップグレードモードを開始
	isUpgradeEnd_ = false;

	// アップグレード内容を選択する
	upgrade_->ChangeState(Upgrade::STATE::SELECT);
}

void UpgradeManager::StopIsUpgrade(void)
{
	// アップグレードモードを終了
	isUpgradeEnd_ = true;

	// アップグレード内容を強制適用
	upgrade_->ChangeState(Upgrade::STATE::APPLY);
}

UpgradeManager::UpgradeManager(void)
{
}

void UpgradeManager::ApplyUpgrade(PLAYER_UPGRADE finalizeUpgrade)
{
	// プレイヤーに強化指示を出し能力強化を反映する
	player_->Upgrade(finalizeUpgrade, upgrade_->GetUpNum(finalizeUpgrade));
}


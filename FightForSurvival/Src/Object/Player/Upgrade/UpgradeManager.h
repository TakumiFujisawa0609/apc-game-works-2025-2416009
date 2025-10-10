#pragma once

#include <string>
#include <vector>

#include "../Player.h"

class UpgradeManager
{
public:

	// ポーションの補充値
	static constexpr float RESTOCK_POTION_NUM = 1.0f;
	// スピード強化値
	static constexpr float SPPED_UP_NUM = 3.0f;
	// スタミナ強化値
	static constexpr float STAMINA_UP_NUM = 3.0f;
	// HP強化値
	static constexpr float HP_UP_NUM = 2.0f;
	// HP回復値
	static constexpr float HEAL_HP_NUM = 2.0f;

	// 選択する強化の数
	static constexpr int SELECT_UPGRADES_NUM = 4;

	static constexpr int ALPHA = 128;

	// 明示的にインステンスを生成する
	static void CreateInstance(void);

	// 静的インスタンスの取得
	static UpgradeManager& GetInstance(void);

	// 初期化
	void Init(Player* player);
	// 更新
	void Update(void);
	// 描画
	void Draw(void);

	// 解放処理
	void Destroy(void);

	void SetIsSelect(bool isSelect);


private:

	// 静的インスタンス
	static UpgradeManager* instance_;

	// プレイヤーのインスタンス
	Player* player_;

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	UpgradeManager(void);

	// コピーコンストラクタも同様
	UpgradeManager(const UpgradeManager& instance) = default;

	// デストラクタも同様
	~UpgradeManager(void) = default;

	struct UpgradeData
	{
		// 強化数値
		float upNum_;
		// 表示用の名前
		std::string name;  
		// 説明文
		std::string desc;  

		// 画像ハンドル
		int image_;
	};

	// 種別
	UpgradeData upgradeData_[static_cast<int>(PLAYER_UPGRADE::MAX)];

	// 選択されたアップグレードの表示座標
	Vector2 pos_[SELECT_UPGRADES_NUM];

	// 4つ選択する前のアップグレードの全種類
	std::vector<PLAYER_UPGRADE>allUpgrades_;
	// 4つ選択した後のアップグレードの全種類
	std::vector<PLAYER_UPGRADE>selectUpgrades_;

	bool isSelect_;

	PLAYER_UPGRADE finalizeUpgrade_;

	// どの能力をアップグレードするか選択を行う
	void SelectUpgrade(void);

	void ApplyUpgrade(PLAYER_UPGRADE finalizeUpgrade);
};


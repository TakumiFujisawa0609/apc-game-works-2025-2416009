#pragma once

#include <string>

#include "../Player.h"

class UpgradeManager
{
public:

	static constexpr float RESTOCK_POTION_NUM = 1.0f;
	static constexpr float SPPED_UP_NUM = 3.0f;
	static constexpr float STAMINA_UP_NUM = 3.0f;
	static constexpr float HP_UP_NUM = 2.0f;
	static constexpr float HEAL_HP_NUM = 2.0f;

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
		// アップグレードのタイプ
		PLAYER_UPGRADE type_;
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
	UpgradeData upgrades_[static_cast<int>(PLAYER_UPGRADE::MAX)];

	// どの能力をアップグレードするか選択を行う
	void SelectUpgrade(void);

	void ApplyUpgrade(int selectIndex);
};


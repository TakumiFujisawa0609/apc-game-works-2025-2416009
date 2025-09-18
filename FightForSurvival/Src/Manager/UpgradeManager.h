#pragma once

class Player;

class UpgradeManager
{
public:

	enum class ABILITY_TYPE
	{
		SPEED_UP,
		HEAL_HP,
		HP_UP,
		STAMINA_UP,
	};

	// 明示的にインステンスを生成する
	static void CreateInstance(void);

	// 静的インスタンスの取得
	static UpgradeManager& GetInstance(void);

	// 初期化
	void Init(Player* player);



	// 解放処理
	void Destroy(void);

	// ゲーム終了か
	void SetIsEnd(const bool flg) { isEnd_ = flg; }

	// ゲーム終了か
	bool GetIsEnd(void)const { return isEnd_; }

private:

	// 静的インスタンス
	static UpgradeManager* instance_;

	// プレイヤーのインスタンス
	Player* player_;

	// ゲーム終了か
	bool isEnd_;

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	UpgradeManager(void);

	// コピーコンストラクタも同様
	UpgradeManager(const UpgradeManager& instance) = default;

	// デストラクタも同様
	~UpgradeManager(void) = default;
};


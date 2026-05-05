#pragma once

#include "../Player.h"
#include "Upgrade.h"

class UpgradeManager
{
public:

	// 明示的にインステンスを生成する
	static void CreateInstance(void);

	// 静的インスタンスの取得
	static UpgradeManager& GetInstance(void);

	// 初期化
	void Load(Player* player);
	void Init(void);
	// 更新
	void Update(void);
	// 描画
	void Draw(void);

	// 解放処理
	void Destroy(void);

	// アップグレードモードを開始
	void StartIsUpgrade(void);
	// アップグレードモードを終了
	void StopIsUpgrade(void);
	// アップグレードモードが終わったかを渡す
	bool GetIsUpgradeEnd(void)const { return isUpgradeEnd_; }

private:

	// 静的インスタンス
	static UpgradeManager* instance_;

	// プレイヤーのインスタンス
	Player* player_;

	// アップグレードクラスのインスタンス
	Upgrade* upgrade_;

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	UpgradeManager(void);

	// コピーコンストラクタも同様
	UpgradeManager(const UpgradeManager& instance) = default;

	// デストラクタも同様
	~UpgradeManager(void) = default;

	// アップグレードモードが終わったか　true / 終了 , false/ 終了していない
	bool isUpgradeEnd_;

	// プレイヤーにアップグレードの指示を行う
	void ApplyUpgrade(PLAYER_UPGRADE finalizeUpgrade);

};


#pragma once

#include <string>
#include <vector>

#include "../Player.h"
#include "../../../Common/Vector2/Vector2.h"

class Upgrade
{
public:

	// ポーションの補充値
	static constexpr float RESTOCK_POTION_NUM = 1.0f;
	// スピード強化値
	static constexpr float SPPED_UP_NUM = 5.0f;
	// スタミナ強化値
	static constexpr float STAMINA_UP_NUM = 5.0f;
	// HP強化値
	static constexpr float HP_UP_NUM = 2.0f;
	// HP回復値
	static constexpr float HEAL_HP_NUM = 2.0f;

	// アルファ値
	static constexpr int ALPHA = 128;

	// 基準座標
	static constexpr int POS_X = 200;
	static constexpr int POS_Y = 100;

	// 当たり判定を行うサイズ
	static constexpr int COL_SIZE_X = 300;
	static constexpr int COL_SIZE_Y = 200;

	// 画像間(余白)の大きさ
	static constexpr int SPACE_X = COL_SIZE_X + 150;
	static constexpr int SPACE_Y = COL_SIZE_Y + 50;

	// 描画画像の縦横数
	static constexpr int DRAW_NUM_XY = 2;

	enum class PLACE
	{
		TOP_LEFT,
		TOP_RIGHT,
		BOTTOM_LEFT,
		BOTTOM_RIGHT,
		MAX
	};

	enum class STATE
	{
		SELECT,
		CONFIRM,
		APPLY,
		NON,
	};

	Upgrade(void);
	~Upgrade(void);

	void Load(void);
	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	float GetUpNum(PLAYER_UPGRADE upgradeType);
	PLAYER_UPGRADE GetFinalizeUpgrade(void)const { return finalizeUpgrade_; }
	STATE GetState(void)const { return state_; }

	void ChangeState(STATE state);
	void ChangePlace(PLACE place) { place_ = place; }

private:

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
	Vector2 pos_[static_cast<int>(PLACE::MAX)];

	// 4つ選択する前のアップグレードの全種類
	std::vector<PLAYER_UPGRADE>allUpgrades_;
	// 4つ選択した後のアップグレードの全種類
	std::vector<PLAYER_UPGRADE>selectUpgrades_;

	PLAYER_UPGRADE finalizeUpgrade_;

	STATE state_;
	PLACE place_;

	// どの能力をアップグレードするか選択を行う
	void SelectUpgrade(void);
	void ConfirmUpgrade(void);

	// マウスの選択処理
	void Collision(void);

	// パッドの選択処理
	void PadSelect(void);

	void SelectInit(void);
	void ConfirmInit(void);
	void ApplyInit(void);
};


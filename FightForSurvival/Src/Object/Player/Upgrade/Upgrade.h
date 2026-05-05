#pragma once

#include <string>
#include <vector>

#include "../Player.h"
#include "../../../Common/Vector2/Vector2.h"

class UIManager;
class TextureManager;

class Upgrade
{
public:

	// ポーションの補充値
	static constexpr float RESTOCK_POTION_NUM = 5.0f;
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
	static constexpr int POS_X = 300;
	static constexpr int POS_Y = 200;

	// 当たり判定を行うサイズ
	static constexpr int COL_SIZE_X = 256;
	static constexpr int COL_SIZE_Y = 128;

	// 画像間(余白)の大きさ
	static constexpr int SPACE_X = COL_SIZE_X + 150;
	static constexpr int SPACE_Y = COL_SIZE_Y + 50;

	// 描画画像の縦横数
	static constexpr int DRAW_NUM_XY = 2;

	// 背景の色
	static constexpr unsigned int BACK_COLOR = 0xa9a9a9;

	// 選択された場所の種類
	enum class PLACE
	{
		TOP_LEFT,		// 左上
		TOP_RIGHT,		// 右上
		BOTTOM_LEFT,	// 左下
		BOTTOM_RIGHT,	// 右下
		MAX
	};

	// 状態
	enum class STATE
	{
		SELECT,		// 選択
		CONFIRM,	// 確認
		APPLY,		// 適用
		NON,
	};

	// ボタンの状態
	enum BUTTON_STATE
	{
		DEFAULE,		// 通常
		HOVER,			// 選択状態
		TRIGGER_DOWN,	// ボタン押下

		MAX,		// 最大数
	};

	// コンストラクタ
	Upgrade(void);
	// デストラクタ
	~Upgrade(void);

	// 読み込みsyオリ
	void Load(void);
	// 初期化処理
	void Init(void);
	// 更新処理
	void Update(void);
	// 描画処理
	void Draw(void);
	// 解放処理
	void Release(void);

	// 強化値を渡す(アップグレードの種類によって変化)
	float GetUpNum(PLAYER_UPGRADE upgradeType)const { return upNum_[static_cast<int>(upgradeType)]; }
	// 最終的にアップグレードする物のを渡す
	PLAYER_UPGRADE GetFinalizeUpgrade(void)const { return finalizeUpgrade_; }
	// 状態を渡す
	STATE GetState(void)const { return state_; }

	// 指定された状態へ変更
	void ChangeState(STATE state);
	// 指定された場所の種類へ変更
	void ChangePlace(PLACE place) { place_ = place; }

private:

	// 下地
	int baseHandle_[static_cast<int>(BUTTON_STATE::MAX)];
	// テキスト
	int textHandle_[static_cast<int>(PLAYER_UPGRADE::MAX)];
	// ボタンの状態
	BUTTON_STATE buttonState_[static_cast<int>(PLAYER_UPGRADE::MAX)];
	// 今押されたか
	bool isTrgDown_[static_cast<int>(PLAYER_UPGRADE::MAX)];

	// 強化数値
	float upNum_[static_cast<int>(PLAYER_UPGRADE::MAX)];

	// 選択されたアップグレードの表示座標
	Vector2 pos_[static_cast<int>(PLACE::MAX)];

	// 4つ選択する前のアップグレードの全種類
	std::vector<PLAYER_UPGRADE>allUpgrades_;
	// 4つ選択した後のアップグレードの全種類
	std::vector<PLAYER_UPGRADE>selectUpgrades_;

	// 最終的にアップグレードする物
	PLAYER_UPGRADE finalizeUpgrade_;

	// 状態
	STATE state_;

	// 選択された場所の種類
	PLACE place_;

	// どの能力をアップグレードするか選択を行う
	void SelectUpgrade(void);
	// 確認処理
	void ConfirmUpgrade(void);

	// マウスの選択処理
	void MouseSelect(void);

	// パッドの選択処理
	void PadSelect(void);

	// 選択処理時の初期化
	void SelectInit(void);
	// 確認処理時の初期化
	void ConfirmInit(void);
	// 適用時の初期化
	void ApplyInit(void);

	// 選択中のものの変更が行われているかつ何かを選択していたら処理を行う
	void CompPlace(const PLACE prevPlace);

};


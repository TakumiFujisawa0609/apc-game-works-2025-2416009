#include "Upgrade.h"

#include <random>
#include <algorithm>

#include "../../../Manager/CollisionManager.h"
#include "../../../Manager/InputManager.h"
#include "../../../Application.h"

Upgrade::Upgrade(void)
{
}

Upgrade::~Upgrade(void)
{
}

void Upgrade::Load(void)
{
}

void Upgrade::Init(void)
{
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
			upgradeData_[i].desc = "ポーションを1つ補充する";
			break;
		case 2:
			upgradeData_[i].upNum_ = SPPED_UP_NUM;
			upgradeData_[i].name = "SPEED_UP";
			upgradeData_[i].desc = "移動速度を5上げる";
			break;
		case 3:
			upgradeData_[i].upNum_ = STAMINA_UP_NUM;
			upgradeData_[i].name = "STAMINA_UP";
			upgradeData_[i].desc = "スタミナの最大値を5上げる";
			break;
		case 4:
			upgradeData_[i].upNum_ = HP_UP_NUM;
			upgradeData_[i].name = "HP_UP";
			upgradeData_[i].desc = "HPの最大値を2上げる";
			break;
		case 5:
			upgradeData_[i].upNum_ = HEAL_HP_NUM;
			upgradeData_[i].name = "HEAL_HP";
			upgradeData_[i].desc = "HPを2回復する";
			break;
		default:
			break;
		}
	}

	// 全ての本の種類を保持するvector
	allUpgrades_ = {
		PLAYER_UPGRADE::RESTOCK_POTION, PLAYER_UPGRADE::SPEED_UP, PLAYER_UPGRADE::STAMINA_UP,
		PLAYER_UPGRADE::HP_UP, PLAYER_UPGRADE::HEAL_HP,
	};

	// vectorの上限値を決める
	selectUpgrades_.reserve(static_cast<int>(PLACE::MAX));

	// 選択決定内容をNONに初期化する
	finalizeUpgrade_ = PLAYER_UPGRADE::NON;

	// 座標初期化
	int i = static_cast<int>(PLACE::TOP_LEFT);
	for (int y = 0; y < DRAW_NUM_XY; y++)
	{
		for (int x = 0; x < DRAW_NUM_XY; x++)
		{
			pos_[i] = { POS_X + x * SPACE_X, POS_Y + y * SPACE_Y };
			i++;
		}
	}

	ChangeState(STATE::NON);
}

void Upgrade::Update(void)
{
	switch (state_)
	{
	case Upgrade::STATE::SELECT:

		// アップグレードを4つ選択
		SelectUpgrade();

		break;
	case Upgrade::STATE::CONFIRM:

		// 確認を行う
		ConfirmUpgrade();

		break;
	case Upgrade::STATE::APPLY:
		break;
	default:
		break;
	}
}

void Upgrade::Draw(void)
{
	// 背景
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, ALPHA);
	DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, 0xa9a9a9, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	auto boxColor = 0x696969;
	auto charColor = 0xffffff;

	for (int i = 0; i < selectUpgrades_.size(); ++i)
	{
		boxColor = 0x696969;
		charColor = 0xffffff;

		if (place_ == static_cast<PLACE>(i))
		{
			// 選択中の物は色を変える
			boxColor = 0xf5f5f5;
			charColor = 0x00ff00;
		}

		DrawBox(pos_[i].x, pos_[i].y, pos_[i].x + COL_SIZE_X, pos_[i].y + COL_SIZE_Y, boxColor, true);

		// 表示名
		auto name = (upgradeData_[static_cast<int>(selectUpgrades_[i])].name).c_str();
		DrawString(pos_[i].x + 40, pos_[i].y + 60, name, charColor);

		// 説明文
		auto desc = (upgradeData_[static_cast<int>(selectUpgrades_[i])].desc).c_str();
		DrawString(pos_[i].x + 40, pos_[i].y + 100, desc, charColor);

		// 画像描画
		//DrawRotaGraph(pos_[i].x, pos_[i].y, 1.0f, 0.0f, upgrades_[i].image_, true, false);
	}
}

void Upgrade::Release(void)
{
	// 使い終わったらクリア
	allUpgrades_.clear();
	selectUpgrades_.clear();
}

float Upgrade::GetUpNum(PLAYER_UPGRADE upgradeType)
{
	return upgradeData_[static_cast<int>(upgradeType)].upNum_;
}

void Upgrade::SelectUpgrade(void)
{
	selectUpgrades_.clear();
	// 乱数生成器
	std::random_device rd; // ハードウェア乱数からシードを生成
	std::mt19937 gen(rd()); // メルセンヌ・ツイスタ乱数生成器

	// allUpgrades_をシャッフル
	std::shuffle(allUpgrades_.begin(), allUpgrades_.end(), gen);

	const int max_choices = static_cast<int>(PLACE::MAX);
	const int available_upgrades = static_cast<int>(allUpgrades_.size());

	// std::min を std:: のスコープで明示的に呼び出す(基本max_choicesのほうが大きくなるが、vectorのsize外に出ないように比較しておく)
	int limit = (std::min)(max_choices, available_upgrades);

	// 先頭の４つをselectUpgrades_にコピーする
	for (int i = 0; i < limit; ++i) {
		selectUpgrades_.push_back(allUpgrades_[i]);
	}

	// ステートを確認に移行
	ChangeState(STATE::CONFIRM);
}

void Upgrade::ConfirmUpgrade(void)
{
	InputManager& ins = InputManager::GetInstance();

	// 引数の座標によって選択中のものを変化させる
	Collision();

	// 何か選択していて、確定ボタンが押されたら処理を行う
	if (ins.Confirm() && place_ != PLACE::MAX)
	{
		finalizeUpgrade_ = selectUpgrades_[static_cast<int>(place_)];

		ChangeState(STATE::APPLY);
	}
}

void Upgrade::Collision(void)
{
	// 当たり判定取る
	for (int i = 0; i < static_cast<int>(PLACE::MAX); i++)
	{
		if(CollisionManager::RectangleAndMouse(pos_[i], COL_SIZE_X, COL_SIZE_Y))
		{
			place_ = static_cast<PLACE>(i);
			break;
		}
		else
		{
			place_ = PLACE::MAX;
		}
	}
}

void Upgrade::ChangeState(STATE state)
{
	state_ = state;

	switch (state_)
	{
	case Upgrade::STATE::SELECT:
		SelectInit();
		break;
	case Upgrade::STATE::CONFIRM:
		ConfirmInit();
		break;
	case Upgrade::STATE::APPLY:
		ApplyInit();
		break;
	case Upgrade::STATE::NON:
		break;
	default:
		break;
	}

}

void Upgrade::SelectInit(void)
{
	// 選択決定内容をNONに初期化する
	finalizeUpgrade_ = PLAYER_UPGRADE::NON;

	// マウスを表示させる
	SetMouseDispFlag(true);
}

void Upgrade::ConfirmInit(void)
{
}

void Upgrade::ApplyInit(void)
{
	// マウスを表示させる
	SetMouseDispFlag(false);

	// マウスの位置を真ん中に初期化する
	SetMousePoint(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2);
}

#include "Upgrade.h"

#include <random>
#include <algorithm>

#include "../../../Utility/Collision/CollisionUtility.h"
#include "../../../Manager/InputManager.h"
#include "../../../Manager/SoundManager.h"
#include "../../../Manager/SystemManager.h"
#include "../../../Application.h"

Upgrade::Upgrade(void)
{
	// 選択決定内容をNONに初期化する
	finalizeUpgrade_ = PLAYER_UPGRADE::MAX;

	// 状態の初期化
	state_ = STATE::NON;
	// 選択された場所の種類の初期化
	place_ = PLACE::MAX;

	// 選択中のボタンの状態初期化
	for (int i = 0; i < static_cast<int>(PLAYER_UPGRADE::MAX); i++)
	{
		buttonState_[i] = BUTTON_STATE::DEFAULE;
		isTrgDown_[i] = false;
	}
}

Upgrade::~Upgrade(void)
{
}

void Upgrade::Load(void)
{
	// ベースロード
	baseHandle_[static_cast<int>(BUTTON_STATE::DEFAULE)] = LoadGraph("Data/Image/UI/Upgrade/Base_0.png");
	baseHandle_[static_cast<int>(BUTTON_STATE::HOVER)] = LoadGraph("Data/Image/UI/Upgrade/Base_1.png");
	baseHandle_[static_cast<int>(BUTTON_STATE::TRIGGER_DOWN)] = LoadGraph("Data/Image/UI/Upgrade/Base_2.png");

	// テキストロード
	textHandle_[static_cast<int>(PLAYER_UPGRADE::HP_UP)] = LoadGraph("Data/Image/UI/Upgrade/HP_UP.png");
	textHandle_[static_cast<int>(PLAYER_UPGRADE::HEAL_HP)] = LoadGraph("Data/Image/UI/Upgrade/HP_HEAL.png");
	textHandle_[static_cast<int>(PLAYER_UPGRADE::SPEED_UP)] = LoadGraph("Data/Image/UI/Upgrade/SPEED_UP.png");
	textHandle_[static_cast<int>(PLAYER_UPGRADE::STAMINA_UP)] = LoadGraph("Data/Image/UI/Upgrade/STAMINA_UP.png");
	textHandle_[static_cast<int>(PLAYER_UPGRADE::RESTOCK_POTION)] = LoadGraph("Data/Image/UI/Upgrade/POTION_RESTOCK.png");
}

void Upgrade::Init(void)
{

	// アップグレードの強化値を初期化
	for (int i = 0; i < static_cast<int>(PLAYER_UPGRADE::MAX); i++)
	{
		switch (i)
		{
		// ポーションの数を追加
		case static_cast<int>(PLAYER_UPGRADE::RESTOCK_POTION):
			upNum_[i] = RESTOCK_POTION_NUM;
			break;
		// 移動速度上昇
		case static_cast<int>(PLAYER_UPGRADE::SPEED_UP):
			upNum_[i] = SPPED_UP_NUM;
			break;
		// スタミナ上昇
		case static_cast<int>(PLAYER_UPGRADE::STAMINA_UP):
			upNum_[i] = STAMINA_UP_NUM;
			break;
		// HP上昇
		case static_cast<int>(PLAYER_UPGRADE::HP_UP):
			upNum_[i] = HP_UP_NUM;
			break;
		// HP回復
		case static_cast<int>(PLAYER_UPGRADE::HEAL_HP):
			upNum_[i] = HEAL_HP_NUM;
			break;
		default:
			break;
		}
	}

	// 全てのアップグレードの種類を保持するvector
	allUpgrades_ = {
		PLAYER_UPGRADE::RESTOCK_POTION, PLAYER_UPGRADE::SPEED_UP, PLAYER_UPGRADE::STAMINA_UP,
		PLAYER_UPGRADE::HP_UP, PLAYER_UPGRADE::HEAL_HP,
	};

	// vectorの上限値を決める
	selectUpgrades_.reserve(static_cast<int>(PLACE::MAX));

	// 選択決定内容をNONに初期化する
	finalizeUpgrade_ = PLAYER_UPGRADE::MAX;

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

	// 状態の初期化
	ChangeState(STATE::NON);
	// 選択された場所の種類の初期化
	ChangePlace(PLACE::MAX);
}

void Upgrade::Update(void)
{
	switch (state_)
	{
	case Upgrade::STATE::SELECT:

		// アップグレードを内容を選択
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
	DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, BACK_COLOR, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// 選択の状態に合わせて描画
	for (int i = 0; i < selectUpgrades_.size(); ++i)
	{
		DrawGraph(pos_[i].x, pos_[i].y,
			baseHandle_[static_cast<int>(buttonState_[i])], true);

		DrawGraph(pos_[i].x, pos_[i].y,
			textHandle_[static_cast<int>(selectUpgrades_[i])], true);
	}

}

void Upgrade::Release(void)
{
	// 解放
	allUpgrades_.clear();
	selectUpgrades_.clear();
}

void Upgrade::SelectUpgrade(void)
{
	// 初期化しておく
	selectUpgrades_.clear();
	// 乱数生成器
	std::random_device rd; // ハードウェア乱数からシードを生成
	std::mt19937 gen(rd()); // メルセンヌ・ツイスタ乱数生成器

	// allUpgrades_をシャッフル
	std::shuffle(allUpgrades_.begin(), allUpgrades_.end(), gen);

	// 選べる最大値
	const int max_choices = static_cast<int>(PLACE::MAX);

	// 先頭の４つをselectUpgrades_にコピーする
	for (int i = 0; i < max_choices; ++i) {
		selectUpgrades_.push_back(allUpgrades_[i]);
	}

	// ステートを確認に移行
	ChangeState(STATE::CONFIRM);
	// 選択を左上にする
	ChangePlace(PLACE::TOP_LEFT);
}

void Upgrade::ConfirmUpgrade(void)
{
	// 使用しているデバイスによって処理変化
	if (SystemManager::GetInstance().GetIsDevice())
	{
		// マウス選択処理
		MouseSelect();
	}
	else
	{
		// パッド選択処理
		PadSelect();
	}
}

void Upgrade::MouseSelect(void)
{
	// 前の選択中の種類を保存しておく
	auto prevPlace = place_;

	// 選択できる種類分回す
	for (int i = 0; i < static_cast<int>(PLACE::MAX); i++)
	{
		// 全て初期化する
		buttonState_[i] = BUTTON_STATE::DEFAULE;

		// マウスと当たっていたら
		if (CollisionUtility::RectangleAndMouse(pos_[i], COL_SIZE_X, COL_SIZE_Y))
		{
			// 選択中のものの種類を設定
			ChangePlace(static_cast<PLACE>(i));

			// 何か選択していて、確定ボタンが押されたら処理を行う
			if(InputManager::GetInstance().ConfirmUp() && isTrgDown_[i])
			{
				// 選択したアップグレードを確定させる
				finalizeUpgrade_ = selectUpgrades_[static_cast<int>(place_)];

				// 確定に移行
				ChangeState(STATE::APPLY);

				// 決定SEをながす
				SoundManager::GetInstance().Play(SoundManager::SE::DECIDE);

			}
			// 押されていて今押されていなかったら
			else if (InputManager::GetInstance().Confirm() && !isTrgDown_[i])
			{
				// 今押されたフラグを立てる
				isTrgDown_[i] = true;
			}

			// 今押されたフラグによってボタンの状態を変更
			if (isTrgDown_[i])
			{
				buttonState_[i] = BUTTON_STATE::TRIGGER_DOWN;
			}
			else
			{
				buttonState_[i] = BUTTON_STATE::HOVER;
			}

			break;
		}
		// マウスに当っていなかったら
		else
		{
			// 何も触っていない状態にする
			ChangePlace(PLACE::MAX);
			// 今押されたフラグも初期化
			isTrgDown_[i] = false;
		}
	}

	// 選択中のものの変更が行われているかつ何かを選択していたら処理を行う
	CompPlace(prevPlace);

}

void Upgrade::PadSelect(void)
{
	// 前の状態を保持しておく
	auto prevPlace = place_;

	for (int i = 0; i < static_cast<int>(PLACE::MAX); i++)
	{
		// 全て初期化する
		buttonState_[i] = BUTTON_STATE::DEFAULE;
	}

	switch (place_)
	{
	case Upgrade::PLACE::TOP_LEFT:

		// パッドの十字キーやスティックの下入力を検知したら
		if (InputManager::GetInstance().SelectDown())
		{
			// 選択中のものを変更
			ChangePlace(PLACE::BOTTOM_LEFT);
		}

		// パッドの十字キーやスティックの右入力を検知したら
		if (InputManager::GetInstance().SelectRightIsTrgDown())
		{
			// 選択中のものを変更
			ChangePlace(PLACE::TOP_RIGHT);
		}

		break;
	case Upgrade::PLACE::TOP_RIGHT:

		// パッドの十字キーやスティックの下入力を検知したら
		if (InputManager::GetInstance().SelectDown())
		{
			// 選択中のものを変更
			ChangePlace(PLACE::BOTTOM_RIGHT);
		}

		// パッドの十字キーやスティックの左入力を検知したら
		if (InputManager::GetInstance().SelectLeftIsTrgDown())
		{
			// 選択中のものを変更
			ChangePlace(PLACE::TOP_LEFT);
		}

		break;
	case Upgrade::PLACE::BOTTOM_LEFT:

		// パッドの十字キーやスティックの上入力を検知したら
		if (InputManager::GetInstance().SelectUp())
		{
			// 選択中のものを変更
			ChangePlace(PLACE::TOP_LEFT);
		}

		// パッドの十字キーやスティックの右入力を検知したら
		if (InputManager::GetInstance().SelectRightIsTrgDown())
		{
			// 選択中のものを変更
			ChangePlace(PLACE::BOTTOM_RIGHT);
		}

		break;
	case Upgrade::PLACE::BOTTOM_RIGHT:

		// パッドの十字キーやスティックの上入力を検知したら
		if (InputManager::GetInstance().SelectUp())
		{
			// 選択中のものを変更
			ChangePlace(PLACE::TOP_RIGHT);
		}

		// パッドの十字キーやスティックの左入力を検知したら
		if (InputManager::GetInstance().SelectLeftIsTrgDown())
		{
			// 選択中のものを変更
			ChangePlace(PLACE::BOTTOM_LEFT);
		}

		break;
	case Upgrade::PLACE::MAX:

		// 最初は左上に設定される
		ChangePlace(PLACE::TOP_LEFT);
		break;
	default:
		break;
	}

	// 見た目を選択中にする
	buttonState_[static_cast<int>(place_)] = BUTTON_STATE::HOVER;

	// 選択中のものの変更が行われているかつ何かを選択していたら処理を行う
	CompPlace(prevPlace);

	// 何か選択していて、確定ボタンが押されたら処理を行う
	if (InputManager::GetInstance().Confirm() && place_ != PLACE::MAX)
	{
		finalizeUpgrade_ = selectUpgrades_[static_cast<int>(place_)];

		// 確定
		ChangeState(STATE::APPLY);

		// 決定SEをながす
		SoundManager::GetInstance().Play(SoundManager::SE::DECIDE);

	}
}

void Upgrade::ChangeState(STATE state)
{
	// 指定された状態に変更
	state_ = state;

	// 変更されたら初期化を行う
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
	finalizeUpgrade_ = PLAYER_UPGRADE::MAX;

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

void Upgrade::CompPlace(const PLACE prevPlace)
{
	if (place_ != prevPlace && place_ != PLACE::MAX)
	{
		// 何も選択されていない状態から選択されたらSEを流す
		SoundManager::GetInstance().Play(SoundManager::SE::SELECT);
	}
}

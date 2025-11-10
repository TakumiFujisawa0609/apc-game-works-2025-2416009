#include "../../Manager/InputManager.h"
#include "../../Scene/SceneManager.h"
#include "../../Manager/SoundManager.h"
#include "../../Manager/SystemManager.h"
#include "../../Utility/Collision/CollisionUtility.h"
#include "SelectScene.h"

SelectScene::SelectScene(void)
{
}

SelectScene::~SelectScene(void)
{
}

void SelectScene::Load(void)
{
}

void SelectScene::Init(void)
{
	// 座標初期化
	for (int i = 0; i < static_cast<int>(STATE::MAX); i++)
	{
		pos_[i] = { POS_X + i * SPACE_X, POS_Y };
	}
}

void SelectScene::Update(void)
{
	if (GetJoypadNum() == 0)
	{
		// 引数の座標によって選択中のものを変化させる
		CursorSelect();
	}
	else
	{
		// 選択処理
		PadSelect();
	}

	// 確定処理
	Confirm();
}

void SelectScene::Draw(void)
{

	DrawBox(pos_[static_cast<int>(STATE::STRAIGHT)].x,
		pos_[static_cast<int>(STATE::STRAIGHT)].y,
		pos_[static_cast<int>(STATE::STRAIGHT)].x + COL_SIZE_X,
		pos_[static_cast<int>(STATE::STRAIGHT)].y + COL_SIZE_Y, 0xffffff, true);

	DrawBox(pos_[static_cast<int>(STATE::FOLLOW)].x,
		pos_[static_cast<int>(STATE::FOLLOW)].y,
		pos_[static_cast<int>(STATE::FOLLOW)].x + COL_SIZE_X,
		pos_[static_cast<int>(STATE::FOLLOW)].y + COL_SIZE_Y, 0xffffff, true);

	DrawBox(pos_[static_cast<int>(STATE::EXPLOSION)].x,
		pos_[static_cast<int>(STATE::EXPLOSION)].y,
		pos_[static_cast<int>(STATE::EXPLOSION)].x + COL_SIZE_X,
		pos_[static_cast<int>(STATE::EXPLOSION)].y + COL_SIZE_Y, 0xffffff, true);

	switch (state_)
	{
	case SelectScene::STATE::STRAIGHT:

		DrawBox(pos_[static_cast<int>(STATE::STRAIGHT)].x,
			pos_[static_cast<int>(STATE::STRAIGHT)].y,
			pos_[static_cast<int>(STATE::STRAIGHT)].x + COL_SIZE_X,
			pos_[static_cast<int>(STATE::STRAIGHT)].y + COL_SIZE_Y, 0x00ff00, true);

		break;
	case SelectScene::STATE::FOLLOW:

		DrawBox(pos_[static_cast<int>(STATE::FOLLOW)].x,
			pos_[static_cast<int>(STATE::FOLLOW)].y,
			pos_[static_cast<int>(STATE::FOLLOW)].x + COL_SIZE_X,
			pos_[static_cast<int>(STATE::FOLLOW)].y + COL_SIZE_Y, 0x00ff00, true);

		break;
	case SelectScene::STATE::EXPLOSION:

		DrawBox(pos_[static_cast<int>(STATE::EXPLOSION)].x,
			pos_[static_cast<int>(STATE::EXPLOSION)].y,
			pos_[static_cast<int>(STATE::EXPLOSION)].x + COL_SIZE_X,
			pos_[static_cast<int>(STATE::EXPLOSION)].y + COL_SIZE_Y, 0x00ff00, true);

		break;
	case SelectScene::STATE::MAX:
		break;
	default:
		break;
	}

	DrawString(pos_[static_cast<int>(STATE::STRAIGHT)].x + COL_SIZE_X / 2,
		pos_[static_cast<int>(STATE::STRAIGHT)].y + COL_SIZE_Y / 2, "普通魔法", 0x000000);
	DrawString(pos_[static_cast<int>(STATE::STRAIGHT)].x + 15,
		pos_[static_cast<int>(STATE::STRAIGHT)].y + COL_SIZE_Y/ 2 + 20, "狙ったところへまっすぐ飛びます", 0x000000);
	DrawString(pos_[static_cast<int>(STATE::STRAIGHT)].x + 15,
		pos_[static_cast<int>(STATE::STRAIGHT)].y + COL_SIZE_Y / 2 + 40, "ヘッドダメージを狙いやすいです。", 0x000000);

	DrawString(pos_[static_cast<int>(STATE::FOLLOW)].x + COL_SIZE_X / 2,
		pos_[static_cast<int>(STATE::FOLLOW)].y + COL_SIZE_Y / 2, "追従魔法", 0x000000);
	DrawString(pos_[static_cast<int>(STATE::FOLLOW)].x + 15,
		pos_[static_cast<int>(STATE::FOLLOW)].y + COL_SIZE_Y / 2 + 20, "一番近くの敵へ追従します", 0x000000);
	DrawString(pos_[static_cast<int>(STATE::FOLLOW)].x + 15,
		pos_[static_cast<int>(STATE::FOLLOW)].y + COL_SIZE_Y / 2 + 40, "狙うのが苦手な方におススメ！", 0x000000);

	DrawString(pos_[static_cast<int>(STATE::EXPLOSION)].x + COL_SIZE_X / 2,
		pos_[static_cast<int>(STATE::EXPLOSION)].y + COL_SIZE_Y / 2, "爆発魔法", 0x000000);
	DrawString(pos_[static_cast<int>(STATE::EXPLOSION)].x + 15,
		pos_[static_cast<int>(STATE::EXPLOSION)].y + COL_SIZE_Y / 2 + 20, "着弾したところから範囲攻撃します", 0x000000);
	DrawString(pos_[static_cast<int>(STATE::EXPLOSION)].x + 15,
		pos_[static_cast<int>(STATE::EXPLOSION)].y + COL_SIZE_Y / 2 + 40, "ヘッドダメージはありません", 0x000000);

}

void SelectScene::Release(void)
{
}

void SelectScene::ChangeState(STATE state)
{
	state_ = state;
}

void SelectScene::Confirm(void)
{

	InputManager& ins = InputManager::GetInstance();

	if (ins.Confirm())
	{
		switch (state_)
		{
		case SelectScene::STATE::STRAIGHT:

			SystemManager::GetInstance().SetTypeMagic(TYPE_MAGIC::FIRE_MAGIC);

			break;
		case SelectScene::STATE::FOLLOW:

			SystemManager::GetInstance().SetTypeMagic(TYPE_MAGIC::WIND_MAGIC);

			break;
		case SelectScene::STATE::EXPLOSION:

			SystemManager::GetInstance().SetTypeMagic(TYPE_MAGIC::EXPLOSION_MAGIC);

			break;
		case SelectScene::STATE::MAX:
			break;
		default:
			break;
		}

		// 決定SEをながす
		SoundManager::GetInstance().Play(SoundManager::SE::DECIDE);

		// 何か選択されている状態で決定ボタンを押されたら
		if (state_ != STATE::MAX)
		{
			SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
		}

	}
}

void SelectScene::CursorSelect(void)
{
	SelectScene::STATE prevState = state_;

	for (int i = 0; i < static_cast<int>(STATE::MAX); i++)
	{
		if (CollisionUtility::RectangleAndMouse(pos_[i], COL_SIZE_X, COL_SIZE_Y))
		{
			ChangeState(static_cast<STATE>(i));
			break;
		}
		else
		{
			ChangeState(STATE::MAX);
		}
	}

	if (state_ != prevState && state_ != STATE::MAX)
	{
		// 何も選択されていない状態から選択されたらSEを流す
		SoundManager::GetInstance().Play(SoundManager::SE::SELECT);
	}

}

void SelectScene::PadSelect(void)
{
	auto& ins = InputManager::GetInstance();
	SelectScene::STATE prevState = state_;

	switch (state_)
	{
	case SelectScene::STATE::STRAIGHT:

		if (ins.SelectRightIsTrgDown())
		{
			ChangeState(STATE::FOLLOW);
		}

		break;
	case SelectScene::STATE::FOLLOW:

		if (ins.SelectLeftIsTrgDown())
		{
			ChangeState(STATE::STRAIGHT);
		}

		if (ins.SelectRightIsTrgDown())
		{
			ChangeState(STATE::EXPLOSION);
		}

		break;
	case SelectScene::STATE::EXPLOSION:

		if (ins.SelectLeftIsTrgDown())
		{
			ChangeState(STATE::FOLLOW);
		}

		break;
	case SelectScene::STATE::MAX:

		ChangeState(STATE::FOLLOW);

		break;
	default:
		break;
	}

	if (state_ != prevState && state_ != SelectScene::STATE::MAX)
	{
		// 何も選択されていない状態から選択されたらSEを流す
		SoundManager::GetInstance().Play(SoundManager::SE::SELECT);
	}
}
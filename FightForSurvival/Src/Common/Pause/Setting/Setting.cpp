#include <DxLib.h>
#include "../../../Utility/Collision/CollisionUtility.h"
#include "../../../Manager/InputManager.h"
#include "../../../Manager/SoundManager.h"
#include "../../../Manager/SystemManager.h"
#include "Setting.h"

Setting::Setting(void)
{
}

Setting::~Setting(void)
{
}

void Setting::Load(void)
{
}

void Setting::Init(void)
{
	pos_.x = DONE_POS_X;
	pos_.y = DONE_POS_Y;
	isSetting_ = false;
	isDone_ = false;

	// 感度の初期化
	auto& ins = SystemManager::GetInstance();
	mouseSensitivity_ = ins.GetMouseSensitivity();
	padSensitivity_ = ins.GetPadSensitivity();

	GetMousePoint(&mousePos_.x, &mousePos_.y);
}

void Setting::Update(void)
{
	// 決定
	Confirm();

	// 選択処理
	if (GetJoypadNum() == 0)
	{
		// マウス
		MouseSelect();
	}
	else
	{
		// ゲームパッド
		PadSelect();
	}

	// 感度設定バーの処理
	BarUpdate();
}

void Setting::Draw(void)
{
	// 背景の半透明
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
	DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// 感度の範囲を分かりやすく
	DrawString(HIGH_POS_X, HIGH_POS_Y, "High", 0xffffff);
	DrawString(LOW_POS_X, LOW_POS_Y, "Low", 0xffffff);

	if (isDone_)
	{
		DrawString(DONE_POS_X, DONE_POS_Y, "Done", 0x00ff00);
	}
	else
	{
		DrawString(DONE_POS_X, DONE_POS_Y, "Done", 0xffffff);
	}

	// バーの表示
	DrawBox(BAR_START_POS_X, BAR_START_POS_Y, BAR_END_POS_X, BAR_END_POS_Y, 0xffffff, true);

	if (GetJoypadNum() == 0)
	{
		//DrawCircle();
	}
	else
	{
		if (isDone_)
		{
			DrawCircle(BAR_START_POS_X + static_cast<int>((padSensitivity_ * 10) * (BAR_END_POS_X - BAR_START_POS_X - ((BAR_END_POS_X - BAR_START_POS_X) / 10))), 310, 15, 0xffffff, true);
		}
		else
		{
			DrawCircle(BAR_START_POS_X + static_cast<int>((padSensitivity_ * 10) * (BAR_END_POS_X - BAR_START_POS_X - ((BAR_END_POS_X - BAR_START_POS_X) / 10))), 310, 15, 0xffff00, true);
		}
	}


#ifdef _DEBUG

	DrawBox(DONE_POS_X,
		DONE_POS_Y,
		DONE_POS_X + COL_SIZE_X,
		DONE_POS_Y + COL_SIZE_Y, 0xff0000, false);

#endif // _DEBUG

}

void Setting::Release(void)
{
}

void Setting::Confirm(void)
{
	InputManager& ins = InputManager::GetInstance();

	if (ins.Confirm() && isDone_)
	{
		isSetting_ = false;

		// 決定SEをながす
		SoundManager::GetInstance().Play(SoundManager::SE::DECIDE);
	}
}

void Setting::MouseSelect(void)
{
	auto prevDone = isDone_;

	if (CollisionUtility::RectangleAndMouse(pos_, COL_SIZE_X, COL_SIZE_Y))
	{
		isDone_ = true;
	}
	else
	{
		isDone_ = false;
	}

	if (isDone_ != prevDone)
	{
		// 何も選択されていない状態から選択されたらSEを流す
		SoundManager::GetInstance().Play(SoundManager::SE::SELECT);
	}
}

void Setting::PadSelect(void)
{
	auto& ins = InputManager::GetInstance();
	auto prevDone = isDone_;

	if (ins.SelectUp())
	{
		isDone_ = false;
	}

	if (ins.SelectDown())
	{
		isDone_ = true;
	}

	if (isDone_ != prevDone)
	{
		// 何も選択されていない状態から選択されたらSEを流す
		SoundManager::GetInstance().Play(SoundManager::SE::SELECT);
	}
}

void Setting::BarUpdate(void)
{
	// 何も選択されていない状態であれば、感度設定のバーを触っている状態なため処理を行う
	if (!isDone_)
	{
		// 感度設定処理
		if (GetJoypadNum() == 0)
		{
			// マウス
			MouseBarUpdate();
		}
		else
		{
			// ゲームパッド
			PadBarUpdate();
		}
	}
}

void Setting::MouseBarUpdate(void)
{
	auto& sysIns = SystemManager::GetInstance();
	mouseSensitivity_ = sysIns.GetMouseSensitivity();
	float prevSensitivity = mouseSensitivity_;


	if (prevSensitivity != mouseSensitivity_)
	{
		// 変更が行われていたら処理を行う
		sysIns.SetMouseSensitivity(mouseSensitivity_);
	}
}

void Setting::PadBarUpdate(void)
{
	auto& sysIns = SystemManager::GetInstance();
	padSensitivity_ = sysIns.GetPadSensitivity();
	float prevSensitivity = padSensitivity_;

	auto& ins = InputManager::GetInstance();

	if (ins.SelectRight())
	{
		// 感度を高くする
		padSensitivity_+= SENSITIVITY_PAD;

		// 最大値を超えないようにする
		if (padSensitivity_ > SENSITIVITY_MAX_PAD)
		{
			padSensitivity_ = SENSITIVITY_MAX_PAD;
		}
	}
	else if (ins.SelectLeft())
	{
		// 感度を低くする
		padSensitivity_ -= SENSITIVITY_PAD;

		// 最小値を超えないようにする
		if (padSensitivity_ < SENSITIVITY_MIN_PAD)
		{
			padSensitivity_ = SENSITIVITY_MIN_PAD;
		}
	}

	if (prevSensitivity != padSensitivity_)
	{
		// 変更が行われていたら処理を行う
		sysIns.SetPadSensitivity(padSensitivity_);
	}
}

#include <DxLib.h>
#include "../../../Utility/Collision/CollisionUtility.h"
#include "../../../Manager/InputManager.h"
#include "../../../Manager/SoundManager.h"
#include "../../../Manager/SystemManager.h"
#include "Setting.h"

#include "../../../UI/UIManager.h"
#include "../../../UI/TextrueManager/TextureManager.h"
#include "../../../UI/Object/Button/SettingButton/SettingButtonDone.h"

Setting::Setting(void)
{
}

Setting::~Setting(void)
{
}

void Setting::Load(void)
{
	// UI管理の生成処理
	uiMgr = new UIManager();
	texMgr = new TextureManager();

	// UIを生成
	UIBase* text = UIFactory::GetInstance()->CreateUI(UI_KIND::SETTING_TEXT, texMgr);
	UIBase* done = UIFactory::GetInstance()->CreateUI(UI_KIND::SETTING_DONE, texMgr);
	UIBase* bar = UIFactory::GetInstance()->CreateUI(UI_KIND::SETTING_BAR, texMgr);

	SettingButtonDone* settingButton = dynamic_cast<SettingButtonDone*>(done);
	settingButton->SetIsDone(&isDone_);

	// 生成したUIを追加
	uiMgr->AddUI(text);
	uiMgr->AddUI(done);
	uiMgr->AddUI(bar);
}

void Setting::Init(void)
{
	pos_.x = DONE_POS_X;
	pos_.y = DONE_POS_Y;
	isSetting_ = false;
	isDone_ = false;
	isDrag_ = false;

	// 感度の初期化
	auto& ins = SystemManager::GetInstance();
	mouseSensitivity_ = ins.GetMouseSensitivity();
	padSensitivity_ = ins.GetPadSensitivity();

	GetMousePoint(&mousePos_.x, &mousePos_.y);
	// 選択処理
	if (SystemManager::GetInstance().GetIsDevice())
	{
		circlePos_ = { static_cast<int>(((mouseSensitivity_ - SENSITIVITY_MIN_MOUSE) / (SENSITIVITY_MAX_MOUSE - SENSITIVITY_MIN_MOUSE)) + BAR_START_POS_X)
			,CIRCLE_POS_Y };
	}
	else
	{
		circlePos_ = {(BAR_START_POS_X - ((BAR_END_POS_X - BAR_START_POS_X) / 9)) + static_cast<int>((padSensitivity_ * 10) * ((BAR_END_POS_X - BAR_START_POS_X) + ((BAR_END_POS_X - BAR_START_POS_X) / 9)))
			, CIRCLE_POS_Y };
	}
}

void Setting::Update(void)
{
	// 決定
	Confirm();

	// 選択処理
	if (SystemManager::GetInstance().GetIsDevice())
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

	// UIの更新
	uiMgr->Update();
}

void Setting::Draw(void)
{
	// 背景の半透明
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
	DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// UIの描画
	uiMgr->Draw();

	if (SystemManager::GetInstance().GetIsDevice())
	{

		if(isDrag_)
		{
			DrawCircle(circlePos_.x, circlePos_.y, CIRCLE_RAD, 0x3b3b3b, true);
		}
		else
		{
			DrawCircle(circlePos_.x, circlePos_.y, CIRCLE_RAD, 0x7c7c7c, true);

		}
	}
	else
	{
		if (isDone_)
		{
			DrawCircle((BAR_START_POS_X - ((BAR_END_POS_X - BAR_START_POS_X) / 9)) + 
				static_cast<int>((padSensitivity_ * 10) * ((BAR_END_POS_X - BAR_START_POS_X) + ((BAR_END_POS_X - BAR_START_POS_X) / 9))), CIRCLE_POS_Y, CIRCLE_RAD, 0x7c7c7c, true);
		}
		else
		{
			DrawCircle((BAR_START_POS_X - ((BAR_END_POS_X - BAR_START_POS_X) / 9)) + 
				static_cast<int>((padSensitivity_ * 10) * ((BAR_END_POS_X - BAR_START_POS_X) + ((BAR_END_POS_X - BAR_START_POS_X) / 9))), CIRCLE_POS_Y, CIRCLE_RAD, 0x3b3b3b, true);
		}
	}


}

void Setting::Release(void)
{
	// UIの解放
	delete uiMgr;
	delete texMgr;
}

void Setting::SetIsSetting(bool flg)
{
	isSetting_ = flg;

	if (flg)
	{
		isDrag_ = false;
	}
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
		if (SystemManager::GetInstance().GetIsDevice())
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
	auto& ins = InputManager::GetInstance();

	// マウス感度の情報を取得
	auto& sysIns = SystemManager::GetInstance();
	mouseSensitivity_ = sysIns.GetMouseSensitivity();
	float prevSensitivity = mouseSensitivity_;

	// マウスの位置を取得
	GetMousePoint(&mousePos_.x, &mousePos_.y);

	// つまみの範囲に入っており、左クリックを押したら
	if (CollisionUtility::CircleAndMouse(circlePos_, CIRCLE_RAD) && ins.IsClickMouseLeft() && !isDrag_)
	{
		// つまみを掴んでいる状態にする
		isDrag_ = true;
	}
	else if (BAR_START_POS_Y < mousePos_.y && mousePos_.y < BAR_END_POS_Y && ins.IsClickMouseLeft() && !isDrag_)
	{
		isDrag_ = true;
	}

	// つまみを掴んでいる状態であれば
	if (isDrag_)
	{
		circlePos_.x = mousePos_.x;

		// 最小値を超えないようにする
		if (circlePos_.x < BAR_START_POS_X)
		{
			circlePos_.x = BAR_START_POS_X;
		}

		// 最大値を超えないようにする
		if (circlePos_.x > BAR_END_POS_X)
		{
			circlePos_.x = BAR_END_POS_X;
		}

		// バーの現在の値
		auto range = circlePos_.x - BAR_START_POS_X;

		// バーの範囲
		auto barRange = BAR_END_POS_X - BAR_START_POS_X;

		// 感度の範囲
		auto sensiRange = SENSITIVITY_MAX_MOUSE - SENSITIVITY_MIN_MOUSE;

		// 現在のバーに対しての割合を計算
		float ratio = (float)range / barRange;

		// 感度の範囲に割合をかけて、感度を求める(感度が0ならないように、最小値から足し算)
		mouseSensitivity_ = SENSITIVITY_MIN_MOUSE + (sensiRange * ratio);
	}

	if (prevSensitivity != mouseSensitivity_)
	{
		// 変更が行われていたら処理を行う
		sysIns.SetMouseSensitivity(mouseSensitivity_);
	}

	if (isDrag_ && !ins.IsClickMouseLeft())
	{
		isDrag_ = false;
	}

}

void Setting::PadBarUpdate(void)
{
	// ゲームパッド感度の情報を取得
	auto& sysIns = SystemManager::GetInstance();
	padSensitivity_ = sysIns.GetPadSensitivity();
	float prevSensitivity = padSensitivity_;

	auto& ins = InputManager::GetInstance();

	if (ins.SelectRightIsTrgDown() || ins.SelectRightIsNew())
	{
		// 感度を高くする
		padSensitivity_+= SENSITIVITY_PAD;

		// 最大値を超えないようにする
		if (padSensitivity_ > SENSITIVITY_MAX_PAD)
		{
			padSensitivity_ = SENSITIVITY_MAX_PAD;
		}
	}
	else if (ins.SelectLeftIsTrgDown() || ins.SelectLeftIsNew())
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

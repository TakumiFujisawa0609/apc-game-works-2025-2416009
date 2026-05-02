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
	:uiMgr(nullptr)
	,texMgr(nullptr)
	,pos_(0,0)
	,mousePos_(0,0)
	,circlePos_(0,0)
	,isSetting_(false)
	,isDone_(false)
	,isDrag_(false)
	,mouseSensitivity_(0.0f)
	,padSensitivity_(0.0f)
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
	// 座標の初期化
	pos_.x = DONE_POS_X;
	pos_.y = DONE_POS_Y;

	// マウス座標の初期化
	GetMousePoint(&mousePos_.x, &mousePos_.y);

	// 使用しているデバイスによって座標を変更
	if (SystemManager::GetInstance().GetIsDevice())
	{
		// マウスの時のつまみの座標
		circlePos_ = 
		{ 
			static_cast<int>(((BAR_DISTANCE_X / SENSITIVITY_RANGE_MOUSE * (mouseSensitivity_ - SENSITIVITY_MIN_MOUSE))) + BAR_START_POS_X)
			,CIRCLE_POS_Y 
		};
	}
	else
	{
		// パッドの時のつまみの座標
		circlePos_ =
		{
			(BAR_START_POS_X - BAR_DISTANCE_OFFSET) + static_cast<int>((padSensitivity_ * SENSITIVITY_ADJUST_PAD) * (BAR_DISTANCE_X + BAR_DISTANCE_OFFSET))
			, CIRCLE_POS_Y 
		};
	}

	isSetting_ = false;
	isDone_ = false;
	isDrag_ = false;

	// 感度の初期化
	mouseSensitivity_ = SystemManager::GetInstance().GetMouseSensitivity();
	padSensitivity_ = SystemManager::GetInstance().GetPadSensitivity();
}

void Setting::Update(void)
{
	// 決定
	Confirm();

	// 使用しているデバイスを確認
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

	// 使用しているデバイスを確認
	if (SystemManager::GetInstance().GetIsDevice())
	{
		// マウスの時の処理
		// つまみを掴んでいるか
		if(isDrag_)
		{
			// 掴んでいる
			DrawCircle
			(
				circlePos_.x
				, circlePos_.y, CIRCLE_RAD, SELECTED_COLOR, true
			);
		}
		else
		{
			// 掴んでいない
			DrawCircle
			(
				circlePos_.x
				, circlePos_.y, CIRCLE_RAD, UNSELECTED_COLOR, true
			);

		}
	}
	else
	{
		// パッドの時の処理
		if (isDone_)
		{
			// 「Done」を選択していたらつまみを掴んでいないことにする
			DrawCircle
			(
				(BAR_START_POS_X - BAR_DISTANCE_OFFSET) + static_cast<int>((padSensitivity_ * SENSITIVITY_ADJUST_PAD) * (BAR_DISTANCE_X + BAR_DISTANCE_OFFSET))
				, CIRCLE_POS_Y, CIRCLE_RAD, UNSELECTED_COLOR, true
			);
		}
		else
		{
			// 「Done」を選択していなかったらつまみを掴んでいることにする
			DrawCircle
			(
				(BAR_START_POS_X - BAR_DISTANCE_OFFSET) + static_cast<int>((padSensitivity_ * SENSITIVITY_ADJUST_PAD) * (BAR_DISTANCE_X + BAR_DISTANCE_OFFSET))
				, CIRCLE_POS_Y, CIRCLE_RAD, SELECTED_COLOR, true
			);
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
	// 感度設定中かを受け取る
	isSetting_ = flg;

	// 感度設定中だったらつまみを掴んでいるかを初期化しておく
	if (flg)
	{
		isDrag_ = false;
	}
}

void Setting::Confirm(void)
{
	// 「Done」を選択中かつ決定ボタンを押されたら
	if (InputManager::GetInstance().Confirm() && isDone_)
	{
		// 感度設定を終了する
		isSetting_ = false;

		// 決定SEをながす
		SoundManager::GetInstance().Play(SoundManager::SE::DECIDE);
	}
}

void Setting::MouseSelect(void)
{
	// 前の状態を保持しておく
	bool prevDone = isDone_;

	// 「Done」とマウスの当たり判定行う
	if (CollisionUtility::RectangleAndMouse(pos_, COL_SIZE_X, COL_SIZE_Y))
	{
		// 当たっていたらtrueを設定
		isDone_ = true;
	}
	else
	{
		// 当たっていたらfalseを設定
		isDone_ = false;
	}

	// 前の状態と比較して変更があったら処理を行う
	CompDone(prevDone);
}

void Setting::PadSelect(void)
{
	// 前の状態を保持しておく
	bool prevDone = isDone_;

	// 上選択ボタンを検知したら
	if (InputManager::GetInstance().SelectUp())
	{
		// つまみを掴んでいないことにする
		isDone_ = false;
	}

	// 下選択ボタンを検知したら
	if (InputManager::GetInstance().SelectDown())
	{
		// つまみを掴んでいることにする
		isDone_ = true;
	}

	// 前の状態と比較して変更があったら処理を行う
	CompDone(prevDone);
}

void Setting::BarUpdate(void)
{
	// 何も選択されていない状態であれば、感度設定のバーを触っている状態なため処理を行う
	if (!isDone_)
	{
		// 感度設定処理
		// 使用しているデバイスを確認
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
	// マウス感度の情報を取得
	mouseSensitivity_ = SystemManager::GetInstance().GetMouseSensitivity();

	// 前の感度を保持しておく
	float prevSensitivity = mouseSensitivity_;

	// マウスの位置を取得
	GetMousePoint(&mousePos_.x, &mousePos_.y);

	// つまみの範囲に入っており、左クリックが押されたら
	if (CollisionUtility::CircleAndMouse(circlePos_, CIRCLE_RAD) && InputManager::GetInstance().IsClickMouseLeft() && !isDrag_)
	{
		// つまみを掴んでいる状態にする
		isDrag_ = true;
	}
	// バーの縦軸内かつ、左クリックが押されたら
	else if (BAR_START_POS_Y < mousePos_.y && mousePos_.y < BAR_END_POS_Y && InputManager::GetInstance().IsClickMouseLeft() && !isDrag_)
	{
		// つまみを掴んでいる状態にする
		isDrag_ = true;
	}

	// つまみを掴んでいる状態であれば
	if (isDrag_)
	{
		// つまみの座標にマウスの座標を設定
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
		int range = circlePos_.x - BAR_START_POS_X;

		// 現在のバーに対しての割合を計算
		float ratio = (float)range / BAR_DISTANCE_X;

		// 感度の範囲に割合をかけて、感度を求める(感度が0ならないように、最小値から足し算)
		mouseSensitivity_ = SENSITIVITY_MIN_MOUSE + (SENSITIVITY_RANGE_MOUSE * ratio);
	}

	// 前の感度と比較して変更があったら
	if (prevSensitivity != mouseSensitivity_)
	{
		// マウスの感度をSystemManagerに保存
		SystemManager::GetInstance().SetMouseSensitivity(mouseSensitivity_);
	}

	// つまみを掴んでいる状態かつ、左クリックが押されていなかったら
	if (isDrag_ && !InputManager::GetInstance().IsClickMouseLeft())
	{
		// つまみを掴んでいない状態にする
		isDrag_ = false;
	}

}

void Setting::PadBarUpdate(void)
{
	// ゲームパッド感度の情報を取得
	padSensitivity_ = SystemManager::GetInstance().GetPadSensitivity();

	// 前の感度を保持しておく
	float prevSensitivity = padSensitivity_;

	// パッドの十字キーやスティックの右入力を検知したら
	if (InputManager::GetInstance().SelectRightIsTrgDown() || InputManager::GetInstance().SelectRightIsNew())
	{
		// 感度を高くする
		padSensitivity_+= SENSITIVITY_PAD;

		// 最大値を超えないようにする
		if (padSensitivity_ > SENSITIVITY_MAX_PAD)
		{
			padSensitivity_ = SENSITIVITY_MAX_PAD;
		}
	}
	// パッドの十字キーやスティックの左入力を検知したら
	else if (InputManager::GetInstance().SelectLeftIsTrgDown() || InputManager::GetInstance().SelectLeftIsNew())
	{
		// 感度を低くする
		padSensitivity_ -= SENSITIVITY_PAD;

		// 最小値を超えないようにする
		if (padSensitivity_ < SENSITIVITY_MIN_PAD)
		{
			padSensitivity_ = SENSITIVITY_MIN_PAD;
		}
	}

	// 前の感度と比較して変更があったら
	if (prevSensitivity != padSensitivity_)
	{
		// パッドの感度をSystemManagerに保存
		SystemManager::GetInstance().SetPadSensitivity(padSensitivity_);
	}
}

void Setting::CompDone(const bool prevDone)
{
	// 何も選択されていない状態から選択されたら
	if (isDone_ != prevDone)
	{
		// SEを流す
		SoundManager::GetInstance().Play(SoundManager::SE::SELECT);
	}
}

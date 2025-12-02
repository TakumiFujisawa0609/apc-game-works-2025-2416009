#include "PauseButton.h"
#include "../../../../Manager/SystemManager.h"

PauseButton::PauseButton(const UIResourceInfo& info, TextureManager* texMgr)
	:Button(info,texMgr)
{
}

PauseButton::~PauseButton()
{
}

void PauseButton::Update(void)
{
	if (SystemManager::GetInstance().GetIsDevice())
	{
		// マウスの選択処理
		Button::Update();

		return;
	}

	// ゲームパッドの選択処理
	// デフォルトに初期化
	buttonState_ = DEFAULE;

	switch (*pause_)
	{
	case Pause::PAUSE::CONTINUE:

		if (uiKind_ == UI_KIND::PAUSE_CONTINUE)
		{
			// クリックされている
			buttonState_ = HOVER;
		}

		break;
	case Pause::PAUSE::SETTING:

		if (uiKind_ == UI_KIND::PAUSE_SETTING)
		{
			// クリックされている
			buttonState_ = HOVER;
		}

		break;
	case Pause::PAUSE::TITLE:

		if (uiKind_ == UI_KIND::PAUSE_TITLE)
		{
			// クリックされている
			buttonState_ = HOVER;
		}

		break;
	case Pause::PAUSE::NON:
		break;
	default:
		break;
	}

}

void PauseButton::Draw(void)
{
	Button::Draw();
}

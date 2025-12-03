#include "SettingButtonDone.h"
#include "../../../../Manager/SystemManager.h"  
#include "../../../../Manager/InputManager.h"

SettingButtonDone::SettingButtonDone(const UIResourceInfo& info, TextureManager* texMgr)
	: Button(info, texMgr)
{
}

SettingButtonDone::~SettingButtonDone()
{
}

void SettingButtonDone::Update(void)
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

    if (InputManager::GetInstance().ConfirmUp() && *isDone_)
    {
        // クリックされた  
        buttonState_ = TRIGGER_DOWN;
    }
    else if(*isDone_)
    {
        // 選択中
        buttonState_ = HOVER;
    }
}

void SettingButtonDone::Draw(void)
{
    Button::Draw();
}

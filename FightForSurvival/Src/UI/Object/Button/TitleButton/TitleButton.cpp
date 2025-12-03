#include "TitleButton.h"  
#include "../../../../Manager/SystemManager.h"  
#include "../../../../Manager/InputManager.h"  

TitleButton::TitleButton(const UIResourceInfo& info, TextureManager* texMgr)  
: Button(info, texMgr)
{  
}  

TitleButton::~TitleButton()  
{  
}  

void TitleButton::Update(void)  
{  
    if (SystemManager::GetInstance().GetIsDevice())
    {
        // マウスの選択処理  
			// マウスの座標取得用変数
		int mousePosX = 0;
		int mousePosY = 0;

		// マウス座標を取得
		GetMousePoint(&mousePosX, &mousePosY);

		// ボタンの上にマウスポインタがあるか？
		if (x_< mousePosX &&
			x_ + w_ > mousePosX &&
			y_< mousePosY &&
			y_ + h_ > mousePosY && 
			*state_ != TitleScene::CLICK)
		{

			// クリックされているか？
			if (InputManager::GetInstance().Confirm() && !isTrgDown_)
			{
				isTrgDown_ = true;
			}
			else
			{
				isTrgDown_ = false;
			}

			if (isTrgDown_)
			{
				buttonState_ = TRIGGER_DOWN;
			}
			else
			{
				buttonState_ = HOVER;
			}

		}
		else
		{
			buttonState_ = DEFAULE;
			isTrgDown_ = false;
		}

        return;
    }

   // ゲームパッドの選択処理  
   // デフォルトに初期化  
   buttonState_ = DEFAULE;  

   switch (*state_)  
   {  
   case TitleScene::CLICK:  
       break;  
   case TitleScene::GAMESTART:  

       if (uiKind_ == UI_KIND::GAME_START)  
       {  
           // クリックされている  
           buttonState_ = HOVER;  
       }  

       break;  
   case TitleScene::END:  

       if (uiKind_ == UI_KIND::END)  
       {  
           // クリックされている  
           buttonState_ = HOVER;
       }  

       break;  
   case TitleScene::NON:  
       break;  
   default:  
       break;  
   }  
}  

void TitleButton::Draw(void)  
{  
   Button::Draw();  
}

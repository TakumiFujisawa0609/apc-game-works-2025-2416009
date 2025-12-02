#include "TitleButton.h"  
#include "../../../../Manager/SystemManager.h"  

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
       Button::Update();  
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

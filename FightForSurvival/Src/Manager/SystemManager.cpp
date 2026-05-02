#include "InputManager.h"
#include "SystemManager.h"

SystemManager* SystemManager::instance_ = nullptr;

void SystemManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new SystemManager();
	}
}

SystemManager& SystemManager::GetInstance(void)
{
    return *instance_;
}

void SystemManager::Update(void)
{
	// マウス選択中
	if (isDeviceMouse_)
	{
		// パッドのボタンを触ったらパッドを使用すると判断
		if (InputManager::GetInstance().ChangeDevicePad())
		{
			// 特定のキーを押されたら(A,B,X,Y,Lスティックのどれか)パッドモードにする
			isDeviceMouse_ = false;
		}
	}
	// パッド選択中
	else
	{
		// マウス・キーボードを触ったらキーマウを使用すると判断
		if (InputManager::GetInstance().ChangeDeviceMouse())
		{
			// 特定のキーを押されたら(マウス左クリック，Q,E,W,A,S,Dのどれか)マウスモードにすす
			isDeviceMouse_ = true;
		}
	}
}

void SystemManager::Destroy()
{
	delete instance_;
	instance_ = nullptr;
}

void SystemManager::ResetGame()
{
	// スコア初期化
	score_ = 0; 

	// 最初の魔法の種類を初期化
	typeMagic_ = TYPE_MAGIC::STRAIGHT_MAGIC;
}

SystemManager::SystemManager(void)
{
	// 感度の初期化
	MouseSensitivity_ = MOUSE_SENSITIVITY;
	PadSensitivity_ = PAD_SENSITIVITY;

	// スコア初期化
	score_ = 0;

	// 最初の魔法の種類を初期化
	typeMagic_ = TYPE_MAGIC::STRAIGHT_MAGIC;

	// パッドの接続数でパッドを使用するかキーマウを使用するか判断
	if (GetJoypadNum() == 0)
	{
		// キーマウを使用
		isDeviceMouse_ = true;
	}
	else
	{
		// パッドを使用
		isDeviceMouse_ = false;
	}
}

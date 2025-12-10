#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "Manager/InputManager.h"
#include "Scene/SceneManager.h"
#include "Manager/SoundManager.h"
#include "Manager/EndManager.h"
#include "Common/Fps/FpsControl.h"
#include "Application.h"

Application* Application::instance_ = nullptr;

const std::string Application::PATH_DATA = "Data/";
const std::string Application::PATH_IMAGE = PATH_DATA + "Image/";
const std::string Application::PATH_MODEL = PATH_DATA + "Model/";
const std::string Application::PATH_SOUND = PATH_DATA + "Sound/";
const std::string Application::PATH_PLAYER = PATH_MODEL + "Player/";
const std::string Application::PATH_EFFECT = PATH_DATA + "Effect/";
const std::string Application::PATH_BGM = PATH_SOUND + "BGM/";
const std::string Application::PATH_SE = PATH_SOUND + "SE/";
const std::string Application::PATH_MAP_DATA = PATH_DATA + "MapData/MapData.csv";

void Application::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new Application();
	}
	instance_->Init();
}

Application& Application::GetInstance(void)
{
	return *instance_;
}

void Application::Init(void)
{

	// アプリケーションの初期設定
	SetWindowText("Fight_For_Survival");

	// ウィンドウサイズ
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 32);

#ifdef _DEBUG
	ChangeWindowMode(true);
#else
	// フルスクリーンにする
	ChangeWindowMode(false);
#endif // _DEBUG

	// DxLibの初期化
	SetUseDirect3DVersion(DX_DIRECT3D_11);
	isInitFail_ = false;
	if (DxLib_Init() == -1)
	{
		isInitFail_ = true;
		return;
	}

	// Effekseerの初期化
	if (InitEffekseer() == -1)
	{
		isInitFail_ = true;
		return;
	}

	// 乱数のシード値を設定する
	DATEDATA date;

	// 現在時刻を取得する
	GetDateTime(&date);

	// 乱数の初期値を設定する
	// 設定する数値によって、ランダムの出方が変わる
	SRand(date.Year + date.Mon + date.Day + date.Hour + date.Min + date.Sec);

	//FPS初期化
	fps_ = new FpsControl;
	fps_->Init();

	// 入力制御初期化
	SetUseDirectInputFlag(true);
	InputManager::CreateInstance();

	// サウンド管理初期化
	SoundManager::CreateInstance();
	SoundManager::GetInstance().Load();

	// シーン管理初期化
	SceneManager::CreateInstance();

	// 終了管理初期化
	EndManager::CreateInstance();
}

void Application::Run(void)
{
	InputManager& inputManager = InputManager::GetInstance();
	SceneManager& sceneManager = SceneManager::GetInstance();
	EndManager& endManager = EndManager::GetInstance();

	// ゲームループ
	while (ProcessMessage() == 0 && !endManager.GetIsEnd())
	{

		//フレームレート更新
		if (!fps_->UpdateFrameRate()) continue;

		inputManager.Update();
		sceneManager.Update();

		sceneManager.Draw();

		fps_->CalcFrameRate();
		fps_->DrawFrameRate();

		ScreenFlip();

	}

}

void Application::Destroy(void)
{

	// Effekseerを終了する
	Effkseer_End();

	// 終了管理解放
	EndManager::GetInstance().Destroy();

	// シーン管理解放
	SceneManager::GetInstance().Destroy();

	// 入力制御解放
	InputManager::GetInstance().Destroy();

	//フレームレート解放
	delete fps_;

	// DxLib終了
	if (DxLib_End() == -1)
	{
		isReleaseFail_ = true;
	}

	// インスタンスのメモリ解放
	delete instance_;

}

bool Application::IsInitFail(void) const
{
	return isInitFail_;
}

bool Application::IsReleaseFail(void) const
{
	return isReleaseFail_;
}

Application::Application(void)
{
	isInitFail_ = false;
	isReleaseFail_ = false;
}

int Application::InitEffekseer(void)
{
	if (Effekseer_Init(8000) == -1)
	{
		DxLib_End();

		return -1;
	}

	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();
	return 1;
}

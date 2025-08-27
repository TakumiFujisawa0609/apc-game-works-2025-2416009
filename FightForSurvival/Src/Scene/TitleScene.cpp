#include <DxLib.h>
#include "../Application.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/Camera.h"
#include "TitleScene.h"

TitleScene::TitleScene(void)
{
	camera_ = nullptr;
}

TitleScene::~TitleScene(void)
{
}

void TitleScene::Load(void)
{
}

void TitleScene::Init(void)
{
	// カメラの初期化
	camera_ = new Camera();
	camera_->Init();
}

void TitleScene::Update(void)
{

	if (InputManager::GetInstance().PushStartKey())
	{
		// スタートキーが押されたらゲームシーンへ移る
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
	}

	// カメラの更新
	camera_->Update();
}

void TitleScene::Draw(void)
{
	// カメラの設定
	camera_->SetBeforeDraw();

#ifdef _DEBUG
	DrawString(0, 0, "Title", 0xffffff);

	// カメラのデバック描画
	camera_->DrawDebug();
#endif // _DEBUG

}

void TitleScene::Release(void)
{
	if (camera_ != nullptr)
	{
		// カメラの解放
		camera_->Release();
		delete camera_;
	}


}

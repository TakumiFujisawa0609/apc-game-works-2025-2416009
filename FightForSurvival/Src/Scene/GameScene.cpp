#include <DxLib.h>
#include "../Object/Grid/Grid.h"
#include "../Manager/Camera.h"
#include "../Object/Player/Player.h"
#include "GameScene.h"

GameScene::GameScene(void)
{
	grid_ = nullptr;
	player_ = nullptr;
	camera_ = nullptr;
}

GameScene::~GameScene(void)
{
}

void GameScene::Load(void)
{
}

void GameScene::Init(void)
{

	// グリッド生成
	grid_ = new Grid();
	grid_->Init();

	// プレイヤー生成
	player_ = new Player();
	player_->Init();

	// カメラの生成
	camera_ = new Camera(dynamic_cast<Player*>(player_));
	camera_->Init();

}

void GameScene::Update(void)
{
	// グリッド更新
	grid_->Update();

	// プレイヤー更新
	player_->Update();

	// カメラの更新
	camera_->Update();
}

void GameScene::Draw(void)
{
	// カメラの設定
	camera_->SetBeforeDraw();

	// グリッド描画
	grid_->Draw();

	// プレイヤー更新
	player_->Draw();

#ifdef _DEBUG
	DrawString(0, 0, "GameScene", 0xffffff);

	// カメラのデバック描画
	camera_->DrawDebug();
#endif // _DEBUG

}

void GameScene::Release(void)
{

	// カメラの解放
	if (camera_ != nullptr)
	{
		camera_->Release();
		delete camera_;
	}

	// プレイヤーの解放
	if (player_ != nullptr)
	{
		player_->Release();
		delete player_;
		player_ = nullptr;
	}

	// グリッドの解放
	if (grid_ != nullptr)
	{
		grid_->Release();
		delete grid_;
		grid_ = nullptr;
	}

}

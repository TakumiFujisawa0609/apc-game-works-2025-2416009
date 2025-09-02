#include <DxLib.h>
#include "../Object/Grid/Grid.h"
#include "../Application.h"
#include "../Manager/Camera.h"
#include "../Manager/InputManager.h"
#include "../Object/Player/Player.h"
#include "../Object/Common/Cursor.h"
#include "../Object/Enemy/Zombie.h"
#include "GameScene.h"

GameScene::GameScene(void)
{
	grid_ = nullptr;
	player_ = nullptr;
	camera_ = nullptr;
	cursor_ = nullptr;
	enemy_ = nullptr;

	enemyId_ = -1;

	// マウスカーソルを表示しない
	SetMouseDispFlag(false);
}

GameScene::~GameScene(void)
{
}

void GameScene::Load(void)
{
	player_->Load();
	cursor_->Load();
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
	camera_ = new Camera(player_);
	camera_->Init();

	// カーソルの生成
	cursor_ = new Cursor();
	cursor_->Init();

	// 敵の生成
	enemyId_ = MV1LoadModel((Application::PATH_MODEL + "Enemy/Zombie.mv1").c_str());
	enemy_ = new Zombie();
	enemy_->Init(EnemyBase::ENEMY_TYPE::ZOMBIE, enemyId_, -1, VGet(0.0f, 0.0f, 100.0f), player_);
}

void GameScene::Update(void)
{
	// グリッド更新
	grid_->Update();

	// プレイヤー更新
	player_->Update();

	// カメラの更新
	camera_->Update();

	// 敵の更新
	enemy_->Update();
}

void GameScene::Draw(void)
{
	// カメラの設定
	camera_->SetBeforeDraw();

	// グリッド描画
	grid_->Draw();

	// プレイヤー描画
	player_->Draw();

	// 敵の描画
	enemy_->Draw();

#ifdef _DEBUG
	DrawString(0, 0, "GameScene", 0xffffff);

	// カメラのデバック描画
	camera_->DrawDebug();
#endif // _DEBUG

	// カーソルの描画
	cursor_->Draw();

}

void GameScene::Release(void)
{

	// 敵の解放
	if (enemy_ != nullptr)
	{
		enemy_->Release();
		delete enemy_;
	}
	MV1DeleteModel(enemyId_);

	// カーソルの解放
	if (cursor_ != nullptr)
	{
		cursor_->Release();
		delete cursor_;
	}

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

	// マウスカーソルを表示させる
	SetMouseDispFlag(true);
}

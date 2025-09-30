#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/SystemManager.h"
#include "../Application.h"
#include "OverScene.h"

OverScene::OverScene(void)
{
}

OverScene::~OverScene(void)
{
}

void OverScene::Init(void)
{
	score_ = SystemManager::GetInstance().GetScore();
}

void OverScene::Load(void)
{
}

void OverScene::Update(void)
{
	if (InputManager::GetInstance().PushStartKey())
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
	}
}

void OverScene::Draw(void)
{
	int POS_X = Application::SCREEN_SIZE_X / 2;
	int POS_Y = Application::SCREEN_SIZE_Y / 2;
	DrawString(POS_X - 50, POS_Y - 50, "最終スコア", 0xffffff);
	DrawFormatString(POS_X, POS_Y, 0xffffff, "%d", score_);

	DrawString(POS_X - 50, POS_Y - 80, "GameOver...", 0xffffff);
}

void OverScene::Release(void)
{
}

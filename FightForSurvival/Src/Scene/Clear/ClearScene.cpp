#include "../../Manager/InputManager.h"
#include "../../Scene/SceneManager.h"
#include "../../Manager/SystemManager.h"
#include "../../Manager/SoundManager.h"
#include "../../Application.h"
#include "ClearScene.h"

ClearScene::ClearScene(void)
{
}

ClearScene::~ClearScene(void)
{
}

void ClearScene::Load(void)
{
}

void ClearScene::Init(void)
{
	score_ = SystemManager::GetInstance().GetScore();

	// BGMをかける
	SoundManager::GetInstance().Play(SoundManager::BGM::CLEAR);
}

void ClearScene::Update(void)
{
	if (InputManager::GetInstance().PushStartKey())
	{
		// タイトルに遷移させる
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
		// 決定SE流す
		SoundManager::GetInstance().Play(SoundManager::SE::DECIDE);
	}
}

void ClearScene::Draw(void)
{
	int POS_X = Application::SCREEN_SIZE_X / 2;
	int POS_Y = Application::SCREEN_SIZE_Y / 2;
	DrawString(POS_X - 50, POS_Y - 50, "最終スコア", 0xffffff);
	DrawFormatString(POS_X, POS_Y, 0xffffff, "%d", score_);

	DrawString(POS_X - 50,POS_Y - 80, "GameClear!", 0xffffff);
}

void ClearScene::Release(void)
{
	// BGMを止める
	SoundManager::GetInstance().Stop(SoundManager::BGM::CLEAR);
}

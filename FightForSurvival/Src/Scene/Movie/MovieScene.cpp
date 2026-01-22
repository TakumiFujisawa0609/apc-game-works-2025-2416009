#include "MovieScene.h"
#include "../../Object/MoviePlayer/MoviePlayer.h"
#include "../../Scene/SceneManager.h"
#include "../../Manager/InputManager.h"

MovieScene::MovieScene(void)
{
}

MovieScene::~MovieScene(void)
{
}

void MovieScene::Load(void)
{
    moviePlayer_ = new MoviePlayer("Data/Movie/FightForSurvival.mp4");
}

void MovieScene::Init(void)
{
    moviePlayer_->Init();
    moviePlayer_->Play();
}

void MovieScene::Update(void)
{
    moviePlayer_->Update();

    if (InputManager::GetInstance().ChangeDeviceMouse() ||
        InputManager::GetInstance().ChangeDevicePad() ||
        CheckHitKeyAll() != 0 ||
        moviePlayer_->IsPlayEnd())
    {
        SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
    }
}

void MovieScene::Draw(void)
{
    moviePlayer_->Draw();
}

void MovieScene::Release(void)
{
    moviePlayer_->Release();
    delete moviePlayer_;
    moviePlayer_ = nullptr;
}

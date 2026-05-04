#include "MovieScene.h"
#include "../../Object/MoviePlayer/MoviePlayer.h"
#include "../../Scene/SceneManager.h"
#include "../../Manager/InputManager.h"

MovieScene::MovieScene(void)
    :moviePlayer_(nullptr)
{
}

MovieScene::~MovieScene(void)
{
}

void MovieScene::Load(void)
{
    // 映像ファイルを指定して映像プレイヤーを生成
    moviePlayer_ = new MoviePlayer("Data/Movie/FightForSurvival.mp4");
}

void MovieScene::Init(void)
{
    // 初期化
    moviePlayer_->Init();
    // 再生させる
    moviePlayer_->Play();
}

void MovieScene::Update(void)
{
    // 更新処理
    moviePlayer_->Update();

    // 操作が行われるか、動画が終了したら
    if (InputManager::GetInstance().ChangeDeviceMouse() ||
        InputManager::GetInstance().ChangeDevicePad() ||
        CheckHitKeyAll() != 0 ||
        moviePlayer_->IsPlayEnd())
    {
        // タイトルへ戻す
        SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
    }
}

void MovieScene::Draw(void)
{
    // 動画の描画
    moviePlayer_->Draw();
}

void MovieScene::Release(void)
{
    // 解放処理
    moviePlayer_->Release();
    delete moviePlayer_;
    moviePlayer_ = nullptr;
}

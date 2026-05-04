#include "MoviePlayer.h"
#include "../../Application.h"

MoviePlayer::MoviePlayer(const char* filePath)
	: movieHandle_(-1)
	, filePath_(filePath)
	,isLoadSuccess_(false)
	, state_(STATE::STOP)
{
}

MoviePlayer::~MoviePlayer(void)
{
}

void MoviePlayer::Init(void)
{
	// 映像を読み込む
	movieHandle_ = LoadGraph(filePath_);

	// 映像が読み込めており中身が-1ではなかったら
	if (movieHandle_ != -1)
	{
		// 読み込み成功
		isLoadSuccess_ = true;

		// 状態を停止中にする
		ChangeState(STATE::STOP);
	}
	else
	{
		// 読み込み失敗
		isLoadSuccess_ = false;

		// 状態をエラー終了にする
		ChangeState(STATE::ERROR_END);
	}
}

void MoviePlayer::Update(void)
{

	// 停止中か終了だった場合入らない
	if (state_ == STATE::STOP || state_ == STATE::END)
	{
		return;
	}

	// 動画の再生状態を得る
	int dxState = GetMovieStateToGraph(movieHandle_);

	// 再生状態によって処理を変更
	switch (dxState)
	{
	case 0: // 停止中
		ChangeState(STATE::END);
		break;
	case 1: // 再生中
		ChangeState(STATE::PLAYING);
		break;
	case -1: // エラー
		ChangeState(STATE::ERROR_END);
		break;
	}

}

void MoviePlayer::Draw(void)
{
	// 再生中であれば描画を行う
	if (state_ == STATE::PLAYING)
	{
		// 動画を描画
		DrawExtendGraph(0, 0,
			Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y,
			movieHandle_, true);
	}
}

void MoviePlayer::Release(void)
{
	// 解放
	if (movieHandle_ != -1)
	{
		DeleteGraph(movieHandle_);
		movieHandle_ = -1;
	}

	// 一応停止状態へ初期化
	ChangeState(STATE::STOP);
}

void MoviePlayer::Play(void)
{
	// 読み込みが失敗していたら処理を行わない
	if (!isLoadSuccess_)
	{
		return;
	}

	// 動画ファイルを使用するグラフィックハンドルの動画ファイルの再生を開始する
	PlayMovieToGraph(movieHandle_);
	// 再生中状態にする
	ChangeState(STATE::PLAYING);
}

bool MoviePlayer::IsPlayEnd(void) const
{
	// 状態が終了かエラー終了だったら
	return state_ == STATE::END || state_ == STATE::ERROR_END;
}

bool MoviePlayer::IsLoadSuccess(void) const
{
	// 読み込みが成功していたら
	return isLoadSuccess_;
}

void MoviePlayer::ChangeState(STATE state)
{
	// 指定された状態へ変更
	state_ = state;
}
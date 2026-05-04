#pragma once
#include <DxLib.h>
#include "../../Common/Vector2/Vector2.h"

class MoviePlayer
{
public:

	// 映像の状態
	enum class STATE
	{
		STOP,		// 停止中
		PLAYING,	// 再生中
		END,		// 終了
		ERROR_END,	// エラー終了
	};

	// コンストラクタ
	MoviePlayer(const char* filePath);
	// デストラクタ
	~MoviePlayer(void);

	// 初期化
	void Init(void);
	// 更新
	void Update(void);
	// 描画
	void Draw(void);
	// 解放
	void Release(void);

	// 動画の再生開始
	void Play(void);

	// 動画の再生が終了したかを取得
	bool IsPlayEnd(void)const;

	// 動画のロードに成功したかを取得
	bool IsLoadSuccess(void)const;

private:

	// 映像ハンドル
	int movieHandle_;

	// ファイルパス
	const char* filePath_;

	// 読み込み成功したか	true / 読み込み成功 . false / 読み込み失敗
	bool isLoadSuccess_;

	// 映像の状態
	STATE state_;

	// 指定された状態へ変更
	void ChangeState(STATE state);
};
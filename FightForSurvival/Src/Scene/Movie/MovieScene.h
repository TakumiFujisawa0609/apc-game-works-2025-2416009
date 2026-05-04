#pragma once
#include "../../Base.h"
#include "../Base/SceneBase.h"

class MoviePlayer;

class MovieScene : public SceneBase
{
public:

	// コンストラクタ
	MovieScene(void);
	// デストラクタ
	~MovieScene(void)override;

	// 読み込み処理
	void Load(void) override;
	// 初期化処理
	void Init(void) override;
	// 更新処理
	void Update(void) override;
	// 描画処理
	void Draw(void) override;
	// 解放処理
	void Release(void) override;

private:

	// 映像プレイヤーのハンドル
	MoviePlayer* moviePlayer_;
};

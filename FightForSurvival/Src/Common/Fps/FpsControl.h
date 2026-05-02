#pragma once

#include "../../Application.h"

//フレームレート
static constexpr float FRAME_RATE(1000 / 60);

class FpsControl
{
public:
	
	static constexpr int POS_X = Application::SCREEN_SIZE_X - 96;
	static constexpr int POS_Y = Application::SCREEN_SIZE_Y - 16;

	//コンストラクタ・デストラクタ
	FpsControl();
	~FpsControl();

	void Init(void);				//初期化
	bool UpdateFrameRate(void);		//フレームレート更新
	void CalcFrameRate(void);		//フレームレート計算
	void DrawFrameRate(void);		//フレームレート表示

private:
	int currentTime_;				//現在の時間
	int prevFrameTime_;				//前回のフレーム実行時の時間

	int frameCnt_;					//フレームカウント
	int updateFrameRateTime_;		//フレームレートを更新した時間

	float frameRate_;				//フレームレート
};
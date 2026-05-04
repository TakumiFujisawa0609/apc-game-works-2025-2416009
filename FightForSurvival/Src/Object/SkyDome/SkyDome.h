#pragma once

#include <DxLib.h>

class SkyDome
{
public:

	// 大きさ
	static constexpr VECTOR SCALE = { 100.0f,100.0f,100.0f };

	// 向き
	static constexpr VECTOR ROTATE = { 0.0f,0.0f,0.0f };

	// コンストラクタ
	SkyDome(void);
	// デストラクタ
	~SkyDome(void);

	// 読み込み処理
	void Load(void);
	// 初期化処理
	void Init(const VECTOR& cameraPos);
	// 更新処理
	void Update(const VECTOR& cameraPos);
	// 描画処理
	void Draw(void);
	// 解放処理
	void Release(void);

private:

	// モデルハンドル
	int modelId_;

	// 大きさ
	VECTOR scale_;
	// 向き
	VECTOR rotate_;
	// 座標
	VECTOR pos_;

};


#pragma once

#include <DxLib.h>

class SkyDome
{
public:

	static constexpr VECTOR SCALE = { 100.0f,100.0f,100.0f };
	static constexpr VECTOR ROTATE = { 0.0f,0.0f,0.0f };

	// コンストラクタ
	SkyDome(void);
	// デストラクタ
	~SkyDome(void);

	void Load(void);
	void Init(const VECTOR& cameraPos);
	void Update(const VECTOR& cameraPos);
	void Draw(void);
	void Release(void);

private:
	VECTOR scale_;
	VECTOR rotate_;
	VECTOR pos_;

	int modelId_;
};


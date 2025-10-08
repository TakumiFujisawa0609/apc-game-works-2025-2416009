#pragma once

#include <DxLib.h>

class SkyDome
{
public:

	// コンストラクタ
	SkyDome(void);
	// デストラクタ
	~SkyDome(void);

	void Load(void);
	void Init(void);
	void Update(const VECTOR& cameraPos);
	void Draw(void);
	void Release(void);

private:
	VECTOR pos_;

	int modelId_;
};


#include "SkyDome.h"

#include "../../Application.h"
#include "../../Utility/AsoUtility.h"


SkyDome::SkyDome(void)
{
	modelId_ = -1;
}

SkyDome::~SkyDome(void)
{
}

void SkyDome::Load(void)
{
	// ドームモデルのロード
	modelId_ = MV1LoadModel((Application::PATH_MODEL + "SkyDome/Skydome.mv1").c_str());
}

void SkyDome::Init(void)
{
	// 座標の初期化
	pos_ = AsoUtility::VECTOR_ZERO;
}

void SkyDome::Update(const VECTOR& cameraPos)
{
	// 座標を更新(カメラの座標と同じにする)
	pos_ = cameraPos;
	MV1SetPosition(modelId_, pos_);
}

void SkyDome::Draw(void)
{
	// ドームモデルの描画
	MV1DrawModel(modelId_);
}

void SkyDome::Release(void)
{
	MV1DeleteModel(modelId_);
}

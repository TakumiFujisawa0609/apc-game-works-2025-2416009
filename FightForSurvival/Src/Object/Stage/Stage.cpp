#include <DxLib.h>
#include "Stage.h"

Stage::Stage(void)
{
}

Stage::~Stage(void)
{
}

void Stage::Load(void)
{
	modelId_ = MV1LoadModel("Data/Model/Stage/stage.mv1");
	wallModelId_ = MV1LoadModel("Data/Model/Stage/wall.mv1");
	flowerModelId_ = MV1LoadModel("Data/Model/Stage/stageFlower.mv1");
}

void Stage::Init(void)
{
	// 大きさを設定する
	scale_ = SCALE;
	MV1SetScale(modelId_, scale_);
	MV1SetScale(wallModelId_, scale_);
	MV1SetScale(flowerModelId_, scale_);

	// アングルを設定する
	rotate_ = ROTATE;
	MV1SetRotationXYZ(modelId_, rotate_);
	MV1SetRotationXYZ(wallModelId_, rotate_);
	MV1SetRotationXYZ(flowerModelId_, rotate_);

	// 座標を設定する
	pos_ = DEFAULT_POS;
	MV1SetPosition(modelId_, pos_);
	MV1SetPosition(wallModelId_, pos_);
	MV1SetPosition(flowerModelId_, pos_);
}

void Stage::Update(void)
{
}

void Stage::Draw(void)
{
	MV1DrawModel(modelId_);
	MV1DrawModel(flowerModelId_);
}

void Stage::Release(void)
{
	MV1DeleteModel(modelId_);
	MV1DeleteModel(wallModelId_);
	MV1DeleteModel(flowerModelId_);
}

float Stage::GetModelScaleX(void) const
{
	VECTOR MMax = MV1GetMeshMaxPosition(modelId_, 0);
	VECTOR MMin = MV1GetMeshMinPosition(modelId_, 0);

	//  // ステージのX軸のサイズを計算
	float StageScaleX = (MMax.x - MMin.x) * scale_.x;
	return StageScaleX;
}

float Stage::GetModelScaleZ(void) const
{
	VECTOR MMax = MV1GetMeshMaxPosition(modelId_, 0);
	VECTOR MMin = MV1GetMeshMinPosition(modelId_, 0);

	//  // ステージのZ軸のサイズを計算
	float StageScaleZ = (MMax.z - MMin.z) * scale_.z;
	return StageScaleZ;
}

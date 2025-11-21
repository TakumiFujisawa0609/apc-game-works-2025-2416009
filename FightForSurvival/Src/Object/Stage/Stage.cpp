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
}

void Stage::Init(void)
{
	// ëÂÇ´Ç≥Çê›íËÇ∑ÇÈ
	scale_ = SCALE;
	MV1SetScale(modelId_, scale_);
	MV1SetScale(wallModelId_, scale_);

	// ÉAÉìÉOÉãÇê›íËÇ∑ÇÈ
	rotate_ = ROTATE;
	MV1SetRotationXYZ(modelId_, rotate_);
	MV1SetRotationXYZ(wallModelId_, rotate_);

	// ç¿ïWÇê›íËÇ∑ÇÈ
	pos_ = DEFAULT_POS;
	MV1SetPosition(modelId_, pos_);
	MV1SetPosition(wallModelId_, pos_);
}

void Stage::Update(void)
{
}

void Stage::Draw(void)
{
	MV1DrawModel(modelId_);
}

void Stage::Release(void)
{
	MV1DeleteModel(modelId_);
	MV1DeleteModel(wallModelId_);
}

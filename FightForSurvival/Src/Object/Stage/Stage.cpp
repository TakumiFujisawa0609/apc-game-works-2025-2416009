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
}

void Stage::Init(void)
{
	// ‘å‚«‚³‚ğİ’è‚·‚é
	scale_ = SCALE;
	MV1SetScale(modelId_, scale_);

	// ƒAƒ“ƒOƒ‹‚ğİ’è‚·‚é
	rotate_ = ROTATE;
	MV1SetRotationXYZ(modelId_, rotate_);

	// À•W‚ğİ’è‚·‚é
	pos_ = DEFAULT_POS;
	MV1SetPosition(modelId_, pos_);
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
}

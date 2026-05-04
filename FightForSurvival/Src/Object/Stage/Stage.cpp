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
	// モデルのロード
	modelId_ = MV1LoadModel("Data/Model/Stage/stage.mv1");
	wallModelId_ = MV1LoadModel("Data/Model/Stage/wall.mv1");
	flowerModelId_ = MV1LoadModel("Data/Model/Stage/stageFlower.mv1");
}

void Stage::Init(void)
{
	// それぞれのモデルに大きさを設定する
	scale_ = SCALE;
	MV1SetScale(modelId_, scale_);
	MV1SetScale(wallModelId_, scale_);
	MV1SetScale(flowerModelId_, scale_);

	// それぞれのモデルに向きを設定する
	rotate_ = ROTATE;
	MV1SetRotationXYZ(modelId_, rotate_);
	MV1SetRotationXYZ(wallModelId_, rotate_);
	MV1SetRotationXYZ(flowerModelId_, rotate_);

	// それぞれのモデルに座標を設定する
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
	// 壁モデルは当たり判定ようなので壁モデル以外を描画
	MV1DrawModel(modelId_);
	MV1DrawModel(flowerModelId_);
}

void Stage::Release(void)
{
	// モデルを解放
	MV1DeleteModel(modelId_);
	MV1DeleteModel(wallModelId_);
	MV1DeleteModel(flowerModelId_);
}

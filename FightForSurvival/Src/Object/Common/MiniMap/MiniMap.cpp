#include "../../Enemy/EnemyManager.h"
#include "../../Player/Player.h"
#include "MiniMap.h"

MiniMap::MiniMap(int stageSizeX, int stageSizeZ, VECTOR& plaPos)
	:
	miniMapImgHandle_(-1),
	playerPos_(plaPos)
{
}

MiniMap::~MiniMap(void)
{
}

void MiniMap::Load(void)
{
	// ミニマップの画像ハンドル
	miniMapImgHandle_ = LoadGraph("Data/Image/UI/MiniMap/miniMap.png");
}

void MiniMap::Init(void)
{
	// 描画位置初期化
	miniMapPos = {};

	// 描画物の初期化
	objectRadis_ = OBJECT_RADIUS;
}

void MiniMap::Update(void)
{
	// 0番目のメッシュの最大・最小座標を直接取得
	//VECTOR MMax = MV1GetMeshMaxPosition(StageModelHandle, 0);
	//VECTOR MMin = MV1GetMeshMinPosition(StageModelHandle, 0);

	//// ステージのサイズを計算
	//float StageWidth = MMax.x - MMin.x;
	//float StageDepth = MMax.z - MMin.z;

	//int referencePoint = miniMapPos.x + MINI_MAP_SIZE_XY / 2;
	//float miniMapScale = 
	//miniMapPlayerPos_.x = referencePoint+ playerPos_.x/stageWid;

	//referencePoint = miniMapPos.y + MINI_MAP_SIZE_XY / 2;
	//miniMapPlayerPos_.y = referencePoint + playerPos_.y / stageHig;

}

void MiniMap::Draw(void)
{
	// ミニマップ背景
	DrawGraph(miniMapPos.x, miniMapPos.y, miniMapImgHandle_, true);

	// 敵の描画
	for (Vector2& enemy : miniMapEnemiesPos_)
	{
		DrawCircle(enemy.x, enemy.y, objectRadis_, 0xff0000);
	}

	// プレイヤーの描画
	DrawCircle(miniMapPlayerPos_.x, miniMapPlayerPos_.y, objectRadis_, 0x0000ff);

}

void MiniMap::Release(void)
{
	// 中身の解放
	enemiesPos_.clear();
	miniMapEnemiesPos_.clear();
}

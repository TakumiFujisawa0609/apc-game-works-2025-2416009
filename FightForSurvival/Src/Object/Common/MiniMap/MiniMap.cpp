#include "../../Enemy/EnemyManager.h"
#include "../../Player/Player.h"
#include "MiniMap.h"

MiniMap::MiniMap(VECTOR& plaPos)
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
	// ミニマップの画像(背景)ハンドル
	miniMapImgHandle_ = LoadGraph("Data/Image/UI/MiniMap/miniMap.png");
}

void MiniMap::Init(void)
{
	// 描画位置初期化
	miniMapPos = {0,0};

	// 描画物の初期化
	objectRadis_ = OBJECT_RADIUS;

	stageModelSizeX_ = stageModelSizeZ_ = 15680;
}

void MiniMap::Update(void)
{

	int referencePoint = miniMapPos.x + MINI_MAP_SIZE_XY / 2;
	float miniMapScale = 
	miniMapPlayerPos_.x = referencePoint+ playerPos_.x/ stageModelSizeX_;

	referencePoint = miniMapPos.y + MINI_MAP_SIZE_XY / 2;
	miniMapPlayerPos_.y = referencePoint + playerPos_.y / stageModelSizeZ_;

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
	// 背景を削除
	DeleteGraph(miniMapImgHandle_);

	// 中身の解放
	enemiesPos_.clear();
	miniMapEnemiesPos_.clear();
}

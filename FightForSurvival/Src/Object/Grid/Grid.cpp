#include <DxLib.h>
#include "Grid.h"

// コンストラクタ
Grid::Grid(void)
{
}

// デストラクタ
Grid::~Grid(void)
{
}

void Grid::Init(void)
{
}

void Grid::Update(void)
{
}

void Grid::Draw(void)
{

	// グリッド線
	for (int i = -HNUM; i < HNUM; i++)
	{

		float num = static_cast<float>(i);

		// X軸(赤)
		VECTOR sPos = { -HLEN, 0.0f, num * TERM };
		VECTOR ePos = { HLEN, 0.0f, num * TERM };
		DrawLine3D(sPos, ePos, 0xff0000);
		DrawSphere3D(ePos, RADIUS, NUM, 0xff0000, 0xff0000, true);

		// Z軸(青)
		sPos = { num * TERM, 0.0f, -HLEN };
		ePos = { num * TERM, 0.0f, HLEN };
		DrawLine3D(sPos, ePos, 0x0000ff);
		DrawSphere3D(ePos, RADIUS, NUM, 0x0000ff, 0x0000ff, true);

	}

	// Y軸(緑)
	VECTOR sPos = { 0.0f, -HLEN, 0.0f };
	VECTOR ePos = { 0.0f, HLEN, 0.0f };
	DrawLine3D(sPos, ePos, 0x00ff00);
	DrawSphere3D(ePos, RADIUS, 1, 0x00ff00, 0x00ff00, true);

}

void Grid::Release(void)
{
}

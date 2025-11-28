#pragma once

#include "../../Common/Vector2/Vector2.h"

namespace CollisionUtility
{
#pragma region 3D

	float VLenSq(VECTOR v);

	// 球の移動経路の線分とカプセルの線分間の最近接距離の2乗を計算
	float GetMinDistSqSegmentToSegment(VECTOR p1, VECTOR p2, VECTOR p3, VECTOR p4);
	// カプセル型同士の当たり判定
	bool IsCollidingCapsules(VECTOR capsule1Top, VECTOR capsule1Under, float capsule1Rad, VECTOR capsule2Top, VECTOR capsule2Under, float capsule2Rad);

	// 球とカプセル型の当たり判定
	bool IsCollidingSphereCapsule(VECTOR spherePos, float sphereRad, VECTOR capsuleTop, VECTOR capsuleUnder, float capsuleRad);

	// 球同士の当たり判定
	bool IsCollidingSpheres(VECTOR sphere1Pos, float sphere1Rad, VECTOR sphere2Pos, float sphere2Rad);

	// 球と点の当たり判定
	bool IsCollidingSphereAndPos(VECTOR sphere1Pos, float sphere1Rad, VECTOR pos);

	// 押し出し判定(押し出し量を渡してくれる)
	VECTOR ExtrusionCollision(VECTOR pos1, float collRad1, VECTOR pos2, float collRad2);

	// ラインの当たり判定
	bool CollisionLine(VECTOR startPos, VECTOR endPos, int modelId, VECTOR& movePos);

	// カプセルの当たり判定
	VECTOR CoolisionCapsule(VECTOR topPos, VECTOR downPos, float rad, int modelId);

	// 扇形当たり判定
	bool CollisionSecter(VECTOR pos1, VECTOR dir,VECTOR pos2, float radius,float viewRange,float viewAngle);

#pragma endregion

#pragma region 2D
	// 矩形と点の当たり判定(左上からの当たり判定)
	bool RectangleAndPoint(Vector2 pos1, int wid1, int hig1, Vector2 pos2);

	// 矩形とマウスの当たり判定(左上からの当たり判定)
	bool RectangleAndMouse(Vector2 pos, int wid, int hig);

	// 円形とマウスの当たり判定
	bool CircleAndMouse(Vector2 pos, int rad);
#pragma endregion

};


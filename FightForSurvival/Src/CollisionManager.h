#pragma once

namespace CollisionManager
{
	float VLenSq(VECTOR v);

	// 球の移動経路の線分とカプセルの線分間の最近接距離の2乗を計算
	float GetMinDistSqSegmentToSegment(VECTOR p1, VECTOR p2, VECTOR p3, VECTOR p4);
	// カプセル型と球の当たり判定
	bool IsCollidingCapsuleSphere(VECTOR capsule1, VECTOR capsule2, float capsuleRad, VECTOR lineStart, VECTOR lineEnd, float sphereRad);

	// 球同士の当たり判定
	bool IsCollidingSphereAndSphere(VECTOR spherePos_1, float sphereRad_1, VECTOR lineStart, VECTOR lineEnd, float sphereRad_2);

};


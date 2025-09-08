#include <DxLib.h>
#include "CollisionManager.h"

float CollisionManager::VLenSq(VECTOR v)
{
	return v.x * v.x + v.y * v.y + v.z * v.z;
}

float CollisionManager::GetMinDistSqSegmentToSegment(VECTOR p1, VECTOR p2, VECTOR p3, VECTOR p4)
{
	VECTOR d1 = VSub(p2, p1); // 線分1のベクトル
	VECTOR d2 = VSub(p4, p3); // 線分2のベクトル
	VECTOR r = VSub(p1, p3);

	float a = VDot(d1, d1); // 線分1の長さの2乗
	float e = VDot(d2, d2); // 線分2の長さの2乗
	float f = VDot(d2, r);

	float s = 0.0f;
	float t = 0.0f;

	if (a <= 1.0E-6f && e <= 1.0E-6f) { // 両方の線分が点の場合
		return VLenSq(VSub(p1, p3));
	}
	if (a <= 1.0E-6f) { // 線分1が点の場合
		t = VDot(d2, r) / e;
		if (t < 0.0f) t = 0.0f;
		if (t > 1.0f) t = 1.0f;
		return VLenSq(VSub(VSub(p3, p1), VScale(d2, t)));
	}
	float c = VDot(d1, r);
	if (e <= 1.0E-6f) { // 線分2が点の場合
		s = VDot(d1, VSub(p3, p1)) / a;
		if (s < 0.0f) s = 0.0f;
		if (s > 1.0f) s = 1.0f;
		return VLenSq(VSub(VSub(p1, p3), VScale(d1, s)));
	}

	float b = VDot(d1, d2);
	float denom = a * e - b * b;

	if (denom != 0.0f) {
		s = (b * f - c * e) / denom;
		if (s < 0.0f) s = 0.0f;
		if (s > 1.0f) s = 1.0f;
	}
	else {
		s = 0.0f;
	}

	t = (b * s + f) / e;

	if (t < 0.0f) {
		t = 0.0f;
		s = -c / a;
		if (s < 0.0f) s = 0.0f;
		if (s > 1.0f) s = 1.0f;
	}
	else if (t > 1.0f) {
		t = 1.0f;
		s = (b - c) / a;
		if (s < 0.0f) s = 0.0f;
		if (s > 1.0f) s = 1.0f;
	}

	VECTOR P1_ = VAdd(p1, VScale(d1, s));
	VECTOR P2_ = VAdd(p3, VScale(d2, t));

	return VLenSq(VSub(P1_, P2_));
}

bool CollisionManager::IsCollidingCapsuleSphere(VECTOR capsule1, VECTOR capsule2, float capsuleRad, VECTOR lineStart, VECTOR lineEnd, float sphereRad)
{
	// 弾の移動経路の線分と敵カプセルの線分間の最近接距離の2乗を計算
	float distSq = CollisionManager::GetMinDistSqSegmentToSegment(capsule1, capsule2, lineStart, lineEnd);

	// カプセルと球の半径の合計
	float total_radius = capsuleRad + sphereRad;

	return distSq <= total_radius * total_radius;
}

bool CollisionManager::IsCollidingSphereAndSphere(VECTOR spherePos_1, float sphereRad_1, VECTOR lineStart, VECTOR lineEnd, float sphereRad_2)
{
	// 敵の球1は、カプセルと判定する対象
	VECTOR sphere_pos = spherePos_1;

	// 球2の移動経路（カプセルの中心線）を定義
	VECTOR capsule_p1 = lineStart;
	VECTOR capsule_p2 = lineEnd;

	// カプセルの線分を表すベクトル
	VECTOR AB = VSub(capsule_p2, capsule_p1);
	// 線分の始点から球の中心へのベクトル
	VECTOR AP = VSub(sphere_pos, capsule_p1);

	// 線分ABの長さの2乗
	float ab_len_sq = VLenSq(AB);

	float t = 0.0f;
	if (ab_len_sq > 0.0f) {
		// 媒介変数tを計算
		t = VDot(AP, AB) / ab_len_sq;
		// tを0.0から1.0の範囲にクランプ
		if (t < 0.0f) t = 0.0f;
		if (t > 1.0f) t = 1.0f;
	}

	// カプセルの線分上の、球の中心に最も近い点Cを求める
	VECTOR C = VAdd(capsule_p1, VScale(AB, t));

	// 点Cと球の中心Pの距離の2乗を計算
	float dist_sq = VLenSq(VSub(sphere_pos, C));

	// 静止している球と、カプセル（動く球）の半径の合計
	float total_radius = sphereRad_1 + sphereRad_2;

	// 距離が半径の合計以下かチェック
	return dist_sq <= total_radius * total_radius;
}

bool CollisionManager::RectangleAndPoint(Vector2 pos1, int wid1, int hig1, Vector2 pos2)
{
	if (pos1.x + wid1 > pos2.x &&
		pos1.x < pos2.x &&
		pos1.y + hig1 >pos2.y &&
		pos1.y < pos2.y)
	{
		return true;
	}

	return false;
}

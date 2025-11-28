#include <DxLib.h>
#include "../AsoUtility.h"
#include "CollisionUtility.h"

float CollisionUtility::VLenSq(VECTOR v)
{
	return v.x * v.x + v.y * v.y + v.z * v.z;
}

float CollisionUtility::GetMinDistSqSegmentToSegment(VECTOR p1, VECTOR p2, VECTOR p3, VECTOR p4)
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

bool CollisionUtility::IsCollidingCapsules(VECTOR capsule1Top, VECTOR capsule1Under, float capsule1Rad, VECTOR capsule2Top, VECTOR capsule2Under, float capsule2Rad)
{
	// 魔法の移動経路の線分と敵カプセルの線分間の最近接距離の2乗を計算
	float distSq = CollisionUtility::GetMinDistSqSegmentToSegment(capsule1Top, capsule1Under, capsule2Top, capsule2Under);

	// カプセルと球の半径の合計
	float total_radius = capsule1Rad + capsule2Rad;

	return distSq <= total_radius * total_radius;
}

bool CollisionUtility::IsCollidingSphereCapsule(VECTOR spherePos, float sphereRad, VECTOR capsuleTop, VECTOR capsuleUnder, float capsuleRad)
{
	// 敵の球1は、カプセルと判定する対象
	VECTOR sphere_pos = spherePos;

	// 球2の移動経路（カプセルの中心線）を定義
	VECTOR capsule_p1 = capsuleTop;
	VECTOR capsule_p2 = capsuleUnder;

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
	float total_radius = sphereRad + capsuleRad;

	// 距離が半径の合計以下かチェック
	return dist_sq <= total_radius * total_radius;
}

bool CollisionUtility::IsCollidingSpheres(VECTOR sphere1Pos, float sphere1Rad, VECTOR sphere2Pos, float sphere2Rad)
{
	// 中心点間の距離の2乗を計算
	VECTOR dis = VSub(sphere2Pos, sphere1Pos);
	float distance = dis.x * dis.x + dis.y * dis.y + dis.z * dis.z;

	// 半径の合計を計算
	float totalRad = sphere1Rad + sphere2Rad;

	// 距離の2乗が半径の合計の2乗より小さいか判定
	return (distance < totalRad * totalRad);
}

bool CollisionUtility::IsCollidingSphereAndPos(VECTOR sphere1Pos, float sphere1Rad, VECTOR pos)
{
	// 中心点間の距離の2乗を計算
	VECTOR dis = VSub(pos, sphere1Pos);
	float distance = dis.x * dis.x + dis.z * dis.z;

	// 距離の2乗が半径の合計の2乗より小さいか判定
	return (distance < sphere1Rad * sphere1Rad);
}

VECTOR CollisionUtility::ExtrusionCollision(VECTOR pos1, float collRad1, VECTOR pos2, float collRad2)
{
	VECTOR pushPow = AsoUtility::VECTOR_ZERO;

	// 球体と球体の衝突判定
	// ２つの座標間の距離をピタゴラスの定理で算出

	VECTOR distance = VSub(pos1, pos2);

	float dis = distance.x * distance.x + distance.y * distance.y + distance.z * distance.z;

	// お互いの半径を合計する
	float radius = collRad1 + collRad2;

	// 合計した半径の２乗よりも、
	// ２つの座標間の距離が小さければ球体は衝突している
	if (radius * radius > dis && dis != 0.0f)
	{
		float length = sqrtf(dis);
		auto overlap = radius - length;

		// 正規化ベクトル（A -> Bの方向）
		VECTOR vec = VNorm(distance);

		// 重なり量の半分
		float push_half = overlap / 2.0f;

		// 押し出し量を計算
		pushPow = VScale(vec, push_half);
		// 上下の押し出しは行わない
		pushPow.y = 0.0f;
	}

	return pushPow;
}

bool CollisionUtility::CollisionLine(VECTOR startPos, VECTOR endPos, int modelId, VECTOR& movePos)
{
	// 線分とモデルの衝突判定
	MV1_COLL_RESULT_POLY res =
		MV1CollCheck_Line(modelId, -1, startPos, endPos);

	// モデルと衝突しているか？
	if (res.HitFlag)
	{
		// 移動量に当たった座標入れる
		movePos = res.HitPosition;

		// 当たったことを返す
		return true;
	}

	// 当たっていないことを返す
	return false;
}

VECTOR CollisionUtility::CoolisionCapsule(VECTOR topPos, VECTOR downPos, float rad, int modelId)
{
	// 移動量
	VECTOR movePos = VGet(0.0f, 0.0f, 0.0f);

	// カプセルとの当たり判定
	MV1_COLL_RESULT_POLY_DIM hits = MV1CollCheck_Capsule(modelId, -1, topPos, downPos, rad);

	// 衝突したポリゴンの数分の検索
	for (int i = 0; i < hits.HitNum; i++)
	{
		// ポリゴン1枚に分解
		MV1_COLL_RESULT_POLY hit = hits.Dim[i];

		// ポリゴン検索を制限(全てすると重いため)
		for (int tryCnt = 0; tryCnt < 20; tryCnt++)
		{
			// 最初の衝突判定で検出した衝突ポリゴン1枚と衝突判定を取る
			int pHit = HitCheck_Capsule_Triangle(topPos, downPos, rad, hit.Position[0], hit.Position[1], hit.Position[2]);

			// カプセルとポリゴンが当たっていた
			if (pHit)
			{
				// 当たっていたので座標をポリゴンの法線方向に移動させる
				movePos = VAdd(movePos, VScale(hit.Normal, 1.0f));

				// カプセルの座標も移動させる
				topPos = VAdd(topPos, VScale(hit.Normal, 1.0f));
				downPos = VAdd(downPos, VScale(hit.Normal, 1.0f));

				// 複数当たっている可能性があるので再検索
				continue;
			}
		}
	}

	// 検出した地面ポリゴン情報の後始末
	MV1CollResultPolyDimTerminate(hits);

	// 移動量を返す
	return movePos;
}

bool CollisionUtility::CollisionSecter(VECTOR pos1, VECTOR dir, VECTOR pos2, float radius, float viewRange, float viewAngle)
{
	// pos1から見たpos2の方向(VNorm(正規化)を行い単位ベクトルにする)
	VECTOR distance = VSub(pos2,pos1);
	VECTOR dirNorm = VNorm(distance);

	// 内積を使ってベクトルの比較を行う
	// +1.0～-1.0の値を取得
	// +1.0の場合、2つのベクトルは同じ方向
	// +0.8の場合、2つのベクトルは結構同じ方向
	// 0.0の場合、2つのベクトルは直交
	// -1.0の場合、2つのベクトルは逆方向
	float dot = VDot(dir, dirNorm);
	float angle = acosf(dot);

	// 視野角をラジアンに返還
	const float viewRad = AsoUtility::Deg2RadF(viewAngle);

	// pos1とpos2間の距離を調べる
	float dis = (distance.x * distance.x + distance.y * distance.y + distance.z * distance.z);

	// 当たり判定用半径の合計を計算
	float collisionRad = viewRange + radius;

	// 視野の範囲内に入っているかつ、攻撃時間になったらtrueを返す
	return angle <= viewRad && (collisionRad * collisionRad) > dis;
}

bool CollisionUtility::RectangleAndPoint(Vector2 pos1, int wid1, int hig1, Vector2 pos2)
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

bool CollisionUtility::RectangleAndMouse(Vector2 pos, int wid, int hig)
{
	Vector2 mousePos;
	// マウスの位置を調べる
	GetMousePoint(&mousePos.x, &mousePos.y);

	// マウスの位置から矩形と当たっているか調べる
	if (pos.x + wid > mousePos.x &&
		pos.x < mousePos.x &&
		pos.y + hig >mousePos.y &&
		pos.y < mousePos.y)
	{
		// 当たっていたらtrueを返す
		return true;
	}

	// 当たっていなかったらfalseを返す
	return false;
}

bool CollisionUtility::CircleAndMouse(Vector2 pos, int rad)
{
	Vector2 mousePos;
	// マウスの位置を調べる
	GetMousePoint(&mousePos.x, &mousePos.y);

	Vector2 dis;
	// マウスの位置と円の距離を求める
	dis.x = pos.x - mousePos.x;
	dis.y = pos.y - mousePos.y;

	int distance = dis.x * dis.x + dis.y * dis.y;

	// 距離の内積が半径の2乗よりも小さければ当たっている
	if (distance < rad * rad)
	{
		// 当たっていたらtrueを返す
		return true;
	}

	// 当たっていなかったらfalseを返す
	return false;
}

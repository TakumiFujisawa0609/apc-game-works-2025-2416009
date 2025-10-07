#pragma once

#include "../../Base.h"

class WeaponBase;


class Player
{
public:

	// モデルの大きさ
	static constexpr VECTOR SCALES = { 0.5f, 0.5f, 0.5f };

	// 初期位置
	static constexpr VECTOR DEFAULT_POS = { 0.0f, 80.0f, 0.0f };

	// プレイヤーと杖の相対座標
	static constexpr float RELATIVE_POS_CAMERA = 70.0f;

	// 移動速度
	static constexpr float DEFAULT_MOVE_SPEED = 7.0f;
	static constexpr float DASH_SPEED = 15.0f;

	// 最大HP
	static constexpr int DEFAULT_HP = 10;

	// スタミナ
	static constexpr float DEFAULT_STAMINA = 20.0f;

	// スタミナ回復量
	static constexpr float RECOVERY_STAMINA = 0.1f;

	// スタミナ回復を行うまでの時間
	static constexpr float RECOVERY_STAMINA_WAIT_TIME = 1.0f;

	// 衝突判定用半径
	static constexpr float COLLISION_RADIUS = 35.0f;

	// 視点の最大角度
	static constexpr float MAX_VIEW_ANGLE = 1.5f;

	// 視点の最小角度
	static constexpr float MIN_VIEW_ANGLE = -1.5f;

	// 当たり判定用カプセルのプレイヤー座標からのオフセット
	static constexpr float COLLISION_OFFSET_TOP = 50.0f;
	static constexpr float COLLISION_OFFSET_UNDER = -50.0f;

	// 視点のしきい値
	static constexpr float THRESHOLD = 1.1f;

	// コンストラクタ
	Player(void);

	// デストラクタ
	~Player(void);

	// ロード処理
	void Load(void);

	// 初期化処理
	void Init(void);

	// 更新
	void Update(void);

	// 描画処理
	void Draw(void);

	// 解放処理
	void Release(void);

	// ゲッターセッター関数
	const Unit GetPlayer(void)const { return player_; }
	const VECTOR GetCameraPos(void) const { return cameraPos_; }
	const float GetYaw(void) const { return yaw_; }
	const float GetPitch(void) const { return pitch_; }
	const VECTOR GetCollisionPosTop(void) const { return collisionPosTop_; }
	const VECTOR GetCollisionPosUnder(void) const { return collisionPosUnder_; }
	void SetPitch(float newPitch)  { pitch_ = newPitch; }

	// 杖のポインター受け渡し
	WeaponBase* GetWeapon(void)const { return weapon_; }

	/// <summary>
	/// プレイヤー強化処理
	/// </summary>
	/// <param name="type">強化する能力の種類</param>
	/// <param name="i">実際に強化させる値</param>
	//void StrengthenAbility(ABILITY_TYPE type, float i);

	// HPにダメージを与える
	void SubHp(int hp);

private:

	// 杖クラスのインスタンス
	WeaponBase* weapon_;

	// プレイヤーの情報
	Unit player_;

	// カメラの位置
	VECTOR cameraPos_;

	Ability ability_;

	// マウスの座標
	Vector2 mouse_;

	// ヨー角、ピッチ角
	float yaw_;
	float pitch_;

	// マウスの感度
	float sensitivity_;

	// スタミナを回復させるまでの時間カウンタ
	float staminaCounter_;

	// 当たり判定用カプセル座標
	VECTOR collisionPosTop_;
	VECTOR collisionPosUnder_;

	// 移動処理
	void ProcessMove(void);
	// 視点処理
	void ProcessAngle(void);
	// 攻撃処理
	void ProcessAttack(void);

	void Reload(void);
};

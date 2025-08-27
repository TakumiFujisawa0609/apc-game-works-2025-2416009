#pragma once

#include "../../Base.h"

class Player
{
public:

	// モデルの大きさ
	static constexpr VECTOR SCALES = { 0.5f, 0.5f, 0.5f };

	// 初期位置
	static constexpr VECTOR DEFAULT_POS = { 0.0f, 305.0f, 0.0f };


	// 移動速度
	static constexpr float DEFAULT_MOVE_SPEED = 7.0f;
	static constexpr float DASH_SPEED = 15.0f;

	// 最大HP
	static constexpr int DEFAULT_HP = 10;

	// スタミナ
	static constexpr float DEFAULT_STAMINA = 30.0f;

	// スタミナ回復量
	static constexpr float RECOVERY_STAMINA = 0.1f;

	// スタミナ回復を行うまでの時間
	static constexpr float RECOVERY_STAMINA_WAIT_TIME = 1.0f;

	enum class AblityType
	{
		SPEED_UP,
		HEAL_HP,
		HP_UP,
		STAMINA_UP,
	};

	// コンストラクタ
	Player(void);

	// デストラクタ
	~Player(void);

	// ロード処理
	void Load(void);

	// 初期化処理
	void Init(void);

	// 更新ステップ
	void Update(void);

	// 描画処理
	void Draw(void);

	// 解放処理
	void Release(void);

	const Unit GetPlayer(void)const { return player_; }
	const float GetYaw() const { return yaw_; }
	const float GetPitch() const { return pitch_; }

	void SetAbility(AblityType type, float i);

private:

	// プレイヤーの情報
	Unit player_;

	Ability ability_;

	// マウスの座標
	Vector2 mouse_;

	// ヨー角、ピッチ角
	float yaw_;
	float pitch_;

	// マウスの感度
	float sensitivity_;

	float staminaCounter_;

	// 移動処理
	void ProcessMove(void);
	// 視点処理
	void ProcessAngle(void);
};

#pragma once

#include  "../ObjectBase.h"
#include "../../Base.h"

class Player : public ObjectBase
{
public:

	// モデルの大きさ
	static constexpr VECTOR SCALES = { 0.5f, 0.5f, 0.5f };

	// 初期位置
	static constexpr VECTOR DEFAULT_POS = { 3.0f, 305.0f, 300.0f };

	// 移動スピード
	static constexpr float SPEED_MOVE = 10.0f;
	static constexpr float SPEED_MOVE_MAX = 18.0f;

	// 最大HP
	static constexpr int MAX_HP = 5;

	// コンストラクタ
	Player(void);

	// デストラクタ
	~Player(void)override;

	// ロード処理
	void Load(void)override;

	// 初期化処理
	void Init(void)override;

	// 更新ステップ
	void Update(void)override;

	// 描画処理
	void Draw(void)override;

	// 解放処理
	void Release(void)override;

	const Unit GetPlayer(void)const { return player_; }
	const float GetYaw() const { return yaw_; }
	const float GetPitch() const { return pitch_; }

private:

	// プレイヤーの情報
	Unit player_;

	// マウスの座標
	Vector2 mouse_;

	// ヨー角、ピッチ角
	float yaw_;
	float pitch_;

	// マウスの感度
	float sensitivity_;

	// 移動処理
	void ProcessMove(void);
	// 視点処理
	void ProcessAngle(void);
};


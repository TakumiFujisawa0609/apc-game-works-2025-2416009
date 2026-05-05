#pragma once

#include "../../Base.h"
#include "../../Application.h"

class WeaponBase;

// アップグレードの内容
enum class PLAYER_UPGRADE
{
	RESTOCK_POTION,
	HP_UP,
	SPEED_UP,
	STAMINA_UP,
	HEAL_HP,
	MAX,
};

class Player
{
public:

	// モデルの大きさ
	static constexpr VECTOR SCALE = { 0.5f, 0.5f, 0.5f };

	// 初期位置
	static constexpr VECTOR DEFAULT_POS = { 0.0f, 100.0f, 0.0f };

	// プレイヤーとカメラの相対座標
	static constexpr float RELATIVE_POS_CAMERA = 90.0f;

	// 移動速度
	static constexpr float DEFAULT_MOVE_SPEED = 7.0f;
	static constexpr float DASH_SPEED = 10.0f;

	// 最大HP
	static constexpr float DEFAULT_HP = 10.0f;

	// スタミナ
	static constexpr float DEFAULT_STAMINA = 10.0f;

	// スタミナ回復量
	static constexpr float RECOVERY_STAMINA = 0.1f;

	// スタミナ回復を行うまでの時間
	static constexpr float RECOVERY_STAMINA_WAIT_TIME = 1.0f;

	// 衝突判定用半径
	static constexpr float COLLISION_RADIUS = 40.0f;

	// 視点の最大角度
	static constexpr float MAX_VIEW_ANGLE = 1.5f;

	// 視点の最小角度
	static constexpr float MIN_VIEW_ANGLE = -1.5f;

	// 当たり判定用カプセルのプレイヤー座標からのオフセット
	static constexpr float COLLISION_OFFSET_TOP = 50.0f;
	static constexpr float COLLISION_OFFSET_UNDER = -50.0f;

	// 視点のしきい値
	static constexpr float THRESHOLD = 1.1f;

	// 最大魔法攻撃回数
	static constexpr int MAGIC_CAPACITY = 10;

	// MPポーション数
	static constexpr int MP_POTION_NUM = 10;

	// リロードする時間
	static constexpr float RELOAD_TIME = 2.0f;

	// リロードする時間
	static constexpr float CHANGE_MAGIC_INTERVAL = 7.0f;

	// 重力
	static constexpr float GRAVITATION = -0.3f;
	// 最大重力
	static constexpr float MAX_GRAVITATION = -9.8f;

	// 魔法の残弾数の文字の座標
	static constexpr int MAGIC_NUM_POS_X = Application::SCREEN_SIZE_X - 208;
	static constexpr int MAGIC_NUM_POS_Y = Application::SCREEN_SIZE_Y - 200;

	// MPポーションの残りの数の文字の座標
	static constexpr int MP_POTION_NUM_POS_X = Application::SCREEN_SIZE_X - 80;
	static constexpr int MP_POTION_NUM_POS_Y = Application::SCREEN_SIZE_Y - 40;

	// 攻撃変更可能時間の文字の座標
	static constexpr int CHANGE_MAGIC_INTERVAL_POS_X = Application::SCREEN_SIZE_X - 210;
	static constexpr int CHANGE_MAGIC_INTERVAL_POS_Y = Application::SCREEN_SIZE_Y - 150;

	// フォントのカラー
	static constexpr unsigned int FONT_COLOR = 0xffffff;

	// ポーション使用時のプログレスバーの座標
	// 左上
	static constexpr int MP_BAR_UPPER_LEFT_POS_X = Application::SCREEN_SIZE_X / 2 - 50;
	static constexpr int MP_BAR_UPPER_LEFT_POS_Y = Application::SCREEN_SIZE_Y / 2 - 30;
	// 右下(枠)		  
	static constexpr int MP_BAR_LOWER_RIGHT_POS_X = Application::SCREEN_SIZE_X / 2 + 50;
	static constexpr int MP_BAR_LOWER_RIGHT_POS_Y = Application::SCREEN_SIZE_Y / 2 - 40;
	// 文字の座標  
	static constexpr int MP_BAR_STRING_POS_X = Application::SCREEN_SIZE_X / 2 - 60;
	static constexpr int MP_BAR_STRING_POS_Y = Application::SCREEN_SIZE_Y / 2 - 60;

	// MPバーの最大幅
	static constexpr int MP_BAR_MAX_WIDTH = 50;

	// プログレスバーのカラー値
	static constexpr unsigned int MP_BAR_FRAME_COLOR = 0x696969;
	static constexpr unsigned int MP_BAR_MAIN_COLOR = 0xff7f50;

	enum class FONT_KIND
	{
		MAGIC_NUM,
		CHANGE_MAGIC_INTERVAL,
		MP_POTION_NUM,
		USE_POTION,

		MAX,
	};

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
	void Draw2D(void);

	// 解放処理
	void Release(void);

	// ゲッターセッター関数
	const Unit GetPlayerStatus(void)const { return player_; }
	const Ability GetPlayerAblity(void)const { return ability_; }
	const VECTOR GetCameraPos(void) const { return cameraPos_; }
	const VECTOR GetCollisionPosTop(void) const { return collisionPosTop_; }
	const VECTOR GetCollisionPosUnder(void) const { return collisionPosUnder_; }
	int GetMagicNum(void)const { return magicNum_; }
	int GetMagicType(void) const { return magicType_; }
	void SetPitch(float newPitch)  { player_.angle_.x = newPitch; }

	// 杖のポインター受け渡し
	WeaponBase* GetWeapon(void)const { return weapon_; }

	// HPにダメージを与える
	void Damage(float damage);

	// 押し出しを行う
	void Extrusion(VECTOR overlap);

	// プレイヤーの能力を上げる
	void Upgrade(PLAYER_UPGRADE type, float upNum);

	// 衝突判定
	void CollisionStage(VECTOR pos);
	void CollisionStage(float posY);

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

	// マウスの感度
	float mouseSensitivity_;

	// パッドの感度
	float padSensitivity_;

	// スタミナを回復させるまでの時間カウンタ
	float staminaCounter_;

	// 当たり判定用カプセル座標
	VECTOR collisionPosTop_;
	VECTOR collisionPosUnder_;

	PLAYER_UPGRADE upgradeType_;

	// MPポーション数
	int MPPotionNum_;
	// 魔法攻撃可能回数
	int magicNum_;
	// 最大魔法攻撃回数
	int magicCapacity_;

	// MP回復時間
	float healMPTime_;
	// MP回復中か
	bool isHealMP_;

	// 魔法の種類
	int magicType_;
	// 魔法変更可能間隔
	float changeMagicInterval_;

	// フォント情報
	int font_[static_cast<int>(FONT_KIND::MAX)];

	// 移動処理
	void ProcessMove(void);
	// 視点処理
	void ProcessAngle(void);
	void MouseAngle(void);
	void PadAngle(void);
	// 攻撃処理
	void ProcessAttack(void);
	// MP回復処理
	void ProcessHealMP(void);

	// MP回復条件
	bool StartHealMpTrg(void);

	// 感度取得処理
	void Sensitivity(void);

	// プレイヤーの攻撃の種類を切り替える
	void ChangeAttackType(void);

	// 重力処理
	void Gravity(void);

	// デバッグ描画
	void DebugDraw(void);
};

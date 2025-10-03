#pragma once
#include <vector>
#include "../Base.h"

class SystemManager
{
public:

	// 感度
	static constexpr float SENSITIVITY = 0.003f;

	// 明示的にインステンスを生成する
	static void CreateInstance(void);

	// 静的インスタンスの取得
	static SystemManager& GetInstance(void);

	// 解放処理
	void Destroy();

	//リセット
	void ResetGame();

	// 感度を格納
	void SetSensitivity(const float num) { sensitivity = num; }

	// 感度を返す
	const float GetSensitivity() const { return sensitivity; }

	// スコアを格納
	void SetScore(const int score) { score_ = score; }

	// スコアを返す
	const int GetScore() const { return score_; }

	// 杖の種類を格納
	void SetGunType(const weapon_TYPE gunType) { gunType_ = gunType; }

	// 杖の種類を返す
	const weapon_TYPE GetWeaponType() const { return gunType_; }

private:

	// 静的インスタンス
	static SystemManager* instance_;

	// マウス感度
	float sensitivity;

	// スコア
	int score_;

	// 杖の種類
	weapon_TYPE gunType_;

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	SystemManager(void);
	// コピーコンストラクタも同様
	SystemManager(const SystemManager& manager) = default;
	// デストラクタも同様
	~SystemManager(void) = default;
};


#pragma once
#include <vector>
#include "../Base.h"
#include "../Object/Magic/MagicInfo.h"

class SystemManager
{
public:

	// マウス感度
	static constexpr float MOUSE_SENSITIVITY = 0.003f;

	// パッド感度
	static constexpr float PAD_SENSITIVITY = 0.03f;

	// 明示的にインステンスを生成する
	static void CreateInstance(void);

	// 静的インスタンスの取得
	static SystemManager& GetInstance(void);

	// 解放処理
	void Destroy();

	//リセット
	void ResetGame();

	// マウス感度を格納
	void SetMouseSensitivity(const float num) { MouseSensitivity_ = num; }

	// マウス感度を返す
	const float GetMouseSensitivity() const { return MouseSensitivity_; }

	// パッド感度を格納
	void SetPadSensitivity(const float num) { PadSensitivity_ = num; }

	// パッド感度を返す
	const float GetPadSensitivity() const { return PadSensitivity_; }

	// スコアを格納
	void SetScore(const int score) { score_ = score; }

	// スコアを返す
	const int GetScore() const { return score_; }

	// 魔法の種類を格納
	void SetTypeMagic(const TYPE_MAGIC type) { typeMagic_ = type; }

	// 魔法種類を返す
	const TYPE_MAGIC GetTypeMagic() const { return typeMagic_; }

private:

	// 静的インスタンス
	static SystemManager* instance_;

	// マウス感度
	float MouseSensitivity_;

	// パッド感度
	float PadSensitivity_;

	// スコア
	int score_;

	// 魔法の種類
	TYPE_MAGIC typeMagic_;

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	SystemManager(void);
	// コピーコンストラクタも同様
	SystemManager(const SystemManager& manager) = default;
	// デストラクタも同様
	~SystemManager(void) = default;
};


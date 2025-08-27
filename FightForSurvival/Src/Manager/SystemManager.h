#pragma once
#include <vector>

class SystemManager
{
public:

	// 明示的にインステンスを生成する
	static void CreateInstance(void);

	// 静的インスタンスの取得
	static SystemManager& GetInstance(void);

	// 解放処理
	void Destroy();

	//リセット
	void ResetGame();

	//人数を格納
	void SetSensitivity(const float num) { sensitivity = num; }

	//人数を返す
	const float GetSensitivity() const { return sensitivity; }

	//スコアを格納
	void SetScore(const int score) { score_ = score; }

	//スコアを返す
	const int GetScore() const { return score_; }

private:

	// 静的インスタンス
	static SystemManager* instance_;

	// マウス感度
	float sensitivity;

	//スコア
	int score_;

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	SystemManager(void);
	// コピーコンストラクタも同様
	SystemManager(const SystemManager& manager) = default;
	// デストラクタも同様
	~SystemManager(void) = default;
};


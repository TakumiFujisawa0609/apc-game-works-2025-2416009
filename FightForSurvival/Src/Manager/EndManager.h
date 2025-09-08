#pragma once

class EndManager
{
public:

	// 明示的にインステンスを生成する
	static void CreateInstance(void);

	// 静的インスタンスの取得
	static EndManager& GetInstance(void);

	// 初期化
	void Init(void);

	// 解放処理
	void Destroy(void);

	// ゲーム終了か
	void SetIsEnd(const bool flg) { isEnd_ = flg; }

	// ゲーム終了か
	bool GetIsEnd(void)const { return isEnd_; }

private:

	// 静的インスタンス
	static EndManager* instance_;

	// ゲーム終了か
	bool isEnd_;

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	EndManager(void);

	// コピーコンストラクタも同様
	EndManager(const EndManager& instance) = default;

	// デストラクタも同様
	~EndManager(void) = default;
};


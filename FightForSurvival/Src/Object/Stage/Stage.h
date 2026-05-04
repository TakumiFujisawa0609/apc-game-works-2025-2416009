#pragma once


class Stage
{
public:

	// モデルの大きさ
	static constexpr VECTOR SCALE = { 0.8f, 0.8f, 0.8f };

	// モデルの向き
	static constexpr VECTOR ROTATE = { 0.0f, 0.0f, 0.0f };

	// 初期位置
	static constexpr VECTOR DEFAULT_POS = { 0.0f, -120.0f, 0.0f };

	// コンストラクタ
	Stage(void);
	// デストラクタ
	~Stage(void);

	// 読み込み処理
	void Load(void);
	// 初期化処理
	void Init(void);
	// 更新処理
	void Update(void);
	// 描画処理
	void Draw(void);
	// 解放処理
	void Release(void);

	// モデルIdを渡す
	int GetModelId(void)const { return modelId_; }

	// 壁のモデルIdを渡す
	int GetWallModelId(void)const { return wallModelId_; }

private:

	int modelId_;
	int wallModelId_;
	int flowerModelId_;

	VECTOR scale_;
	VECTOR rotate_;
	VECTOR pos_;

};


#pragma once

class ObjectBase
{
public:

	// コンストラクタ
	ObjectBase(void);

	// デストラクタ
	virtual ~ObjectBase(void) = 0;

	// ロード処理
	virtual void Load(void) = 0;

	// 初期化処理
	virtual void Init(void) = 0;

	// 更新ステップ
	virtual void Update(void) = 0;

	// 描画処理
	virtual void Draw(void) = 0;

	// 解放処理
	virtual void Release(void) = 0;

};


#pragma once

#include "../../Common/Vector2/Vector2.h"
#include "../Base/SceneBase.h"

class SelectScene : public SceneBase
{
public:

	// 基準座標
	static constexpr int POS_X = 50;
	static constexpr int POS_Y = 50;

	// 当たり判定を行うサイズ
	static constexpr int COL_SIZE_X = 300;
	static constexpr int COL_SIZE_Y = 500;

	// 画像間(余白)の大きさ
	static constexpr int SPACE_X = COL_SIZE_X + 50;

	enum class STATE
	{
		STRAIGHT,
		FOLLOW,
		EXPLOSION,
		MAX,
	};

	// コンストラクタ
	SelectScene(void);

	// デストラクタ
	~SelectScene(void);

	void Load(void) override;
	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

	void ChangeState(STATE state);
private:

	STATE state_;

	// 選択されたアップグレードの表示座標
	Vector2 pos_[static_cast<int>(STATE::MAX)];

	// 確定させる
	void Confirm(void);

	// カーソルの選択処理
	void CursorSelect(void);

	// パッドの選択処理
	void PadSelect(void);
};

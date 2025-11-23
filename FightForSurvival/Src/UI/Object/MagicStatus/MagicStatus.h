#pragma once

#include "../UIBase.h"
#include "../../UIInfo.h"

// クラスの前方宣言
class TextureManager;
class Player;

// ベースを継承
class MagicStatus : public UIBase
{
public:

	static constexpr int STATUS_NUM = 11;
	static constexpr int BULLET_KIND_NUM = 3;

	// コンストラクタ
	MagicStatus(const UIResourceInfo& info, TextureManager* texMgr);

	// デストラクタ
	~MagicStatus();

	// 情報のアドレスをセット
	void SetPlayer(Player* player);

	void Update(void) override;		// 更新
	void Draw(void) override;		// 描画

private:
	int statusHandle_[STATUS_NUM];
	int bulletKindHandle_[BULLET_KIND_NUM];

	Player* player_;
};


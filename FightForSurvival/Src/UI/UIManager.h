#pragma once

#include <vector>

#include "Factory/UIFactory.h"

class UIManager
{
public:
	UIManager();	// コンストラクタ
	~UIManager();	// デストラクタ

	// UIをリストに追加する
	void AddUI(UIBase* ui);

	void Update(void);	// 更新
	void Draw(void);	// 描画
	void Delete(void);	// 削除

	std::vector<UIBase*> GetUIList(void)const { return uiList_; }

private:
	// UIのリスト
	std::vector<UIBase*> uiList_;
};

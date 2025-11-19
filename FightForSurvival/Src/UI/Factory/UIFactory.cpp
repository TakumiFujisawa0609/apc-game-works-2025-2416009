#include "UIFactory.h"

#include "../UIResourceTable.h"
#include "../TextrueManager/TextureManager.h"

UIFactory* UIFactory::instance_ = nullptr;

UIFactory::UIFactory()
{
}

UIFactory::~UIFactory()
{
}

void UIFactory::Register(const UI_KIND& kind, std::function<UIBase* (const UIResourceInfo&, TextureManager*)> func)
{
	// UIの生成情報を登録しておく
	createTable_[kind] = func;
}

UIBase* UIFactory::CreateUI(const UI_KIND& kind, TextureManager* texMgr)
{
	// UI種別でテーブルを参照して生成するUIを検索
	auto it = createTable_.find(kind);
	if (it == createTable_.end()) 
		return nullptr;
	
	// UI種別でテーブルを参照してリソースを検索
	auto res = UIResourceTable.find(kind);
	if (res == UIResourceTable.end())
		return nullptr;

	// 登録されている情報からUIを生成する
	return it->second(res->second, texMgr);
}

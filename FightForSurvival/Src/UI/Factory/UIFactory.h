#pragma once

#include <map>
#include <functional>

#include "../UIInfo.h"
#include "../Object/UIBase.h"

// クラスの前方宣言
class TextureManager;

class UIFactory
{
public:
	// シングルトン（生成・取得・削除）
	static void CreateInstance(void) { if (instance_ == nullptr) { instance_ = new UIFactory(); } }
	static UIFactory* GetInstance(void) { return instance_; }
	static void DeleteInstance(void) { if (instance_ != nullptr) { delete instance_; instance_ = nullptr; } }

private:
	// 静的インスタンス
	static UIFactory* instance_;

	UIFactory();	// コンストラクタ
	~UIFactory();	// デストラクタ

	// コピー・ムーブ操作を禁止
	UIFactory(const UIFactory&) = delete;
	UIFactory& operator=(const UIFactory&) = delete;
	UIFactory(UIFactory&&) = delete;
	UIFactory& operator=(UIFactory&&) = delete;

public:
	// 生成処理 (functionでコンストラクタを呼ぶ)
	void Register(const UI_KIND& kind, std::function<UIBase* (const UIResourceInfo&, TextureManager*)> func);

	// UIを作成
	UIBase* CreateUI(const UI_KIND& kind, TextureManager* texMgr);

private:
	// UIテーブル
	std::map<UI_KIND, std::function<UIBase* (const UIResourceInfo&, TextureManager*)>> createTable_;
};

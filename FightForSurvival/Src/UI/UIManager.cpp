#include "UIManager.h"

UIManager::UIManager()
{
}

UIManager::~UIManager()
{
	Delete();
}

void UIManager::AddUI(UIBase* ui)
{
	// UIをリストに追加
	uiList_.push_back(ui);
}

void UIManager::Update(void)
{
	// リストが空だった何もしない
	if (uiList_.empty()) return;

	// 全てのUIを更新
	for (auto ui : uiList_)
	{
		// 更新
		ui->Update();
	}
}

void UIManager::Draw(void)
{
	// リストが空だった何もしない
	if (uiList_.empty()) return;
	
	// 全てのUIを更新
	for (auto ui : uiList_)
	{
		// 描画
		ui->Draw();
	}
}

void UIManager::Delete(void)
{
	// リストが空だった何もしない
	if (uiList_.empty()) return;

	// 全てのUIを削除
	for (auto ui : uiList_)
	{
		// 削除
		delete ui;
	}

	// リストをクリア
	uiList_.clear();
}

#include "TextureManager.h"

#include <DxLib.h>

TextureManager::TextureManager()
{
}

TextureManager::~TextureManager()
{
	// 解放
	DeleteAll();
}

int TextureManager::LoadTexture(const std::string& path)
{
	// パスが無いなら -1 を返す
	if (path.empty()) return -1;

	// パスからテクスチャリストを検索
	auto it = textures_.find(path);

	// 既にロード済みなのでそのまま返す
	if (it != textures_.end())
		return it->second;

	// 画像をロードする
	int handle = LoadGraph(path.c_str());

	// リストに登録する
	textures_[path] = handle;

	// 画像ハンドルを返す
	return handle;
}

void TextureManager::DeleteAll(void)
{
	// 読み込んだ全ての画像を検索
	for (auto& texture : textures_)
	{
		// 削除
		DeleteGraph(texture.second);
	}
	
	// 配列をクリア
	textures_.clear();
}

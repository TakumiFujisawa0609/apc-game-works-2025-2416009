#pragma once

#include <string>
#include <map>

class TextureManager
{
public:
	TextureManager();	// コンストラクタ
	~TextureManager();	// デストラクタ

	// テクスチャのロード関数
	int LoadTexture(const std::string& path);

	// 指定したパスの画像ハンドルを返す
	int GetPath(const std::string& path) const { return textures_.at(path); }

	// 読み込んだテクスチャを削除する
	void DeleteAll(void);

private:
	// テスクチャのマップ管理
	std::map<std::string, int> textures_;
};

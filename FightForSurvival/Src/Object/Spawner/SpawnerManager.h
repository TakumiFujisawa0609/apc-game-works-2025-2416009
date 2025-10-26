#pragma once

#include <vector>
#include <DxLib.h>

class Spawner;

class SpawnerManager
{
public:

	// 明示的にインステンスを生成する
	static void CreateInstance(void);

	// 静的インスタンスの取得
	static SpawnerManager& GetInstance(void);

	// ロード処理
	void Load(void);
	// 更新処理
	void Update(void);
	// 描画処理
	void Draw(void);
	// 解放処理
	void Destroy(void);

	// スポナーの設置を行う
	void SpawnerInstallation(VECTOR pos,float interval);

private:

	// スポナーのモデルId
	int modelId_;

	// スポーンマネージャーのインスタンス
	static SpawnerManager* instance_;

	// スポナーのインスタンス
	std::vector<Spawner*> spawners_;

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	SpawnerManager(void);
	// コピーコンストラクタも同様
	SpawnerManager(const SpawnerManager& manager) = default;
	// デストラクタも同様
	~SpawnerManager(void) = default;

	// スポナーを追加する
	void AddSpawner(Spawner* spawner);
	// 未使用のスポナーを再利用する。未使用のものが無ければ生成を行う
	Spawner* GetValidSpawner(void);

};


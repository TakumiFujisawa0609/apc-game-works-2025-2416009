#pragma once

#include <vector>
#include <memory>
#include "EnemyBase.h"

class EnemyManager
{
public:
    // シングルトン（生成・取得・削除）
    static void CreateInstance(void) { if (instance_ == nullptr) { instance_ = new EnemyManager(); } };
    static EnemyManager* GetInstance(void) { return instance_; };
    static void DeleteInstance(void) { if (instance_ != nullptr) { delete instance_; instance_ = nullptr; } }

    // 敵の追加
    void AddEnemy(std::unique_ptr<EnemyBase> enemy);

    void Update();  // 更新
    void Draw();    // 描画
    void Delete();  // 削除

    const std::vector<std::unique_ptr<EnemyBase>>& GetEnemy(void)const { return enemys_; }
private:
    // 静的インスタンス
    static EnemyManager* instance_;

    // デフォルトコンストラクタをprivateにして、外部から生成できない様にする
    EnemyManager(void);
    // デストラクタも同様
    ~EnemyManager(void);

    // コピー・ムーブ操作を禁止
    EnemyManager(const EnemyManager&) = delete;
    EnemyManager& operator=(const EnemyManager&) = delete;
    EnemyManager(EnemyManager&&) = delete;
    EnemyManager& operator=(EnemyManager&&) = delete;

    // 下記をコンパイルエラーさせるため 上記を追加
    // EnemyManager copy = *EnemyManager::GetInstance();
    // EnemyManager copied(*EznemyManager::GetInstance());
    // EnemyManager moved = std::move(*EnemyManager::GetInstance());
    
    // 全てのエネミーを管理する
    std::vector<std::unique_ptr<EnemyBase>> enemys_;

	//void ChangeWave(WAVE wave);
	//void UpdateWave01(void);
	//void UpdateWave02(void);
	////void UpdateWave03(void);
	//void UpdateEnd(void);
};


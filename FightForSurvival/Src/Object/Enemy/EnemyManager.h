#pragma once

#include <vector>
#include <memory>

#include "Base/EnemyBase.h"
#include "../Player/Player.h"

// 敵のタイプ(エネミーのマネージャー等で作る！）
enum class ENEMY_TYPE
{
    ZOMBIE,
    BAT,
    DRAGON,
    MAX,
};

class EnemyManager
{
public:

    // ゾンビを出現させることができる最大数
    static constexpr int ZOMBIE_NUM = 50;

    // コウモリを出現させることができる最大数
    static constexpr int BAT_NUM = 50;

    // シングルトン（生成・取得・削除）
    static void CreateInstance(void) { if (instance_ == nullptr) { instance_ = new EnemyManager(); } };
    static EnemyManager& GetInstance(void) { return *instance_; };
    static void DeleteInstance(void) { if (instance_ != nullptr) { delete instance_; instance_ = nullptr; } }

    // 敵の追加
    void AddEnemy(EnemyBase* enemy);

    void Load(void);  // 更新
    void Update(void);  // 更新
    void Draw(void);    // 描画
    void Delete(void);  // 削除

    void Spawn(ENEMY_TYPE type, VECTOR pos);

    const std::vector<EnemyBase*> GetEnemy(void)const { return enemies_; }

    void GetPlayerPoint(Player* player) { player_ = player; }

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
    // 敵モデルのの読み込み

    // プレイヤーのポインター
    Player* player_;

    // 全てのエネミーを管理する
    std::vector<EnemyBase*> enemies_;

    // エネミー用のモデルハンドルID
    std::vector<int> enemyModelIds_;

    // ゾンビ用のアニメーションモデルハンドルID
    std::vector<int> zombieAnimModelIds_;

    // 未使用の敵を再利用する。未使用のものが無ければ生成を行う
    EnemyBase* GetValidEnemy(ENEMY_TYPE type);
};
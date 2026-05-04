#pragma once

#include <vector>
#include <memory>

class EnemyBase;
class MagicBase;
#include "../Player/Player.h"
#include "../Magic/MagicInfo.h"

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

    // 読み込み処理
    void Load(void);
    // 更新処理
    void Update(void);
    // 描画処理
    void Draw(void);
    // 解放処理
    void Delete(void);

    // 倒されて使われていない敵がいたら再利用する
    void Spawn(ENEMY_TYPE type, VECTOR pos);

    // 敵全てを渡す
    const std::vector<EnemyBase*> GetEnemy(void)const { return enemies_; }

    // プレイヤーのポインタを取得
    void SetPlayerPoint(Player* player) { player_ = player; }

    // 魔法の取得
    std::vector<MagicBase*> GetMagics(void)const { return magics_; }

    // 未使用の魔法を再利用する。未使用のものが無ければ生成を行う
    MagicBase* GetValidMagic(TYPE_MAGIC type);
    // 前方に魔法生成処理
    void CreateMagic(ENEMY_TYPE type, VECTOR pos ,VECTOR dir);
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

    // プレイヤーのポインター
    Player* player_;

    // 全てのエネミーを管理する
    std::vector<EnemyBase*> enemies_;

    // エネミー用のモデルハンドルID
    std::vector<int> enemyModelIds_;

    // エネミー全体の魔法を管理する
    std::vector<MagicBase*> magics_;

    // エフェクト用モデルハンドルID
    int baseAttackEffectModelId_;

    // 未使用の敵を再利用する。未使用のものが無ければ生成を行う
    EnemyBase* GetValidEnemy(ENEMY_TYPE type);

    // 魔法の更新
    void UpdateMagic(void);
    // 魔法の描画
    void DrawMagic(void);
};
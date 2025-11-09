#include <DxLib.h>
#include "../../Application.h"
#include "../../Scene/SceneManager.h"
#include "Zombie/Zombie.h"
#include "Bat/Bat.h"
#include "Dragon/Dragon.h"
#include "../Magic/Base/MagicBase.h"
#include "../Magic/BatMagic/BatMagic.h"
#include "../Magic/DragonMagic/DragonMagic.h"
#include "EnemyManager.h"

EnemyManager* EnemyManager::instance_ = nullptr;


EnemyManager::EnemyManager(void) {}
EnemyManager::~EnemyManager(void) {}

void EnemyManager::AddEnemy(EnemyBase* enemy)
{
	enemies_.emplace_back(std::move(enemy));
}

void EnemyManager::CraateMagic(ENEMY_TYPE type, VECTOR pos, VECTOR dir)
{
	MagicBase* magic = nullptr;

	// 有効な魔法を取得する
	switch (type)
	{
	case ENEMY_TYPE::BAT:
		magic = GetValidMagic(TYPE_MAGIC::BAT_MAGIC);
		break;
	case ENEMY_TYPE::DRAGON:
		break;
	default:
		break;
	}

	if (magic == nullptr)
	{
		// 中身がnullptrだったら処理を行わない
		return;
	}

	// 初期化処理
	magic->Init();

	if (ENEMY_TYPE::BAT == type)
	{
		// 座標を更新する
		magic->CreateShot(pos, dir);
	}
}

void EnemyManager::Load(void)
{
	// 敵へのパス
	std::string enePas = Application::PATH_MODEL + "Enemy/";

	// エネミーモデルのロード
	enemyModelIds_.emplace_back(MV1LoadModel((enePas + "Zombie.mv1").c_str()));
	enemyModelIds_.emplace_back(MV1LoadModel((enePas + "Bat.mv1").c_str()));
	enemyModelIds_.emplace_back(MV1LoadModel((enePas + "Dragon.mv1").c_str()));

	// ボスのメモリ確保
	Dragon* enemy = new Dragon(ENEMY_TYPE::DRAGON, enemyModelIds_[static_cast<int>(ENEMY_TYPE::DRAGON)], zombieAnimModelIds_, player_);
	AddEnemy(enemy);

	// 空のVector型を渡すためにアニメーションのロードの前にコウモリのメモリ確保を行う
	for (int i = 0; i < BAT_NUM; i++)
	{
		// 先にメモリ確保しておく(ゲーム途中にnewを行わないようにする)
		Bat* enemy = new Bat(ENEMY_TYPE::BAT, enemyModelIds_[static_cast<int>(ENEMY_TYPE::BAT)],zombieAnimModelIds_, player_);
		AddEnemy(enemy);
	}

	// アニメーションのロード
	zombieAnimModelIds_.emplace_back(MV1LoadModel((enePas + "Zombie Idle.mv1").c_str()));
	zombieAnimModelIds_.emplace_back(MV1LoadModel((enePas + "Zombie Running.mv1").c_str()));
	zombieAnimModelIds_.emplace_back(MV1LoadModel((enePas + "Standing Melee Attack Downward.mv1").c_str()));
	zombieAnimModelIds_.emplace_back(MV1LoadModel((enePas + "Standing Walk Back.mv1").c_str()));
	zombieAnimModelIds_.emplace_back(MV1LoadModel((enePas + "Zombie HitIdle.mv1").c_str()));
	zombieAnimModelIds_.emplace_back(MV1LoadModel((enePas + "Zombie Dying.mv1").c_str()));

	for (int i = 0; i < ZOMBIE_NUM; i++)
	{
		// 先にメモリ確保しておく(ゲーム途中にnewを行わないようにする)
		Zombie* enemy = new Zombie(ENEMY_TYPE::ZOMBIE, enemyModelIds_[static_cast<int>(ENEMY_TYPE::ZOMBIE)], zombieAnimModelIds_, player_);
		AddEnemy(enemy);
	}

	//// 攻撃エフェクト用のモデルのロード
	//attackEffectModelIds_.emplace_back(
	//	MV1LoadModel(
	//		(Application::PATH_MODEL + "Effect/Fireball/Fireball.mv1").c_str()));
	//attackEffectModelIds_.emplace_back(
	//	MV1LoadModel(
	//		(Application::PATH_MODEL + "Effect/Rockfall/Rock.mv1").c_str()));
}

void EnemyManager::Update(void)
{
	// エネミーの更新
	for (auto& enemy : enemies_)
	{
		enemy->Update();
	}

	// 魔法の更新
	UpdateMagic();
}

void EnemyManager::Draw(void)
{
	DrawFormatString(0, 200, 0xffffff, "敵の総数 = %d", enemies_.size());

	for (auto& enemy : enemies_)
	{
		enemy->Draw();
	}

	// 魔法の描画
	DrawMagic();
}

void EnemyManager::Delete(void)
{
	// Enemyクラスのメモリ解放
	for (auto& enemy : enemies_)
	{
		enemy->Release();
	}

	//// ベースとなるモデルのメモリ解放
	//for (int id : enemyModelIds_)
	//{
	//	MV1DeleteModel(id);
	//}

	enemies_.clear();

	// エネミーモデルの解放を追加
	for (auto& modelId : enemyModelIds_)
	{
		MV1DeleteModel(modelId);
	}

	enemyModelIds_.clear();
	zombieAnimModelIds_.clear();

	// 中にデータが入っていたら解放する
	MV1DeleteModel(baseAttackEffectModelId_);
}

void EnemyManager::Spawn(ENEMY_TYPE type, VECTOR pos)
{
	// 有効な敵を取得する
	EnemyBase* enemy = GetValidEnemy(type);

	if (enemy == nullptr)
	{
		// 使われていない敵がいなかったら復活処理は行わない
		return;
	}

	// 敵の初期化
	enemy->CreateEnemy(pos);
}

EnemyBase* EnemyManager::GetValidEnemy(ENEMY_TYPE type)
{
	auto& ins = EnemyManager::GetInstance();
	auto& enemies_ = ins.GetEnemy();

	size_t size = enemies_.size();

	for (int i = 0; i < size; i++)
	{
		// 敵の種類が違ったら次の敵を見る
		if (enemies_[i]->GetType() != type)
		{
			continue;
		}

		// 魔法の種別が同じ、かつ、未使用(生存していない)なら再利用する
		if (!enemies_[i]->GetEnemy().isAlive_)
		{
			return enemies_[i];
		}
	}

	return nullptr;
}

MagicBase* EnemyManager::GetValidMagic(TYPE_MAGIC type)
{
	size_t size = magics_.size();

	for (int i = 0; i < size; i++)
	{
		if (magics_[i]->GetTypeMagic() != type)
		{
			// 種類が違ったら、次の魔法を見る
			continue;
		}

		// 未使用(生存していない)で、かつ、魔法の種別が同じ
		if (!magics_[i]->GetMagic().isExists_ && !magics_[i]->GetMagic().isDraw_)
		{
			return magics_[i];
		}
	}

	// 未使用の魔法がなかった場合新しい魔法を生成
	MagicBase* magic;

	// 新しい魔法のインスタンスを生成する
	switch (type)
	{
	case TYPE_MAGIC::BAT_MAGIC:
		magic = new BatMagic(TYPE_MAGIC::BAT_MAGIC, -1);
		break;
	case TYPE_MAGIC::DRAGON_MAGIC:
		magic = new DragonMagic(TYPE_MAGIC::DRAGON_MAGIC, -1);
		break;
	default:
		break;
	}

	// 可変長配列に追加
	magics_.push_back(magic);

	return magic;
}

void EnemyManager::UpdateMagic(void)
{
	// 魔法の更新
	for (auto& magic : magics_)
	{
		if (magic->GetMagic().isExists_)
		{
			magic->Update();
		}
	}
}

void EnemyManager::DrawMagic(void)
{
	// 魔法の描画
	for (auto& magic : magics_)
	{
		magic->Draw();
	}
}

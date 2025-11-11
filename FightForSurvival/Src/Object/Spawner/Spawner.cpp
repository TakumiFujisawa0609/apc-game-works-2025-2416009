#include "Spawner.h"

#include "../../Scene/SceneManager.h"
#include "../Enemy/EnemyManager.h"

Spawner::Spawner(int modelId)
{
	if (modelId != -1)
	{
		// 中身が-1じゃなかったら(中身が入っていたら)モデルのロードを行う
		spawner_.modelId_ = MV1DuplicateModel(modelId);
	}
}

Spawner::~Spawner(void)
{
}

void Spawner::Create(VECTOR pos, float interval)
{
	// 座標取得(この座標が軸となる)
	spawner_.basePos_ = pos;
	// 軸座標を基に座標を設定
	PositionInit();
	
	// スポーン間隔を取得
	spawner_.spawnInterval_ = interval;

	// 存在フラグ初期化
	spawner_.isExists_ = true;

	// 時間初期化
	spawner_.time_ = 0.0f;

	// 当たり判定用半径初期化
	spawner_.collisionRadius_ = COLLISION_RADIUS;

	// 耐久力の初期化
	spawner_.durability_ = DURABILITY;

	// 最初の出現パターンを決める
	SelectPattern();
}

void Spawner::Update(void)
{
	// 存在していなかったら、処理を行わない
	if (!spawner_.isExists_)
	{
		return;
	}

	// 時間を進める
	spawner_.time_ += SceneManager::GetInstance().GetDeltaTime();

	// 出現時間になったら
	if (spawner_.time_ >= spawner_.spawnInterval_)
	{
		// 時間を初期化
		spawner_.time_ = 0.0f;

		for (int i = 0; i < SPAWN_ENEMY_NUM; i++)
		{
			// 敵をスポーンさせる
			EnemyManager::GetInstance().Spawn(spawner_.eneType_[i], spawner_.pos_[i]);
		}

		// パターンを変更する
		SelectPattern();
	}

}

void Spawner::Draw(void)
{
	if (!spawner_.isExists_)
	{
		return;
	}

	// どこが中心位置か分かるようにデバック表示
	DrawSphere3D(spawner_.basePos_, spawner_.collisionRadius_, 100, 0xffff00, 0xffff00, false);

}

void Spawner::Release(void)
{
	// モデルの解放
	MV1DeleteModel(spawner_.modelId_);
}

void Spawner::Damage(float durability)
{
	spawner_.durability_ -= durability;

	if (spawner_.durability_ <= 0.0f)
	{
		spawner_.durability_ = 0.0f;
		// 攻撃を受けて耐久力が無くなったら、存在をなくす
		spawner_.isExists_ = false;
	}
}

void Spawner::SelectPattern(void)
{
	// ランダムで決める
	spawner_.pattern_ = static_cast<PATTERN>(GetRand(2));

	// パターンを設定する
	PatternInsInit(spawner_.pattern_);
}

void Spawner::PatternInsInit(PATTERN pattern)
{
	switch (pattern)
	{
	case Spawner::PATTERN::PATTERN_1:

		for (int i = 0; i < SPAWN_ENEMY_NUM; i++)
		{
			spawner_.eneType_[i] = ENEMY_TYPE::ZOMBIE;
		}

		break;
	case Spawner::PATTERN::PATTERN_2:

		for (int i = 0; i < SPAWN_ENEMY_NUM; i++)
		{
			spawner_.eneType_[i] = ENEMY_TYPE::BAT;
		}

		break;
	case Spawner::PATTERN::PATTERN_3:

		spawner_.eneType_[0] = ENEMY_TYPE::ZOMBIE;
		spawner_.eneType_[1] = ENEMY_TYPE::ZOMBIE;
		spawner_.eneType_[2] = ENEMY_TYPE::BAT;
		spawner_.eneType_[3] = ENEMY_TYPE::BAT;

		break;
	default:
		break;
	}
}

void Spawner::PositionInit(void)
{
	// 基軸のpos_[1]から他の座標も設定する
	SetPosition(1, LEFT_UP);
	SetPosition(2, LEFT_DOWN);
	SetPosition(3, RIGHT_UP);
	SetPosition(4, RIGHT_DOWN);
}

void Spawner::SetPosition(int i, VECTOR offset)
{
	spawner_.pos_[i] = VAdd(spawner_.basePos_, offset);
}

#include "Spawner.h"

#include "../../../Manager/SceneManager.h"

Spawner::Spawner(void)
{
}

Spawner::~Spawner(void)
{
}

void Spawner::Create(VECTOR pos, float interval)
{
	// 座標取得(この座標が軸となる)
	pos_[0] = pos;
	// 軸座標を基に座標を設定
	PositionInit();
	
	// スポーン間隔を取得
	spawnInterval_ = interval;

	// 存在フラグ初期化
	isExists_ = true;

	// 時間初期化
	time_ = 0.0f;

	// 最初の出現パターンを決める
	SelectPattern();
}

void Spawner::Update(void)
{
	// 存在していなかったら、処理を行わない
	if (!isExists_)
	{
		return;
	}

	// 時間を進める
	time_ += SceneManager::GetInstance().GetDeltaTime();

	// 出現時間になったら
	if (time_ >= spawnInterval_)
	{
		// 時間を初期化
		time_ = 0.0f;

		for (int i = 0; i < SPAWN_ENEMY_NUM; i++)
		{
			// 敵をスポーンさせる
			EnemyManager::GetInstance().Spawn(eneType_[i], pos_[i]);
		}

		// パターンを変更する
		SelectPattern();
	}

}

void Spawner::Draw(void)
{
	if (!isExists_)
	{
		return;
	}
}

void Spawner::Release(void)
{
}

void Spawner::SelectPattern(void)
{
	pattern_ = PATTERN::PATTERN_1;

	// ランダムで決める
}

void Spawner::PatternInsInit(PATTERN pattern)
{
	switch (pattern)
	{
	case Spawner::PATTERN::PATTERN_1:

		eneType_[0] = ENEMY_TYPE::ZOMBIE;
		eneType_[1] = ENEMY_TYPE::ZOMBIE;
		eneType_[2] = ENEMY_TYPE::ZOMBIE;
		eneType_[3] = ENEMY_TYPE::ZOMBIE;
		eneType_[4] = ENEMY_TYPE::ZOMBIE;

		break;
	case Spawner::PATTERN::PATTERN_2:
		break;
	case Spawner::PATTERN::PATTERN_3:
		break;
	default:
		break;
	}
}

void Spawner::PositionInit(void)
{
	for (int i = 1; i < SPAWN_ENEMY_NUM; i++)
	{
		pos_[i] = pos_[0];
	}

	pos_[1] = VAdd(pos_[1], LEFT_UP);
	pos_[2] = VAdd(pos_[2], LEFT_DOWN);
	pos_[3] = VAdd(pos_[3], RIGHT_UP);
	pos_[4] = VAdd(pos_[4], RIGHT_DOWN);
}

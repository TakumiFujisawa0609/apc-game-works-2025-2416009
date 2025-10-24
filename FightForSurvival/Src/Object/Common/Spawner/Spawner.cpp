#include "Spawner.h"

#include "../../../Manager/SceneManager.h"
#include "../../Enemy/EnemyManager.h"

Spawner::Spawner(int modelId)
{
	// モデルのロード
	spawner_->modelId_ = MV1DuplicateModel(modelId);
}

Spawner::~Spawner(void)
{
}

void Spawner::Create(VECTOR pos, float interval)
{
	// 座標取得(この座標が軸となる)
	spawner_->pos_[0] = pos;
	// 軸座標を基に座標を設定
	PositionInit();
	
	// スポーン間隔を取得
	spawner_->spawnInterval_ = interval;

	// 存在フラグ初期化
	spawner_->isExists_ = true;

	// 時間初期化
	spawner_->time_ = 0.0f;

	// 最初の出現パターンを決める
	SelectPattern();
}

void Spawner::Update(void)
{
	// 存在していなかったら、処理を行わない
	if (!spawner_->isExists_)
	{
		return;
	}

	// 時間を進める
	spawner_->time_ += SceneManager::GetInstance().GetDeltaTime();

	// 出現時間になったら
	if (spawner_->time_ >= spawner_->spawnInterval_)
	{
		// 時間を初期化
		spawner_->time_ = 0.0f;

		for (int i = 0; i < SPAWN_ENEMY_NUM; i++)
		{
			// 敵をスポーンさせる
			EnemyManager::GetInstance().Spawn(spawner_->eneType_[i], spawner_->pos_[i]);
		}

		// パターンを変更する
		SelectPattern();
	}

}

void Spawner::Draw(void)
{
	if (!spawner_->isExists_)
	{
		return;
	}

#ifdef _DEBUG

	// どこが中心位置か分かるようにデバック表示
	DrawSphere3D(spawner_->pos_[1], 40.0f, 100, 0xffff00, 0xffff00, false);

#endif // _DEBUG

}

void Spawner::Release(void)
{
	// モデルの解放
	MV1DeleteModel(spawner_->modelId_);
}

void Spawner::SelectPattern(void)
{
	// ランダムで決める
	spawner_->pattern_ = PATTERN::PATTERN_1;

	// パターンを設定する
	PatternInsInit(spawner_->pattern_);
}

void Spawner::PatternInsInit(PATTERN pattern)
{
	switch (pattern)
	{
	case Spawner::PATTERN::PATTERN_1:

		for (int i = 0; i < SPAWN_ENEMY_NUM; i++)
		{
			spawner_->eneType_[i] = ENEMY_TYPE::ZOMBIE;
		}

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
	// 基軸のpos_[1]から他の座標も設定する
	SetPosition(1, LEFT_UP);
	SetPosition(2, LEFT_DOWN);
	SetPosition(3, RIGHT_UP);
	SetPosition(3, RIGHT_DOWN);
}

void Spawner::SetPosition(int i, VECTOR offset)
{
	spawner_->pos_[i] = VAdd(spawner_->pos_[1], offset);
}

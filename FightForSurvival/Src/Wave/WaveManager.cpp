//#include "../Object/Enemy/EnemyManager.h"
#include "WaveBase.h"
#include "Wave1.h"
#include "../Application.h"
#include "WaveManager.h"

WaveManager::WaveManager(Player* player)
{
	player_ = player;

	wave1_ = nullptr;
	wave2_ = nullptr;

	// テーブルに関数のポインタを格納
	waveTable_[WAVE1][UPDATE] = Wave1Update;
	waveTable_[WAVE2][UPDATE] = Wave2Update;
	waveTable_[SELECT][UPDATE] = SelectUpdate;
	waveTable_[WAVE_END][UPDATE] = EndUpdate;

	waveTable_[WAVE1][DRAW] = Wave1Draw;
	waveTable_[WAVE2][DRAW] = Wave2Draw;
	waveTable_[SELECT][DRAW] = SelectDraw;
	waveTable_[WAVE_END][DRAW] = EndDraw;

}

WaveManager::~WaveManager(void)
{
}

void WaveManager::Init(void)
{
	wave1_ = new Wave1(this);
	wave1_->Init();

	//wave2_ = new Wave2(this);
	//wave2_->Init();

	// ウェーブの初期化
	ChangeWave(WAVE1);
}

void WaveManager::Load(void)
{
	// 敵モデルのの読み込み
	enemyModelIds_.emplace_back(MV1LoadModel((Application::PATH_MODEL + "Enemy/Zombie.mv1").c_str()));

	wave1_->Load();
	//wave2_->Load();
}

void WaveManager::Update(void)
{
	// nullチェック
	if (waveTable_[wave_][UPDATE])
	{
		waveTable_[wave_][UPDATE](*this);
	}
}

void WaveManager::Draw(void)
{
	// nullチェック
	if (waveTable_[wave_][DRAW])
	{
		waveTable_[wave_][DRAW](*this);
	}
}

void WaveManager::Release(void)
{
	// ウェーブインスタンスの解放
	if (wave1_ != nullptr)
	{
		wave1_->Release();
		delete wave1_;
		wave1_ = nullptr;
	}

	//if (wave2_ != nullptr)
	//{
	//	wave2_->Release();
	//	delete wave2_;
	//	wave2_ = nullptr;
	//}

	// エネミーモデルの解放を追加
	for (int modelId : enemyModelIds_)
	{
		MV1DeleteModel(modelId);
	}

	enemyModelIds_.clear();

}

void WaveManager::ChangeWave(WAVE wave)
{
	wave_ = wave;
}

void WaveManager::Wave1Update(WaveManager& wave)
{
	wave.wave1_->Update();

	// もし全ての敵を生成し終わってEND状態なら
	if (wave.wave1_->GetWaveState() == WAVE_STATE::END)
	{
		// プレイヤー強化タイムに入る
		wave.ChangeWave(SELECT);
	}
}

void WaveManager::Wave2Update(WaveManager& wave)
{
	//wave.wave2_->Update();

	//if (wave.wave2_->GetWaveState() == WAVE_STATE::END)
	//{
	//	wave.ChangeWave(SELECT);
	//}
}

void WaveManager::SelectUpdate(WaveManager& wave)
{
	wave.ChangeWave(WAVE_END);
}

void WaveManager::EndUpdate(WaveManager& wave)
{
}

void WaveManager::Wave1Draw(WaveManager& wave)
{
	wave.wave1_->Draw();
}

void WaveManager::Wave2Draw(WaveManager& wave)
{
	//wave.wave2_->Draw();
}

void WaveManager::SelectDraw(WaveManager& wave)
{
}

void WaveManager::EndDraw(WaveManager& wave)
{
}

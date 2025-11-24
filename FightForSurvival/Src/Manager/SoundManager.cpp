#include "SoundManager.h"
#include "../Application.h"
#include <DxLib.h>

SoundManager* SoundManager::instance_ = nullptr;

void SoundManager::Load(void)
{
	// BGM/SEの情報を追加
	AddSound();

	// Addで追加した分のロードを行う
	for (int i = 0; i < bgm_.fileName_.size(); i++)
	{
		bgm_.handle_.emplace_back(LoadSoundMem(bgm_.fileName_[i].c_str()));

		if (bgm_.handle_[i] != -1)
		{
			ChangeVolumeSoundMem(255 * bgm_.volume_[i] / 100, bgm_.handle_[i]);
		}
	}

	for (int i = 0; i < se_.fileName_.size(); i++)
	{
		se_.handle_.emplace_back(LoadSoundMem(se_.fileName_[i].c_str()));

		if (se_.handle_[i] != -1)
		{
			ChangeVolumeSoundMem(255 * se_.volume_[i] / 100, se_.handle_[i]);
		}
	}
}

void SoundManager::Init(void)
{
}

void SoundManager::Delete(void)
{

	for (int i = 0; i < bgm_.handle_.size(); i++)
	{
		DeleteSoundMem(bgm_.handle_[i]);
	}

	bgm_.handle_.clear();
	bgm_.fileName_.clear();
	bgm_.volume_.clear();


	for (int i = 0; i < se_.handle_.size(); i++)
	{
		DeleteSoundMem(se_.handle_[i]);
	}

	se_.handle_.clear();
	se_.fileName_.clear();
	se_.volume_.clear();

	delete instance_;
}

void SoundManager::Play(BGM bgm, bool flg)
{
	int handle = bgm_.handle_[static_cast<int>(bgm)];

	// 指定のハンドルが再生中か確認
	if (CheckSoundMem(handle) == 0)
	{
		PlaySoundMem(handle, DX_PLAYTYPE_LOOP, flg);
	}
}

void SoundManager::Play(SE se)
{
	int handle = se_.handle_[static_cast<int>(se)];

	PlaySoundMem(handle, DX_PLAYTYPE_BACK);
}

void SoundManager::Stop(BGM bgm)
{
	StopSoundMem(bgm_.handle_[static_cast<int>(bgm)]);
}

void SoundManager::Stop(SE se)
{
	StopSoundMem(se_.handle_[static_cast<int>(se)]);
}

SoundManager::SoundManager(void)
{
}

SoundManager::SoundManager(const SoundManager& manager)
{
}

SoundManager::~SoundManager(void)
{
}

void SoundManager::AddSound(void)
{
	std::string bgmPas = Application::PATH_BGM;
	// タイトル
	std::string pas = bgmPas + "AS_20267_オーケストラオープニングファンタジー.mp3";
	AddBGM(pas.c_str(),50);
	// 選択
	pas = bgmPas + "AS_152442_緩めなオケによる出撃・待機・メニュー曲.mp3";
	AddBGM(pas.c_str(),40);
	// ゲーム
	pas = bgmPas + "AS_1153557_爽快感のある激しい戦闘曲的オーケストラ.mp3";
	AddBGM(pas.c_str(),40);
	// クリア
	pas = bgmPas + "AS_22319_43秒／表彰式、クリアに合うオーケストラ.mp3";
	AddBGM(pas.c_str(),50);
	// オーバー
	pas = bgmPas + "AS_1128687_ゲームオーバー用の悲しげな曲.mp3";
	AddBGM(pas.c_str(),50);

	std::string sePas = Application::PATH_SE;
	// 決定
	pas = sePas + "AS_890907_決定／クリック／選択音（ピコンッ）.mp3";
	AddSE(pas.c_str(), 60);
	// ポーズモード
	pas = sePas + "AS_880033_決定／クリック／選択音（ピッ）.mp3";
	AddSE(pas.c_str(), 60);
	// 選択
	pas = sePas + "AS_130312_決定／クリック／選択音（ピコッ）.mp3";
	AddSE(pas.c_str(), 60);
	// 攻撃
	pas = sePas + "AS_60330_炎系魔法（ボウッ！）.mp3";
	AddSE(pas.c_str(), 60);
	// アップグレードモード
	pas = sePas + "AS_1272926_ゲーム中断、ポーズ_A.mp3";
	AddSE(pas.c_str(), 60);
	// ポーション使用
	pas = sePas + "AS_1080278_ポーション、アイテムで体力回復するSE.mp3";
	AddSE(pas.c_str(), 60);
	// ダメージ
	pas = sePas + "AS_146989_ザクッ（斬撃／ひっかき／剣／爪）.mp3";
	AddSE(pas.c_str(), 60);
	// 敵にダメージ
	pas = sePas + "AS_104666_炎の魔法／たいまつ／燃やす／ファイア.mp3";
	AddSE(pas.c_str(), 60);

}

void SoundManager::AddBGM(const char* fileName, int volume_)
{
	// 情報を追加
	bgm_.fileName_.emplace_back(fileName);
	bgm_.volume_.emplace_back(volume_);
}

void SoundManager::AddSE(const char* fileName, int volume_)
{
	// 情報を追加
	se_.fileName_.emplace_back(fileName);
	se_.volume_.emplace_back(volume_);
}

#pragma once

#include <vector>
#include <string>

class SoundManager
{
public:
	// BGMの種類
	enum class BGM
	{
		TITLE,
		SELECT,
		GAME,
		CLEAR,
		OVER,

		MAX,
	};

	// SEの種類
	enum class SE
	{
		DECIDE,
		PAUSE,
		SELECT,
		ATTACK,
		UPGRADE,
		POTION,
		DAMEGED,
		DAMEGED_ENEMY,

		MAX,
	};

	// インスタンスを明示的に生成
	static void CreateInstance(void) { if (instance_ == nullptr) { instance_ = new SoundManager(); } };
	// インスタンスの取得
	static SoundManager& GetInstance(void) { if (instance_ == nullptr) { SoundManager::CreateInstance(); }return *instance_; };
	// インスタンスの削除
	static void DeleteInstance(void) { if (instance_ != nullptr) { delete instance_; } };

	void Load(void);
	void Init(void);
	void Delete(void);

	//再生
	void Play(BGM bgm,bool flg = true);
	void Play(SE se);
	
	//停止
	void Stop(BGM bgm);
	void Stop(SE se);

private:
	static SoundManager* instance_;

	struct Info
	{
		std::vector<int> handle_;			// BGMを格納する配列
		std::vector<std::string> fileName_;
		std::vector<int> volume_;
	};

	Info bgm_;
	Info se_;

	// コンストラクタ・デストラクタをprivateにして、
	// 外部から生成出来ない様にする
	SoundManager(void);
	SoundManager(const SoundManager& manager);
	~SoundManager(void);

	void AddSound(void);

	void AddBGM(const char* fileName, int volume_);
	void AddSE(const char* fileName, int volume_);

};
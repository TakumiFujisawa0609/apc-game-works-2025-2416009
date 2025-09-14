#pragma once

class WaveBase
{
public:
	// 出現させる数
	static constexpr int ENEMY_NUM = 5;

	// コンストラクタ
	WaveBase(void);

	// デストラクタ
	~WaveBase(void);

	void Init(void);
	void Load(void);
	void Update(void);
	void Draw(void);
	void Release(void);
private:

};


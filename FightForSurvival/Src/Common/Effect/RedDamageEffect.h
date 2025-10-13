#pragma once

class RedDamageEffect
{
public:

	RedDamageEffect(void);
	~RedDamageEffect(void);

	void Load(void);
	void Init(void);
	void Update(void);
	void Draw(void);

	void SetRedEffect(void);

private:

	int alpha_;
};


#pragma once


class Stage
{
public:

	// ƒ‚ƒfƒ‹‚Ì‘å‚«‚³
	static constexpr VECTOR SCALE = { 1.0f, 1.0f, 1.0f };

	// ƒ‚ƒfƒ‹‚ÌŒü‚«
	static constexpr VECTOR ROTATE = { 0.0f, 0.0f, 0.0f };

	// ‰ŠúˆÊ’u
	static constexpr VECTOR DEFAULT_POS = { 0.0f, -120.0f, 0.0f };

	Stage(void);
	~Stage(void);

	void Load(void);
	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

private:

	int modelId_;

	VECTOR scale_;
	VECTOR rotate_;
	VECTOR pos_;

};


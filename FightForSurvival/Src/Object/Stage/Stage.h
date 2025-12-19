#pragma once


class Stage
{
public:

	// ƒ‚ƒfƒ‹‚Ì‘å‚«‚³
	static constexpr VECTOR SCALE = { 0.8f, 0.8f, 0.8f };

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

	int GetModelId(void)const { return modelId_; }
	int GetWallModelId(void)const { return wallModelId_; }

	float GetModelScaleX(void)const;
	float GetModelScaleZ(void)const;

private:

	int modelId_;
	int wallModelId_;
	int flowerModelId_;

	VECTOR scale_;
	VECTOR rotate_;
	VECTOR pos_;

};


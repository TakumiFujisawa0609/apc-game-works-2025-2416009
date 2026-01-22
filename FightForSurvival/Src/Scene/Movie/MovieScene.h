#pragma once
#include "../../Base.h"
#include "../Base/SceneBase.h"

class MoviePlayer;

class MovieScene : public SceneBase
{
public:
	MovieScene(void);
	~MovieScene(void)override;

	void Load(void) override;
	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:
	MoviePlayer* moviePlayer_;
};

#pragma once

#include <DxLib.h>
#include <vector>

class MiniMap
{
public:

	MiniMap(VECTOR& plaPos);
	~MiniMap(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

private:

};


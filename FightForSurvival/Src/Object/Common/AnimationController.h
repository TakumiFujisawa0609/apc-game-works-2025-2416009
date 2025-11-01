#pragma once

#include <string>
#include <map>

class AnimationController
{
public:

	static constexpr float BLEND_LATIO = 0.2f;

	// アニメーションデータ
	struct Animation
	{
		int model = -1;
		int attachNo = -1;
		int animIndex = 0;
		float speed = 0.0f;
		float totalTime = 0.0f;
		float step = 0.0f;
	};

	// コンストラクタ
	AnimationController(int modelId);
	// デストラクタ
	~AnimationController(void);

	// 同じFBX内のアニメーションを準備
	void AddInFbx(int type, float speed, int animIndex);

	// 外部FBXからアニメーション追加
	void Add(int type, float speed, const std::string path);

	// 指定のモデルと同じ基礎データを使用してモデルを作成する
	void Duplicate(int type, float speed, const int modelId);

	// アニメーション再生
	void BlendAnimPlay(int nextPlayType, float duration , bool isLoop = true);
	void Play(int type, bool isLoop = true);

	void Update(void);
	void Release(void);

	// 再生終了
	bool IsEnd(void)const;

	// 再生中のアニメーション
	int GetPlayType(void)const;

private:

	// アニメーションするモデルのハンドルID
	int modelId_;

	// 種類別のアニメーションデータ
	std::map<int, Animation>animations_;

	// 再生中のアニメーション
	int playType_;
	Animation playAnim_;

	// アニメーションをループするかしないか
	bool isLoop_;

	// ブレンド処理用
	int blendingNextType_;          // 次にブレンドしたいアニメーションのType (-1ならブレンドは行わない)
	float blendDuration_;           // ブレンドにかける総時間（例: 0.2秒）
	float blendTimer_;              // 現在のブレンド経過時間

	// アニメーション追加の共通処理
	void Add(int type, float speed, Animation& animation);
};


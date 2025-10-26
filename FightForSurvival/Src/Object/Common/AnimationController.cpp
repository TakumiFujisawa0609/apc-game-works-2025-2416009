#include <DxLib.h>
#include "../../Scene/SceneManager.h"
#include "AnimationController.h"

// コンストラクタ
AnimationController::AnimationController(int modelId)
{
	modelId_ = modelId;
	playType_ = -1;
	isLoop_ = true;

	blendingNextType_ = -1;
	blendDuration_ = 0.0f;
	blendTimer_ = 0.0f;
}

// デストラクタ
AnimationController::~AnimationController(void)
{
}

// 同じFBX内のアニメーションを準備
void AnimationController::AddInFbx(int type, float speed, int animIndex)
{

	Animation animation;
	animation.model = -1;
	animation.animIndex = animIndex;

	// モデルにアニメーションをアタッチする
	animation.attachNo = MV1AttachAnim(modelId_, animation.animIndex);
	// ブレンド率を全て0にする
	MV1SetAttachAnimBlendRate(modelId_, animation.attachNo, 0.0f);

	Add(type, speed, animation);

}

// 外部FBXからアニメーション追加
void AnimationController::Add(int type, float speed, const std::string path)
{
	Animation animation;
	animation.model = MV1LoadModel(path.c_str());
	animation.animIndex = 0;

	// モデルにアニメーションをアタッチする
	animation.attachNo = MV1AttachAnim(modelId_, animation.animIndex, animation.model);
	// ブレンド率を全て0にする
	MV1SetAttachAnimBlendRate(modelId_, animation.attachNo, 0.0f);

	// アニメーション追加の共通処理
	Add(type, speed, animation);
}

void AnimationController::BlendAnimPlay(int nextPlayType, float duration, bool isLoop)
{
	if (playType_ == -1)
	{
		// ブレンドせず、すぐに次のアニメーションに切り替える Play を呼び出す
		Play(nextPlayType, isLoop);

		return;
	}

	// 現在ブレンド中か、すでに次のアニメーションになっているかをチェック
	if (playType_ == nextPlayType || blendingNextType_ == nextPlayType)
	{
		return;
	}

	// 新しいブレンドを開始
	blendingNextType_ = nextPlayType;
	blendDuration_ = duration;
	blendTimer_ = 0.0f; // タイマーをリセット
	isLoop_ = isLoop;

	playAnim_.step = 0.0f;

	// playType_ は現在の再生中アニメのまま維持する

}

// アニメーション再生
void AnimationController::Play(int type, bool isLoop)
{
	if (playType_ == type)
	{
		return;
	}

	for (auto& pair : animations_)
	{
		// 再生時間を強制的に 0.0f にリセット
		pair.second.step = 0.0f;

		// 強制的にアタッチされている全てのアニメーションに対してブレンド率を 0.0f にリセット
		MV1SetAttachAnimBlendRate(modelId_, pair.second.attachNo, 0.0f);

		// もし、前回再生時間が0でなかったら0に初期化する
		// これにより、一瞬途中からアニメーションしてしまうのを防ぐ
		if (MV1GetAttachAnimTime(modelId_, pair.second.attachNo) != 0.0f)
		{
			MV1SetAttachAnimTime(modelId_, pair.second.attachNo, 0.0f);
		}
	}

	// ブレンドを中断
	blendingNextType_ = -1;
	blendTimer_ = 0.0f;
	blendDuration_ = 0.0f;

	// アニメーション種別を変更
	playType_ = type;
	playAnim_ = animations_[type];

	// ブレンド率をMAXの1.0fにして、再生したいアニメーションのみのブレンドにする
	MV1SetAttachAnimBlendRate(modelId_, playAnim_.attachNo, 1.0f);

	// 初期化
	playAnim_.step = 0.0f;

	// アニメーション総時間の取得
	playAnim_.totalTime = MV1GetAttachAnimTotalTime(modelId_, playAnim_.attachNo);

	isLoop_ = isLoop;


}

void AnimationController::Update(void)
{
	float deltaTime = SceneManager::GetInstance().GetDeltaTime();

	// 1. ブレンド処理
	if (blendingNextType_ != -1)
	{
		blendTimer_ += deltaTime;

		// 0.0fから1.0fのブレンド率を計算
		float blendRate = blendTimer_ / blendDuration_;
		if (blendRate > 1.0f)
		{
			blendRate = 1.0f;
		}

		// 現在のアニメーションのブレンド率を減らす
		int currentAttachNo = animations_[playType_].attachNo;
		MV1SetAttachAnimBlendRate(modelId_, currentAttachNo, 1.0f - blendRate);

		// 次のアニメーションのブレンド率を増やす
		int nextAttachNo = animations_[blendingNextType_].attachNo;
		MV1SetAttachAnimBlendRate(modelId_, nextAttachNo, blendRate);

		// ブレンドが完了したら、現在の再生アニメーションを切り替える
		if (blendRate >= 1.0f)
		{
			// 現在の再生アニメーションを切り替え
			Play(blendingNextType_,isLoop_);
		}
	}

	// 2. アニメーションの再生時間更新 (既存の処理)

    // 現在再生中のアニメーションの時間更新と設定
	playAnim_.step += (deltaTime * playAnim_.speed);

	if (playAnim_.step > playAnim_.totalTime)
	{
		if (isLoop_)
		{
			playAnim_.step = 0.0f;
		}
		else
		{
			playAnim_.step = playAnim_.totalTime;
		}
	}

	// アニメーション設定 (再生中のアニメーションのみ時間設定すればOK)
	MV1SetAttachAnimTime(modelId_, playAnim_.attachNo, playAnim_.step);

}

void AnimationController::Release(void)
{

	// ロードした外部FBXのモデル(アニメーション)のメモリを解放する
	// pair　→　int型とAnimation型をセットでもらってください
	// const →　読み取り専用
	for (const std::pair<int, Animation>& pair : animations_)
	{
		if (pair.second.model != -1)
		{
			MV1DeleteModel(pair.second.model);
		}
	}

	// 一般的 auto で勝手に型を確認してもらう
	//for (const auto& pair : animations_)
	//{
	//	if (pair.second.model != -1)
	//	{
	//		MV1DeleteModel(pair.second.model);
	//	}
	//}

	// 可変長配列のクリア
	animations_.clear();


}

// 再生終了
bool AnimationController::IsEnd(void) const
{
	if (isLoop_)
	{
		return false;
	}

	// アニメーションを終了したら
	if (playAnim_.step >= playAnim_.totalTime)
	{
		// 再生時間が過ぎている
		return true;
	}

	return false;
}

// 再生中のアニメーション
int AnimationController::GetPlayType(void) const
{
	return playType_;
}

// アニメーション追加の共通処理
void AnimationController::Add(int type, float speed, Animation& animation)
{

	animation.speed = speed;

	// 入れようとしているアニメーションのタイプが重複していないか確認
	// 重複していなければ配列に追加
	if (animations_.count(type) == 0)
	{
		// 動的配列に追加
		animations_.emplace(type, animation);
	}
}

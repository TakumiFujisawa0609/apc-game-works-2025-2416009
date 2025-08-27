#include <DxLib.h>
#include "../../Manager/SceneManager.h"
#include "AnimationController.h"

// コンストラクタ
AnimationController::AnimationController(int modelId)
{
	modelId_ = modelId;
	playType_ = -1;
	isLoop_ = true;
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

	Add(type, speed, animation);

}

// 外部FBXからアニメーション追加
void AnimationController::Add(int type, float speed, const std::string path)
{
	Animation animation;
	animation.model = MV1LoadModel(path.c_str());
	animation.animIndex = -1;

	// アニメーション追加の共通処理
	Add(type, speed, animation);
}

// アニメーション再生
void AnimationController::Play(int type, bool isLoop)
{
	if (playType_ == type)
	{
		// 同じアニメーションだったら再生を継続する
		return;
	}

	if (playType_ != -1)
	{
		// モデルからアニメーションを外す
		MV1DetachAnim(modelId_, playAnim_.attachNo);
	}

	// アニメーション種別を変更
	playType_ = type;
	playAnim_ = animations_[type];

	// 初期化
	playAnim_.step = 0.0f;

	// モデルと同じファイルからアニメーションをアタッチする
	if (playAnim_.model == -1)
	{
		playAnim_.attachNo = MV1AttachAnim(modelId_, playAnim_.animIndex);

	}
	else
	{
		// 別のモデルファイルからアニメーションをアタッチする
		// DxModelViewerを確認すること(大体0か1)
		int animIdx = 0;
		playAnim_.attachNo = MV1AttachAnim(modelId_, animIdx, playAnim_.model);
	}


	// アニメーション総時間の取得
	playAnim_.totalTime = MV1GetAttachAnimTotalTime(modelId_, playAnim_.attachNo);

	isLoop_ = isLoop;

}

void AnimationController::Update(void)
{

	// 経過時間の取得
	float deltaTime = SceneManager::GetInstance().GetDeltaTime();

	// 再生
	playAnim_.step += (deltaTime * playAnim_.speed);

	// ループさせるアニメーションなら処理
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

	// アニメーション設定
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

#include <DxLib.h>
#include "../../Utility/AsoUtility.h"
#include "../Player/Player.h"
#include "../Common/AnimationController.h"
#include "EnemyBase.h"

EnemyBase::EnemyBase(void)
{
	enemy_.modelId_ = -1;
}

EnemyBase::~EnemyBase(void)
{
}

void EnemyBase::Init(ENEMY_TYPE type, int baseModelId, int baseAttackEffectModelId, VECTOR pos, Player* player)
{
	// エネミー種別
	type_ = type;

	// モデルのロード
	enemy_.modelId_ = MV1DuplicateModel(baseModelId);

	// エフェクト用モデルハンドル
	baseAttackEffectModelId_ = baseAttackEffectModelId;

	// 指定された座標を設定
	enemy_.pos_ = pos;

	// プレイヤーのポインタを格納
	player_ = player;

	// パラメータ設定
	SetParam();

	// 大きさを設定する
	MV1SetScale(enemy_.modelId_, enemy_.scales_);

	enemy_.dir_ = AsoUtility::VECTOR_ZERO;

	// アニメーション登録
	AddAnimation();

	// 初期状態
	ChangeState(STATE::IDLE);

	attackFrameCnt_ = 0;
}

void EnemyBase::Update(void)
{
	switch (state_)
	{
	case EnemyBase::STATE::IDLE:
		IdleUpdate();
		break;
	case EnemyBase::STATE::MOVE:
		MoveUpdate();
		break;
	case EnemyBase::STATE::PUNCH:
		PunchUpdate();
		break;
	case EnemyBase::STATE::MAGIC:
		MagicUpdate();
		break;
	case EnemyBase::STATE::AREA:
		AreaUpdate();
		break;
	case EnemyBase::STATE::HIT_REACT:
		HitReactUpdate();
		break;
	case EnemyBase::STATE::DEAD_REACT:
		DeadReactUpdate();
		break;
	case EnemyBase::STATE::END:
		EndUpdate();
		break;
	default:
		break;
	}

	// アニメーション更新
	animationController_->Update();
}

void EnemyBase::Draw(void)
{
	if (!enemy_.isAlive_)
	{
		// 生存していなければ処理を行わない
		return;
	}

	MV1DrawModel(enemy_.modelId_);

#ifdef _DEBUG
	// デバッグ用：衝突判定用球体
	DrawSphere3D(enemy_.pos_, enemy_.collisionRadius_, 10, 0xff0000, 0xff0000, false);
#endif // _DEBUG
}

void EnemyBase::Release(void)
{
	if (enemy_.modelId_ != -1)
	{
		// 中にデータが入っていたら解放する
		MV1DeleteModel(enemy_.modelId_);
	}
}

void EnemyBase::ChangeState(STATE state)
{
	state_ = state;

	switch (state_)
	{
	case EnemyBase::STATE::IDLE:
		ChangeIdle();
		break;
	case EnemyBase::STATE::MOVE:
		ChangeMove();
		break;
	case EnemyBase::STATE::PUNCH:
		ChangePunch();
		break;
	case EnemyBase::STATE::MAGIC:
		ChangeMagic();
		break;
	case EnemyBase::STATE::AREA:
		ChangeArea();
		break;
	case EnemyBase::STATE::HIT_REACT:
		ChangeHitReact();
		break;
	case EnemyBase::STATE::DEAD_REACT:
		ChangeDeadReact();
		break;
	case EnemyBase::STATE::END:
		ChangeEnd();
		break;
	default:
		break;
	}
}

void EnemyBase::ChangeIdle(void)
{
	animationController_->Play(static_cast<int>(ANIM_TYPE::IDLE));
}

void EnemyBase::ChangeMove(void)
{
	animationController_->Play(static_cast<int>(ANIM_TYPE::WALK));
}

void EnemyBase::ChangePunch(void)
{
	animationController_->Play(static_cast<int>(ANIM_TYPE::PUNCH),false);
}

void EnemyBase::ChangeMagic(void)
{
	//animationController_->Play(static_cast<int>(ANIM_TYPE::MAGIC));
}

void EnemyBase::ChangeArea(void)
{
	//animationController_->Play(static_cast<int>(ANIM_TYPE::AREA));
}

void EnemyBase::ChangeHitReact(void)
{
	animationController_->Play(static_cast<int>(ANIM_TYPE::HIT_REACT),false);
}

void EnemyBase::ChangeDeadReact(void)
{
	animationController_->Play(static_cast<int>(ANIM_TYPE::DEATH),false);
}

void EnemyBase::ChangeEnd(void)
{
	// エフェクト停止
	//StopEffekseer3DEffect(effectBlastPlayId_);
	// 生存判定を折る
	enemy_.isAlive_ = false;
}

void EnemyBase::IdleUpdate(void)
{
	//if (範囲内に入ってなかったら移動)
	{
		// 範囲内に入っていなかったら移動を行う
		ChangeState(STATE::MOVE);
	}
	//else
	//{

	//	if (攻撃制限時間を超えたら入る)
	//	{
	//		// 攻撃制限時間を超えているかつ、範囲内に入っていたら攻撃を行う
	//		ChangeState(STATE::PUNCH);
	//	}
	//}
}

void EnemyBase::MoveUpdate(void)
{
	// プレイヤーのほうへ向く
	LookPlayer();

	enemy_.pos_ = VAdd(enemy_.pos_, VScale(enemy_.dir_, enemy_.moveSpeed_));

	//if (範囲内に入っていたら攻撃)
	{
		// 攻撃範囲内に入ったら攻撃を行う
		ChangeState(STATE::PUNCH);
	}
}

void EnemyBase::PunchUpdate(void)
{
	attackFrameCnt_++;

	//if (攻撃終わったらIDLEに戻す)
	{
		// 攻撃が終わったらIDLE状態に戻す
		ChangeState(STATE::IDLE);
	}
}

void EnemyBase::MagicUpdate(void)
{
	attackFrameCnt_++;
}

void EnemyBase::AreaUpdate(void)
{
	attackFrameCnt_++;
}

void EnemyBase::HitReactUpdate(void)
{
	//if (アニメーションを終えたら)
	{
		ChangeState(STATE::IDLE);
	}
}

void EnemyBase::DeadReactUpdate(void)
{
	//if (アニメーション終わったら入る)
	{
		// 死亡リアクションを終えたらENDへ移行
		ChangeState(STATE::END);
	}
}

void EnemyBase::EndUpdate(void)
{
}

void EnemyBase::LookPlayer(void)
{
	// プレイヤーの座標
	VECTOR playerPos = player_->GetPlayer().pos_;

	// 相手へのベクトルを計算(引き算)
	VECTOR vec;
	vec.x = playerPos.x - enemy_.pos_.x;
	vec.z = playerPos.z - enemy_.pos_.z;
	vec.y = 0.0f;

	// ベクトルの正規化で単位ベクトル(方向)を取得する
	float length = sqrtf(vec.x * vec.x + vec.z * vec.z);

	if (length == 0.0f)
	{
		vec.x = vec.z = 0.0f;
	}

	// 大きさで割って単位ベクトルにする
	enemy_.dir_.x = vec.x / length;
	enemy_.dir_.z = vec.z / length;

	// 方向から角度を出す
	//angles_.y = atan2(moveDir_.x, moveDir_.z);

	// 今回のモデルのY軸向きが逆なので向きを反転させる
	//angles_.y += 180.0f * (DX_PI_F / 180.0f);

	// 回転はY軸のみとする
	//angles_.x = angles_.z = 0.0f;

	// モデルに向きを設定
	//MV1SetRotationXYZ(modelId_, angles_);
}

void EnemyBase::AddAnimation(void)
{
	// ダンステスト
	//std::string pas = "Data/Model/Enemy/Idle.mv1";
	//animationController_->Add(static_cast<int>(ANIM_TYPE::IDLE), 60.0f, pas);
}

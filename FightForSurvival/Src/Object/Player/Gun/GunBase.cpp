#include <DxLib.h>
#include "GunBase.h"
//
//GunBase::GunBase(Player* player)
//{
//	player_ = player;
//	modelId_ = -1;
//}
//
//GunBase::~GunBase(void)
//{
//}
//
//void GunBase::Init(GUN_TYPE type)
//{
//	weapontype_ = type;
//
//	// 画像やモデルなどのロード
//	Load();
//
//	// パラメータ設定
//	SetParam();
//
//}
//
//void GunBase::Update(void)
//{
//	
//}
//
//void GunBase::Draw(void)
//{
//	if (modelId_ != -1)
//	{
//		// モデルを何か読み込んでいたら描画させる
//		MV1DrawModel(modelId_);
//	}
//}
//
//void GunBase::Release(void)
//{
//	if (modelId_ != -1)
//	{
//		// モデルを何か読み込んでいたら解放させる
//		MV1DeleteModel(modelId_);
//	}
//}
//
//

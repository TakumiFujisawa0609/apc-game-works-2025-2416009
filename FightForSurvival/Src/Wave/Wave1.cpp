#include "Wave1.h"

#include "../Application.h"

#include "../UI/UIManager.h"
#include "../UI/TextrueManager/TextureManager.h"

// 準備30秒 → 戦闘120秒
Wave1::Wave1(void)
	: WaveBase(60 * 0, 60 * 20)
{
	// UI管理の生成処理
	uiMgr_ = new UIManager();
	texMgr_ = new TextureManager();

	// UIを生成
	UIBase* manual = UIFactory::GetInstance()->CreateUI(UI_KIND::MANUAL, texMgr_);
	// 生成したUIを追加
	uiMgr_->AddUI(manual);

	// ※数値や敵の種別を外部ファイルから取得するようにすると評価〇！
	// スポーンタイミング、敵種別、座標
	AddSpawnEvent(60 * 3, ENEMY_TYPE::ZOMBIE, VGet(-1000.0f, 5.0f, 500.0f));
	AddSpawnEvent(60 * 7, ENEMY_TYPE::ZOMBIE, VGet(600.0f, 5.0f, 3000.0f));
	AddSpawnEvent(60 * 10, ENEMY_TYPE::BAT, VGet(-500.0f, 5.0f, 0.0f));
	AddSpawnEvent(60 * 15, ENEMY_TYPE::ZOMBIE, VGet(0.0f, 5.0f, -1500.0f));
	AddSpawnEvent(60 * 18, ENEMY_TYPE::BAT, VGet(100.0f, 5.0f, 300.0f));

	// スポーンタイミング、スポナーの敵スポーン間隔、座標
	AddSpawner(60 * 7, 10, VGet(-500.0f, -70.0f, -2000.0f));

}

Wave1::~Wave1(void)
{
	// UIの解放
	delete uiMgr_;
	delete texMgr_;
}

void Wave1::OnStart()
{
}

void Wave1::OnWave()
{
}

void Wave1::OnClear()
{
}

void Wave1::Update(void)
{
	// 親クラスの
	WaveBase::Update();

	//if (state_ != WaveState::PREPARE)
	//{
	//	// 準備中でなければこの先の処理を行わない
	//	return;
	//}

	//// 2秒たったら説明書の表示を消す
	//if (elapsed_ >= 60 * 2)
	//{
	//	for (UIBase* ui : uiMgr_->GetUIList())
	//	{
	//		if (ui->GetUIKind() != UI_KIND::MANUAL)
	//		{
	//			continue;
	//		}

	//		ui->SetIsDraw(false);
	//	}
	//}

	//// UIの更新
	//uiMgr_->Update();
}

void Wave1::Draw()
{
	// UIの描画
	//uiMgr_->Draw();

	int posX = Application::SCREEN_SIZE_X / 2;

	// 親クラスの共通描画物
	WaveBase::Draw();

	if (state_ != WaveBase::WaveState::PREPARE)
	{
		DrawStringToHandle(posX - 25, 5, "Wave1", 0xffffff, font_[static_cast<int>(Font::SMALL)]);
	}
	DrawStringToHandle(8, 40, "敵を倒せ！", 0xff0000, font_[static_cast<int>(Font::SMALL)]);
}
#include "Wave1.h"

#include "../Application.h"

#include "../UI/UIManager.h"
#include "../UI/TextrueManager/TextureManager.h"
#include "../Manager/InputManager.h"
#include "../UI/Object/Sprite/ExplaneSprite/ExplaneSprite.h"

// 準備20秒 → 戦闘45秒
Wave1::Wave1(void)
	: WaveBase(60 * 20, 60 * 45)
{
	// UI管理の生成処理
	uiMgr_ = new UIManager();
	texMgr_ = new TextureManager();

	// UIを生成
	UIBase* manual = UIFactory::GetInstance()->CreateUI(UI_KIND::EXPLANE, texMgr_);
	// 生成したUIを追加
	uiMgr_->AddUI(manual);

	// スポーンタイミング、敵種別、座標
	AddSpawnEvent(60 * 7, ENEMY_TYPE::ZOMBIE, VGet(0.0f, 5.0f, 1500.0f));
	AddSpawnEvent(60 * 10, ENEMY_TYPE::ZOMBIE, VGet(0.0f, 5.0f, 1500.0f));

	AddSpawnEvent(60 * 15, ENEMY_TYPE::ZOMBIE, VGet(20.0f, 5.0f, 1500.0f));
	AddSpawnEvent(60 * 16, ENEMY_TYPE::ZOMBIE, VGet(-20.0f, 5.0f, 1500.0f));
	AddSpawnEvent(60 * 17, ENEMY_TYPE::ZOMBIE, VGet(40.0f, 5.0f, 1500.0f));
	AddSpawnEvent(60 * 18, ENEMY_TYPE::ZOMBIE, VGet(-40.0f, 5.0f, 1500.0f));
	AddSpawnEvent(60 * 19, ENEMY_TYPE::ZOMBIE, VGet(-40.0f, 5.0f, 1500.0f));

	// スポーンタイミング、スポナーの敵スポーン間隔、座標
	AddSpawner(60 * 30, 10, VGet(-100.0f, 5.0f, 1500.0f), Spawner::PATTERN::PATTERN_1);

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

	if (state_ != WaveState::PREPARE)
	{
		// 準備中でなければこの先の処理を行わない
		return;
	}

	// 準備時間が経過したら
	//if (elapsed_ >= prepareTime_)
	//{
	//	// ウェーブ開始
	//	StartInWave();
	//}

	// 2秒たったら説明書の表示を消す
	for (UIBase* ui : uiMgr_->GetUIList())
	{
		if (InputManager::GetInstance().IsTrgUp(KEY_INPUT_M))
		{
			ui->SetIsDraw(false);
			state_ = WaveState::INWAVE;
		}

		if (ui->GetUIKind() != UI_KIND::EXPLANE)
		{
			continue;
		}
		ExplaneSprite* explane = dynamic_cast<ExplaneSprite*>(ui);

		if (elapsed_ >= 60 * 19)
		{
			ui->SetIsDraw(false);
		}
		else if (elapsed_ >= 60 * 15)
		{
			explane->SetDrawPictureKind(ExplaneSprite::EXPLANE_4);
		}
		else if (elapsed_ >= 60 * 10)
		{
			explane->SetDrawPictureKind(ExplaneSprite::EXPLANE_3);
		}
		else if (elapsed_ >= 60 * 5)
		{
			explane->SetDrawPictureKind(ExplaneSprite::EXPLANE_2);
		}
	}

	// UIの更新
	uiMgr_->Update();
}

void Wave1::Draw()
{
	// UIの描画
	uiMgr_->Draw();

	int posX = Application::SCREEN_SIZE_X / 2;

	// 親クラスの共通描画物
	WaveBase::Draw();

	if (state_ != WaveBase::WaveState::PREPARE)
	{
		DrawStringToHandle(posX - 25, 5, "Wave1", 0xffffff, font_[static_cast<int>(Font::SMALL)]);
	}
	DrawStringToHandle(8, 40, "敵を倒せ！", 0xff0000, font_[static_cast<int>(Font::SMALL)]);
}
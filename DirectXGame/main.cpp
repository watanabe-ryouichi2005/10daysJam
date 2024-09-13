#include "Audio.h"
#include "AxisIndicator.h"
#include "ClearScene.h"
#include "DirectXCommon.h"
#include "GameScene.h"
#include "ImGuiManager.h"
#include "OverScene.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
#include "TitleScene.h"
#include "TutorialScene.h"
#include "WinApp.h"

GameScene* gameScene = nullptr;
TitleScene* titleScene = nullptr;
TutorialScene* tutorialScene = nullptr;
ClearScene* clearScene = nullptr;
OverScene* overScene = nullptr;
Audio* audio_ = Audio::GetInstance();

enum class Scene {
	kUnknown = 0,

	kTitle,
	kTutorial,
	kGame,
	kDead,
	kClear,
};

Scene scene = Scene::kTitle;

void ChangeScene() {
	switch (scene) {
	case Scene::kTitle:
		if (titleScene->IsGame()) {
			// シーン変更
			scene = Scene::kGame;
			// 旧シーンの解放
			delete titleScene;
			titleScene = nullptr;
			// 新シーンの生成と初期化
			gameScene = new GameScene();
			gameScene->Initialize();
		}
		else if (titleScene->IsTutorial()) {
			// シーン変更
			scene = Scene::kTutorial;
			// 旧シーンの解放
			delete titleScene;
			titleScene = nullptr;
			// 新シーンの生成と初期化
			tutorialScene = new TutorialScene();
			tutorialScene->Initialize();
		}
		break;		
	case Scene::kTutorial:
		if (tutorialScene->IsFinished()) {
			// シーン変更
			scene = Scene::kTitle;
			delete tutorialScene;
			tutorialScene = nullptr;
			// 新シーンの生成と初期化
			titleScene = new TitleScene();
			titleScene->Initialize();
		}
		break;

	case Scene::kGame:
		if (gameScene->IsDead()) {
			// シーン変更
			scene = Scene::kDead;
			delete gameScene;
			gameScene = nullptr;
			overScene = new OverScene();

			overScene->Initialize();
			
		}
		else if (gameScene->IsFinished()) {
			// シーン変更
			scene = Scene::kClear;
			delete gameScene;
			gameScene = nullptr;
			clearScene = new ClearScene();
			clearScene->Initialize();
		}
		break;

	case Scene::kDead:
		if (overScene->IsFinished()) {
			// シーン変更
			scene = Scene::kTitle;
			delete overScene;
			overScene = nullptr;
			titleScene = new TitleScene();
			titleScene->Initialize();
		}
		break;
	case Scene::kClear:
		if (clearScene->IsFinished()) {
			// シーン変更
			scene = Scene::kTitle;
			delete clearScene;
			clearScene = nullptr;
			titleScene = new TitleScene();
			titleScene->Initialize();
		}
		break;
	}
}
void UpdateScene() {
	switch (scene) {
	case Scene::kTitle:
		titleScene->Update();
		break;
	case Scene::kTutorial:
		tutorialScene->Update();
		break;
	case Scene::kGame:
		gameScene->Update();
		break;
	case Scene::kDead:
		overScene->Update();
	//	Audio::StopWave()
		break;
	case Scene::kClear:
		clearScene->Update();
		break;
	}
}
void DrawScene() {
	switch (scene) {
	case Scene::kTitle:
		titleScene->Draw();
		break;
	case Scene::kTutorial:
		tutorialScene->Draw();
		break;
	case Scene::kGame:
		gameScene->Draw();
		break;
	case Scene::kDead:
		overScene->Draw();
		break;
	case Scene::kClear:
		clearScene->Draw();
		break;
	}
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	WinApp* win = nullptr;
	DirectXCommon* dxCommon = nullptr;
	// 汎用機能
	Input* input = nullptr;
	Audio* audio = nullptr;
	AxisIndicator* axisIndicator = nullptr;
	PrimitiveDrawer* primitiveDrawer = nullptr;

	// ゲームウィンドウの作成
	win = WinApp::GetInstance();
	win->CreateGameWindow(L"6041_Ghost_Down");

	// DirectX初期化処理
	dxCommon = DirectXCommon::GetInstance();
	dxCommon->Initialize(win);

#pragma region 汎用機能初期化
	// ImGuiの初期化
	ImGuiManager* imguiManager = ImGuiManager::GetInstance();
	imguiManager->Initialize(win, dxCommon);

	// 入力の初期化
	input = Input::GetInstance();
	input->Initialize();

	// オーディオの初期化
	audio = Audio::GetInstance();
	audio->Initialize();

	// テクスチャマネージャの初期化
	TextureManager::GetInstance()->Initialize(dxCommon->GetDevice());
	TextureManager::Load("white1x1.png");

	// スプライト静的初期化
	Sprite::StaticInitialize(dxCommon->GetDevice(), WinApp::kWindowWidth, WinApp::kWindowHeight);

	// 3Dモデル静的初期化
	Model::StaticInitialize();

	// 軸方向表示初期化
	axisIndicator = AxisIndicator::GetInstance();
	axisIndicator->Initialize();

	primitiveDrawer = PrimitiveDrawer::GetInstance();
	primitiveDrawer->Initialize();
#pragma endregion

	// ゲームシーンの初期化
	scene = Scene::kTitle;
	scene = Scene::kTitle;
	titleScene = new TitleScene;
	titleScene->Initialize();

	// メインループ
	while (true) {
		// メッセージ処理
		if (win->ProcessMessage()) {
			break;
		}

		// ImGui受付開始
		imguiManager->Begin();
		// 入力関連の毎フレーム処理
		input->Update();
		// シーンの切り替え
		ChangeScene();
		// シーン変更
		UpdateScene();
		// ImGui受付終了
		imguiManager->End();

		// 描画開始
		dxCommon->PreDraw();
		// ゲームシーンの描画
		DrawScene();
		// 軸表示の描画
		axisIndicator->Draw();
		// プリミティブ描画のリセット
		primitiveDrawer->Reset();
		// ImGui描画
		imguiManager->Draw();
		// 描画終了
		dxCommon->PostDraw();
	}

	// 各種解放
	delete titleScene;
	delete tutorialScene;
	delete gameScene;
	delete overScene;
	delete clearScene;
	// 3Dモデル解放
	Model::StaticFinalize();
	audio->Finalize();
	// ImGui解放
	imguiManager->Finalize();

	// ゲームウィンドウの破棄
	win->TerminateGameWindow();

	return 0;
}
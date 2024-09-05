#include "GameScene.h"
#include "TextureManager.h"
#include "myMath.h"
#include <cassert>
#include <cstdint>

GameScene::GameScene() {}

GameScene::~GameScene() {

	delete deathParticles_;
	for (DeathBlock* deathBlock : deathBlocks_) {
		delete deathBlock;
	}
	for (Goal* goal : goals_) {
		delete goal;
	}
	for (JumpBlock* jumpBlock : jumpBlocks_) {
		delete jumpBlock;
	}
	delete player_;
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
			worldTransformBlock = nullptr;
		}
	}
	delete modelDeathParticle_;
	delete modelGoal_;
	delete modelDeathBlock_;
	delete modelJumpBlock_;
	delete modelPlayer_;
	delete modelBlock_;
	delete debugCamera_;
	delete modelSkydome_;
	delete mapChipField_;
	delete cameraController;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

	// 3Dモデルの生成
	modelPlayer_ = Model::CreateFromOBJ("player");
	modelGoal_ = Model::CreateFromOBJ("goal");
	modelDeathBlock_ = Model::CreateFromOBJ("deathBlock");
	modelJumpBlock_ = Model::CreateFromOBJ("jumpBlock");
	modelBlock_ = Model::CreateFromOBJ("block");
	modelSkydome_ = Model::CreateFromOBJ("sky", true);
	modelDeathParticle_ = Model::CreateFromOBJ("deathParticle", true);

	// マップチップフィールドの生成
	mapChipField_ = new MapChipField;
	mapChipField_->LoadMapChipCsv("Resources/blocks.csv");

	// 自キャラの生成
	player_ = new Player();
	// 自キャラの初期化
	// 座標をマップチップ番号で指定
	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(2, 2);
	player_->Initialize(modelPlayer_, &viewProjection_, playerPosition);
	player_->SetMapChipField(mapChipField_);

	viewProjection_.Initialize();

	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);

	// 天球の初期化
	worldTransformSkydome_.Initialize();

	GenerateBlocks();

	cameraController = new CameraController;
	cameraController->Initialize();
	cameraController->SetTarget(player_);
	cameraController->Reset();

	CameraController::Rect cameraArea = {12.0f, 100 - 12.0f, 6.0f, 6.0f};
	cameraController->SetMovableArea(cameraArea);

	// 敵の生成
	Goal* newGoal = new Goal();
	Vector3 goalPosition = mapChipField_->GetMapChipPositionByIndex(0, 68);
	newGoal->Initialize(modelGoal_, &viewProjection_, goalPosition);

	goals_.push_back(newGoal);

	for (int32_t i = 0; i < 38; ++i) {
		DeathBlock* newDeathBlock = new DeathBlock();
		Vector3 deathBlockPosition = mapChipField_->GetMapChipPositionByIndex(0 + i, 0);
		newDeathBlock->Initialize(modelDeathBlock_, &viewProjection_, deathBlockPosition);

		deathBlocks_.push_back(newDeathBlock);
	}

	JumpBlock* newJumpBlock = new JumpBlock();
	Vector3 jumpBlockPosition = mapChipField_->GetMapChipPositionByIndex(8, 7);
	newJumpBlock->Initialize(modelJumpBlock_, &viewProjection_, jumpBlockPosition);
	jumpBlocks_.push_back(newJumpBlock);

	phase_ = Phase::kPlay;
}

void GameScene::Update() {

	ChangePhase();

	switch (phase_) {
	case Phase::kPlay:
		worldTransformSkydome_.UpdateMatrix();

		// 自キャラの更新
		player_->Update();

		cameraController->Update();

		for (DeathBlock* deathBlock : deathBlocks_) {
			deathBlock->Update();
		}

		for (Goal* goal : goals_) {
			goal->Update();
		}

		for (JumpBlock* jumpBlock : jumpBlocks_) {
			jumpBlock->Update();
		}

		UpdateCamera();

		UpdateBlocks();

		CheckAllCollisions();

		break;

	case Phase::kDead:
		if (deathParticles_ && deathParticles_->IsDead()) {
			isDead_ = true;
		}
		worldTransformSkydome_.UpdateMatrix();

		for (DeathBlock* deathBlock : deathBlocks_) {
			deathBlock->Update();
		}

		if (deathParticles_) {
			deathParticles_->Update();
		}

		UpdateCamera();
		break;

	case Phase::kGoal:
		if (deathParticles_ && deathParticles_->IsFinished()) {
			finished_ = true;
		}
		worldTransformSkydome_.UpdateMatrix();

		for (Goal* goal : goals_) {
			goal->Update();
		}

		if (deathParticles_) {
			deathParticles_->Update();
		}

		UpdateCamera();
		break;
	}
}

void GameScene::Draw() {
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	// 天球の描画
	modelSkydome_->Draw(worldTransformSkydome_, viewProjection_);

	for (std::vector<WorldTransform*> worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform*& worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)

				continue;

			modelBlock_->Draw(*worldTransformBlock, viewProjection_);
		}
	}
	if (!player_->IsDead()) {
		// 自キャラの描画
		player_->Draw();
	}
	if (!player_->IsGoal()) {
		// 自キャラの描画
		player_->Draw();
	}

	for (DeathBlock* deathBlock : deathBlocks_) {
		deathBlock->Draw();
	}

	for (Goal* goal : goals_) {
		goal->Draw();
	}

	for (JumpBlock* jumpBlock : jumpBlocks_) {
		jumpBlock->Draw();
	}

	if (deathParticles_) {
		deathParticles_->Draw();
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::ChangePhase() {
	switch (phase_) {
	case Phase::kPlay:
		if (player_->IsDead()) {
			// 死亡演出
			phase_ = Phase::kDead;

			const Vector3& deathParticlesPosition = player_->GetWorldPosition();

			deathParticles_ = new DeathParticles;

			deathParticles_->Initialize(modelDeathParticle_, &viewProjection_, deathParticlesPosition);
		}

		if (player_->IsGoal()) {
			// 死亡演出
			phase_ = Phase::kGoal;

			const Vector3& deathParticlesPosition = player_->GetWorldPosition();

			deathParticles_ = new DeathParticles;

			deathParticles_->Initialize(modelDeathParticle_, &viewProjection_, deathParticlesPosition);
		}
		break;

	case Phase::kDead:

		break;
	case Phase::kGoal:

		break;
	}
}

void GameScene::GenerateBlocks() {

	// 要素数
	uint32_t numBlockVirtical = mapChipField_->GetNumBlockVirtical();
	uint32_t numBlockHorizontal = mapChipField_->GetNumBlockHorizontal();

	// 要素数を変更する
	// 列数を設定 (縦方向のブロック数)
	worldTransformBlocks_.resize(numBlockVirtical);
	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		// 1列の要素数を設定 (横方向のブロック数)
		worldTransformBlocks_[i].resize(numBlockHorizontal);
	}

	// ブロックの生成
	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		for (uint32_t j = 0; j < numBlockHorizontal; ++j) {
			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kBlock) {
				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransformBlocks_[i][j] = worldTransform;
				worldTransformBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
			}
		}
	}
}

void GameScene::UpdateCamera() {
#ifdef _DEBUG
	if (input_->TriggerKey(DIK_SPACE)) {
		// フラグをトグル
		isDebugCameraActive_ = !isDebugCameraActive_;
	}
#endif
	// カメラ処理
	if (isDebugCameraActive_) {
		// デバッグカメラの更新
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	} else {
		// ビュープロジェクション行列の更新と転送
		viewProjection_.matView = cameraController->GetViewProjection().matView;
		viewProjection_.matProjection = cameraController->GetViewProjection().matProjection;
		// ビュープロジェクションの転送
		viewProjection_.TransferMatrix();
	}
}

void GameScene::UpdateBlocks() {
	// 縦横ブロック更新
	for (std::vector<WorldTransform*> worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform*& worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;

			worldTransformBlock->UpdateMatrix();
		}
	}
}

void GameScene::CheckAllCollisions() {

	// 判定対象1と2座標
	AABB aabb1, aabb2, aabb3, aabb4;

#pragma region 自キャラと敵キャラの当たり判定
	{
		// 自キャラの座標
		aabb1 = player_->GetAABB();

		for (DeathBlock* deathBlock : deathBlocks_) {
			aabb2 = deathBlock->GetAABB();

			// AABB同士の交差判定
			if (IsCollision(aabb1, aabb2)) {
				// 自キャラの衝突時コールバックを呼び出す
				player_->OverOnCollision(deathBlock);
				// 擲弾の衝突時コールバックを呼び出す
				deathBlock->OnCollision(player_);
			}
		}

		// 自キャラと擲弾全ての当たり判定
		for (Goal* goal : goals_) {
			// 擲弾の座標
			aabb3 = goal->GetAABB();

			// AABB同士の交差判定
			if (IsCollision(aabb1, aabb3)) {
				// 自キャラの衝突時コールバックを呼び出す
				player_->GoalOnCollision(goal);
				// 擲弾の衝突時コールバックを呼び出す
				goal->OnCollision(player_);
			}
		}

		for (JumpBlock* jumpBlock : jumpBlocks_) {
			aabb4 = jumpBlock->GetAABB();

			// AABB同士の交差判定
			if (IsCollision(aabb1, aabb4)) {
				// 自キャラの衝突時コールバックを呼び出す
				player_->JumpOnCollision(jumpBlock);
				// 擲弾の衝突時コールバックを呼び出す
				jumpBlock->OnCollision(player_);
			}
		}
	}
#pragma endregion
}
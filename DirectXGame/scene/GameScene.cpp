#include "GameScene.h"
#include "TextureManager.h"
#include "myMath.h"
#include <cassert>
#include <cstdint>

GameScene::GameScene() {}

GameScene::~GameScene() {

	delete deathParticles_;
	for (FallingBlock* fallingBlock : fallingBlocks_) {
		delete fallingBlock;
	}
	for (DeathBlock* deathBlock : deathBlocks_) {
		delete deathBlock;
	}
	for (JumpBlock* jumpBlock : jumpBlocks_) {
		delete jumpBlock;
	}
	for (Goal* goal : goals_) {
		delete goal;
	}
	delete player_;
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
			worldTransformBlock = nullptr;
		}
	}
	delete modelDeathParticle_;
	delete modelFallingBlock_;
	delete modelDeathBlock_;
	delete modelJumpBlock_;
	delete modelGoal_;
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
	modelFallingBlock_ = Model::CreateFromOBJ("fallingBlock");
	modelDeathBlock_ = Model::CreateFromOBJ("deathBlock");
	modelJumpBlock_ = Model::CreateFromOBJ("jumpBlock");
	modelGoal_ = Model::CreateFromOBJ("goal");
	modelBlock_ = Model::CreateFromOBJ("block");
	modelSkydome_ = Model::CreateFromOBJ("sky", true);
	modelDeathParticle_ = Model::CreateFromOBJ("deathParticle", true);
	modelLifeCube_ = Model::CreateFromOBJ("LifeCube", true);


	// マップチップフィールドの生成
	mapChipField_ = new MapChipField;
	mapChipField_->LoadMapChipCsv("Resources/blocks.csv");

	// 自キャラの生成
	player_ = new Player();
	// 自キャラの初期化
	// 座標をマップチップ番号で指定
	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(2, 8);
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

	CameraController::Rect cameraArea = {5.0f, 100 - 12.0f, 6.0f, 6.0f};
	cameraController->SetMovableArea(cameraArea);

	// 敵の生成
	Goal* newGoal = new Goal();
	Vector3 goalPosition = mapChipField_->GetMapChipPositionByIndex(23, 198);
	newGoal->Initialize(modelGoal_, &viewProjection_, goalPosition);

	goals_.push_back(newGoal);

	// 落ちるブロックの生成
	for (int32_t i = 0; i < 9; ++i) {
		FallingBlock* newFallingBlock = new FallingBlock();
		Vector3 fallingBlockPosition = mapChipField_->GetMapChipPositionByIndex(1 + i, 0);
		newFallingBlock->Initialize(modelDeathBlock_, &viewProjection_, fallingBlockPosition);

		fallingBlocks_.push_back(newFallingBlock);
	}

	// デスブロックの生成
	DeathBlock* newDeathBlock = new DeathBlock();
	Vector3 deathBlockPosition = mapChipField_->GetMapChipPositionByIndex(5, 9);
	newDeathBlock->Initialize(modelDeathBlock_, &viewProjection_, deathBlockPosition);

	deathBlocks_.push_back(newDeathBlock);

	DeathBlock* newDeathBlock_2 = new DeathBlock();
	Vector3 deathBlockPosition_2 = mapChipField_->GetMapChipPositionByIndex(4, 23);
	newDeathBlock_2->Initialize(modelDeathBlock_, &viewProjection_, deathBlockPosition_2);

	deathBlocks_.push_back(newDeathBlock_2);

	DeathBlock* newDeathBlock_3 = new DeathBlock();
	Vector3 deathBlockPosition_3 = mapChipField_->GetMapChipPositionByIndex(10, 28);
	newDeathBlock_3->Initialize(modelDeathBlock_, &viewProjection_, deathBlockPosition_3);

	deathBlocks_.push_back(newDeathBlock_3);

	for (int32_t i = 0; i < 8; ++i) {
		DeathBlock* newDeathBlock_4 = new DeathBlock();
		Vector3 deathBlockPosition_4 = mapChipField_->GetMapChipPositionByIndex(2 + i, 52);
		newDeathBlock_4->Initialize(modelDeathBlock_, &viewProjection_, deathBlockPosition_4);

		deathBlocks_.push_back(newDeathBlock_4);
	}

	for (int32_t i = 0; i < 7; ++i) {
		DeathBlock* newDeathBlock_5 = new DeathBlock();
		Vector3 deathBlockPosition_5 = mapChipField_->GetMapChipPositionByIndex(1 + i, 59);
		newDeathBlock_5->Initialize(modelDeathBlock_, &viewProjection_, deathBlockPosition_5);

		deathBlocks_.push_back(newDeathBlock_5);
	}

	/*for (int32_t i = 0; i < 2; ++i) {
		DeathBlock* newDeathBlock_6 = new DeathBlock();
		Vector3 deathBlockPosition_6 = mapChipField_->GetMapChipPositionByIndex(3, 83 + i);
		newDeathBlock_6->Initialize(modelDeathBlock_, &viewProjection_, deathBlockPosition_6);

		deathBlocks_.push_back(newDeathBlock_6);
	}

	for (int32_t i = 0; i < 2; ++i) {
		DeathBlock* newDeathBlock_7 = new DeathBlock();
		Vector3 deathBlockPosition_7 = mapChipField_->GetMapChipPositionByIndex(6, 83 + i);
		newDeathBlock_7->Initialize(modelDeathBlock_, &viewProjection_, deathBlockPosition_7);

		deathBlocks_.push_back(newDeathBlock_7);
	}*/

	// ジャンプブロックの生成
	JumpBlock* newJumpBlock = new JumpBlock();
	Vector3 jumpBlockPosition = mapChipField_->GetMapChipPositionByIndex(3, 58);
	newJumpBlock->Initialize(modelJumpBlock_, &viewProjection_, jumpBlockPosition);

	jumpBlocks_.push_back(newJumpBlock);

	JumpBlock* newJumpBlock_2 = new JumpBlock();
	Vector3 jumpBlockPosition_2 = mapChipField_->GetMapChipPositionByIndex(6, 58);
	newJumpBlock_2->Initialize(modelJumpBlock_, &viewProjection_, jumpBlockPosition_2);

	jumpBlocks_.push_back(newJumpBlock_2);

	// サウンドデータの読み込み
	soundDataHandle_ = audio_->LoadWave("bgm.mp3");
	// 音声再生
	audio_->PlayWave(soundDataHandle_);
	// 音声再生
	voiceHandle_ = audio_->PlayWave(soundDataHandle_, true);

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

		for (FallingBlock* fallingBlock : fallingBlocks_) {
			fallingBlock->Update();
		}

		for (DeathBlock* deathBlock : deathBlocks_) {
			deathBlock->Update();
		}

		for (JumpBlock* jumpBlock : jumpBlocks_) {
			jumpBlock->Update();
		}

		for (Goal* goal : goals_) {
			goal->Update();
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

		for (FallingBlock* fallingBlock : fallingBlocks_) {
			fallingBlock->Update();
		}

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
	else if (!player_->IsGoal()) {
		// 自キャラの描画
		player_->Draw();
	}
	//modelLifeCube_->Draw(worldTransformSkydome_, viewProjection_);

	for (FallingBlock* fallingBlock : fallingBlocks_) {
		fallingBlock->Draw();
	}

	for (DeathBlock* deathBlock : deathBlocks_) {
		deathBlock->Draw();
	}

	for (JumpBlock* jumpBlock : jumpBlocks_) {
		jumpBlock->Draw();
	}

	for (Goal* goal : goals_) {
		goal->Draw();
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

		else if (player_->IsGoal()) {
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
	AABB aabb1, aabb2, aabb3, aabb4, aabb5;

#pragma region 自キャラと敵キャラの当たり判定
	{
		// 自キャラの座標
		aabb1 = player_->GetAABB();

		for (FallingBlock* fallingBlock : fallingBlocks_) {
			aabb2 = fallingBlock->GetAABB();

			// AABB同士の交差判定
			if (IsCollision(aabb1, aabb2)) {
				// 自キャラの衝突時コールバックを呼び出す
				player_->FallingOnCollision(fallingBlock);
				// 擲弾の衝突時コールバックを呼び出す
				fallingBlock->OnCollision(player_);
			}
		}

		for (DeathBlock* deathBlock : deathBlocks_) {
			aabb3 = deathBlock->GetAABB();

			// AABB同士の交差判定
			if (IsCollision(aabb1, aabb3)) {
				// 自キャラの衝突時コールバックを呼び出す
				player_->OverOnCollision(deathBlock);
				// 擲弾の衝突時コールバックを呼び出す
				deathBlock->OnCollision(player_);
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

		// 自キャラと擲弾全ての当たり判定
		for (Goal* goal : goals_) {
			// 擲弾の座標
			aabb5 = goal->GetAABB();

			// AABB同士の交差判定
			if (IsCollision(aabb1, aabb5)) {
				// 自キャラの衝突時コールバックを呼び出す
				player_->GoalOnCollision(goal);
				// 擲弾の衝突時コールバックを呼び出す
				goal->OnCollision(player_);
			}
		}
	}
#pragma endregion
}
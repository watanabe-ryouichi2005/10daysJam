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
	delete modelOneText_;
	delete modelTwoText_;
	delete modelThreeText_;
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

	// 3Dテキストモデル生成
	modelOneText_ = Model::CreateFromOBJ("1_text", true);
	modelTwoText_ = Model::CreateFromOBJ("2_text", true);
	modelThreeText_ = Model::CreateFromOBJ("3_text", true);
	modelClearText_ = Model::CreateFromOBJ("clearText", true);

	const float kOneText = 1.6f;
	worldTransformOneText_.Initialize();
	worldTransformOneText_.scale_ = {kOneText, kOneText, kOneText};
	worldTransformOneText_.rotation_.y = 0.99f * std::numbers::pi_v<float>;
	worldTransformOneText_.translation_.x = 2.0f;
	worldTransformOneText_.translation_.y = +19.5f;

	const float kTwoText = 1.6f;
	worldTransformTwoText_.Initialize();
	worldTransformTwoText_.scale_ = {kTwoText, kTwoText, kTwoText};
	worldTransformTwoText_.rotation_.y = 0.99f * std::numbers::pi_v<float>;
	worldTransformTwoText_.translation_.x = 5.0f;
	worldTransformTwoText_.translation_.y = +19.5f;

	const float kThreeText = 1.6f;
	worldTransformThreeText_.Initialize();
	worldTransformThreeText_.scale_ = {kThreeText, kThreeText, kThreeText};
	worldTransformThreeText_.rotation_.y = 0.99f * std::numbers::pi_v<float>;
	worldTransformThreeText_.translation_.x = 8.0f;
	worldTransformThreeText_.translation_.y = +19.5f;

	const float kClearText = 0.5f;
	worldTransformClearText_.Initialize();
	worldTransformClearText_.scale_ = {kClearText, kClearText, kClearText};
	worldTransformClearText_.rotation_.y = 0.99f * std::numbers::pi_v<float>;
	worldTransformClearText_.translation_.x = 8.0f;
	worldTransformClearText_.translation_.y = +1.7f;

	// サウンドデータの読み込み
	soundDataHandle_ = audio_->LoadWave("bgm.mp3");
	// 音声再生
	//audio_->PlayWave(soundDataHandle_);
	// 音声再生
	voiceHandle_ = audio_->PlayWave(soundDataHandle_, true);

	// マップチップフィールドの生成
	mapChipField_ = new MapChipField;
	mapChipField_->LoadMapChipCsv("Resources/blocks.csv");

	// 自キャラの生成
	player_ = new Player();
	// 自キャラの初期化
	// 座標をマップチップ番号で指定
	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(1, 8);
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

	// 落ちるブロックの生成
	for (int32_t i = 0; i < 9; ++i) {
		FallingBlock* newFallingBlock = new FallingBlock();
		Vector3 fallingBlockPosition = mapChipField_->GetMapChipPositionByIndex(1 + i, 0);
		newFallingBlock->Initialize(modelDeathBlock_, &viewProjection_, fallingBlockPosition);
		fallingBlocks_.push_back(newFallingBlock);
	}

	// デスブロックの生成
	for (int32_t i = 0; i < 2; ++i) {
		DeathBlock* newDeathBlock = new DeathBlock();
		Vector3 deathBlockPosition = mapChipField_->GetMapChipPositionByIndex(10, 14 + i);
		newDeathBlock->Initialize(modelDeathBlock_, &viewProjection_, deathBlockPosition);
		deathBlocks_.push_back(newDeathBlock);
	}

	DeathBlock* newDeathBlock_2 = new DeathBlock();
	Vector3 deathBlockPosition_2 = mapChipField_->GetMapChipPositionByIndex(4, 23);
	newDeathBlock_2->Initialize(modelDeathBlock_, &viewProjection_, deathBlockPosition_2);
	deathBlocks_.push_back(newDeathBlock_2);

	DeathBlock* newDeathBlock_3 = new DeathBlock();
	Vector3 deathBlockPosition_3 = mapChipField_->GetMapChipPositionByIndex(10, 28);
	newDeathBlock_3->Initialize(modelDeathBlock_, &viewProjection_, deathBlockPosition_3);
	deathBlocks_.push_back(newDeathBlock_3);

	DeathBlock* newDeathBlock_4 = new DeathBlock();
	Vector3 deathBlockPosition_4 = mapChipField_->GetMapChipPositionByIndex(6, 33);
	newDeathBlock_4->Initialize(modelDeathBlock_, &viewProjection_, deathBlockPosition_4);
	deathBlocks_.push_back(newDeathBlock_4);

	for (int32_t i = 0; i < 8; ++i) {
		DeathBlock* newDeathBlock_5 = new DeathBlock();
		Vector3 deathBlockPosition_5 = mapChipField_->GetMapChipPositionByIndex(2 + i, 52);
		newDeathBlock_5->Initialize(modelDeathBlock_, &viewProjection_, deathBlockPosition_5);
		deathBlocks_.push_back(newDeathBlock_5);
	}

	for (int32_t i = 0; i < 7; ++i) {
		DeathBlock* newDeathBlock_6 = new DeathBlock();
		Vector3 deathBlockPosition_6 = mapChipField_->GetMapChipPositionByIndex(1 + i, 59);
		newDeathBlock_6->Initialize(modelDeathBlock_, &viewProjection_, deathBlockPosition_6);
		deathBlocks_.push_back(newDeathBlock_6);
	}

	for (int32_t i = 0; i < 4; ++i) {
		DeathBlock* newDeathBlock_7 = new DeathBlock();
		Vector3 deathBlockPosition_7 = mapChipField_->GetMapChipPositionByIndex(0, 84 + i);
		newDeathBlock_7->Initialize(modelDeathBlock_, &viewProjection_, deathBlockPosition_7);
		deathBlocks_.push_back(newDeathBlock_7);
	}

	for (int32_t i = 0; i < 7; ++i) {
		DeathBlock* newDeathBlock_8 = new DeathBlock();
		Vector3 deathBlockPosition_8 = mapChipField_->GetMapChipPositionByIndex(3 + i, 100);
		newDeathBlock_8->Initialize(modelDeathBlock_, &viewProjection_, deathBlockPosition_8);
		deathBlocks_.push_back(newDeathBlock_8);
	}

	for (int32_t i = 0; i < 8; ++i) {
		DeathBlock* newDeathBlock_9 = new DeathBlock();
		Vector3 deathBlockPosition_9 = mapChipField_->GetMapChipPositionByIndex(2 + i, 104 + i);
		newDeathBlock_9->Initialize(modelDeathBlock_, &viewProjection_, deathBlockPosition_9);
		deathBlocks_.push_back(newDeathBlock_9);
	}

	for (int32_t i = 0; i < 8; ++i) {
		DeathBlock* newDeathBlock_10 = new DeathBlock();
		Vector3 deathBlockPosition_10 = mapChipField_->GetMapChipPositionByIndex(8 - i, 114 + i);
		newDeathBlock_10->Initialize(modelDeathBlock_, &viewProjection_, deathBlockPosition_10);
		deathBlocks_.push_back(newDeathBlock_10);
	}

	for (int32_t i = 0; i < 8; ++i) {
		DeathBlock* newDeathBlock_11 = new DeathBlock();
		Vector3 deathBlockPosition_11 = mapChipField_->GetMapChipPositionByIndex(2 + i, 124 + i);
		newDeathBlock_11->Initialize(modelDeathBlock_, &viewProjection_, deathBlockPosition_11);
		deathBlocks_.push_back(newDeathBlock_11);
	}

	for (int32_t i = 0; i < 8; ++i) {
		DeathBlock* newDeathBlock_12 = new DeathBlock();
		Vector3 deathBlockPosition_12 = mapChipField_->GetMapChipPositionByIndex(2 + i, 141);
		newDeathBlock_12->Initialize(modelDeathBlock_, &viewProjection_, deathBlockPosition_12);
		deathBlocks_.push_back(newDeathBlock_12);
	}

	for (int32_t i = 0; i < 8; ++i) {
		DeathBlock* newDeathBlock_13 = new DeathBlock();
		Vector3 deathBlockPosition_13 = mapChipField_->GetMapChipPositionByIndex(1 + i, 148);
		newDeathBlock_13->Initialize(modelDeathBlock_, &viewProjection_, deathBlockPosition_13);
		deathBlocks_.push_back(newDeathBlock_13);
	}

	for (int32_t i = 0; i < 4; ++i) {
		DeathBlock* newDeathBlock_14 = new DeathBlock();
		Vector3 deathBlockPosition_14 = mapChipField_->GetMapChipPositionByIndex(2 + i, 159 + i);
		newDeathBlock_14->Initialize(modelDeathBlock_, &viewProjection_, deathBlockPosition_14);
		deathBlocks_.push_back(newDeathBlock_14);
	}

	for (int32_t i = 0; i < 4; ++i) {
		DeathBlock* newDeathBlock_15 = new DeathBlock();
		Vector3 deathBlockPosition_15 = mapChipField_->GetMapChipPositionByIndex(8 - i, 164 + i);
		newDeathBlock_15->Initialize(modelDeathBlock_, &viewProjection_, deathBlockPosition_15);
		deathBlocks_.push_back(newDeathBlock_15);
	}

	for (int32_t i = 0; i < 4; ++i) {
		DeathBlock* newDeathBlock_16 = new DeathBlock();
		Vector3 deathBlockPosition_16 = mapChipField_->GetMapChipPositionByIndex(1 + i, 167);
		newDeathBlock_16->Initialize(modelDeathBlock_, &viewProjection_, deathBlockPosition_16);
		deathBlocks_.push_back(newDeathBlock_16);
	}

	for (int32_t i = 0; i < 2; ++i) {
		DeathBlock* newDeathBlock_17 = new DeathBlock();
		Vector3 deathBlockPosition_17 = mapChipField_->GetMapChipPositionByIndex(3 + i + i + i, 174);
		newDeathBlock_17->Initialize(modelDeathBlock_, &viewProjection_, deathBlockPosition_17);
		deathBlocks_.push_back(newDeathBlock_17);
	}

	for (int32_t i = 0; i < 2; ++i) {
		DeathBlock* newDeathBlock_18 = new DeathBlock();
		Vector3 deathBlockPosition_18 = mapChipField_->GetMapChipPositionByIndex(5 + i + i + i, 189);
		newDeathBlock_18->Initialize(modelDeathBlock_, &viewProjection_, deathBlockPosition_18);
		deathBlocks_.push_back(newDeathBlock_18);
	}

	// ジャンプブロックの生成
	for (int32_t i = 0; i < 2; ++i) {
		JumpBlock* newJumpBlock = new JumpBlock();
		Vector3 jumpBlockPosition = mapChipField_->GetMapChipPositionByIndex(3 + i + i + i, 58);
		newJumpBlock->Initialize(modelJumpBlock_, &viewProjection_, jumpBlockPosition);
		jumpBlocks_.push_back(newJumpBlock);
	}

	for (int32_t i = 0; i < 2; ++i) {
		JumpBlock* newJumpBlock_2 = new JumpBlock();
		Vector3 jumpBlockPosition_2 = mapChipField_->GetMapChipPositionByIndex(2 + i + i + i, 147);
		newJumpBlock_2->Initialize(modelJumpBlock_, &viewProjection_, jumpBlockPosition_2);
		jumpBlocks_.push_back(newJumpBlock_2);
	}

	// ゴールの生成
	Goal* newGoal = new Goal();
	Vector3 goalPosition = mapChipField_->GetMapChipPositionByIndex(8, 198);
	newGoal->Initialize(modelGoal_, &viewProjection_, goalPosition);
	goals_.push_back(newGoal);

	phase_ = Phase::kPlay;
}

void GameScene::Update() {

	ChangePhase();

	switch (phase_) {
	case Phase::kPlay:

		worldTransformSkydome_.UpdateMatrix();

		worldTransformOneText_.UpdateMatrix();
		worldTransformTwoText_.UpdateMatrix();
		worldTransformThreeText_.UpdateMatrix();
		worldTransformClearText_.UpdateMatrix();

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
			audio_->StopWave(voiceHandle_);
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

		//audio_->StopWave(soundDataHandle_);
		}


		UpdateCamera();
		break;

	case Phase::kGoal:
		if (deathParticles_ && deathParticles_->IsFinished()) {
			finished_ = true;
			audio_->StopWave(voiceHandle_);
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
	
	modelOneText_->Draw(worldTransformOneText_, viewProjection_);
	modelTwoText_->Draw(worldTransformTwoText_, viewProjection_);
	modelThreeText_->Draw(worldTransformThreeText_, viewProjection_);
	modelClearText_->Draw(worldTransformClearText_, viewProjection_);

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
			//audio_->StopWave(soundDataHandle_);
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
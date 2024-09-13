#define NOMINMAX

#include "DirectXCommon.h"
#include "Input.h"
#include "TutorialScene.h"
#include <cmath>
#include <numbers>

TutorialScene::~TutorialScene() {
	delete modelSkydome_;
	delete modelPlayer_;
	delete modelDeathBlock_;
	delete modelJumpBlock_;
	delete modelGoal_;
	delete modelKeys_;
	delete modelSpaceText_;
	delete modelUpText_;
	delete modelLeftAndRightText_;
	delete modelRuleText_;
	delete modelRuleDescriptionText_1_;
	delete modelRuleDescriptionText_2_;
	delete modelDeathBlockText_;
	delete modelJumpBlockText_;
	delete modelGoalText_;
}

void TutorialScene::Initialize() {
	modelSkydome_ = Model::CreateFromOBJ("sky", true);
	// 3Dモデル
	modelPlayer_ = Model::CreateFromOBJ("player");
	modelDeathBlock_ = Model::CreateFromOBJ("deathBlock");
	modelJumpBlock_ = Model::CreateFromOBJ("jumpBlock");
	modelGoal_ = Model::CreateFromOBJ("goal");
	// テキスト
	modelKeys_ = Model::CreateFromOBJ("keysText", true);
	modelSpaceText_ = Model::CreateFromOBJ("spaceKeysText", true);
	modelUpText_ = Model::CreateFromOBJ("upKeysText", true);
	modelLeftAndRightText_ = Model::CreateFromOBJ("leftAndRightKeysText", true);
	modelRuleText_ = Model::CreateFromOBJ("ruleText", true);
	modelRuleDescriptionText_1_ = Model::CreateFromOBJ("ruleDescriptionText_1", true);
	modelRuleDescriptionText_2_ = Model::CreateFromOBJ("ruleDescriptionText_2", true);
	modelDeathBlockText_ = Model::CreateFromOBJ("obstructionText", true);
	modelJumpBlockText_ = Model::CreateFromOBJ("jumpText", true);
	modelGoalText_ = Model::CreateFromOBJ("goalText", true);

	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

	// モデル
	const float kPlayerScale = 8.0f;
	worldTransformPlayer_.Initialize();
	worldTransformPlayer_.scale_ = {kPlayerScale, kPlayerScale, kPlayerScale};
	worldTransformPlayer_.rotation_.y = 0.9f * std::numbers::pi_v<float>;
	worldTransformPlayer_.translation_.x = -27.0f;
	worldTransformPlayer_.translation_.y = +5.0f;

	const float kDeathBlock = 5.0f;
	worldTransformDeathBlock_.Initialize();
	worldTransformDeathBlock_.scale_ = {kDeathBlock, kDeathBlock, kDeathBlock};
	worldTransformDeathBlock_.rotation_.y = 0.9f * std::numbers::pi_v<float>;
	worldTransformDeathBlock_.translation_.x = +7.0f;
	worldTransformDeathBlock_.translation_.y = +12.0f;

	const float kJumpBlock = 5.0f;
	worldTransformJumpBlock_.Initialize();
	worldTransformJumpBlock_.scale_ = {kJumpBlock, kJumpBlock, kJumpBlock};
	worldTransformJumpBlock_.rotation_.y = 0.9f * std::numbers::pi_v<float>;
	worldTransformJumpBlock_.translation_.x = +16.0f;
	worldTransformJumpBlock_.translation_.y = +12.0f;

	const float kGoal = 5.0f;
	worldTransformGoal_.Initialize();
	worldTransformGoal_.scale_ = {kGoal, kGoal, kGoal};
	worldTransformGoal_.rotation_.y = 0.9f * std::numbers::pi_v<float>;
	worldTransformGoal_.translation_.x = +25.0f;
	worldTransformGoal_.translation_.y = +12.0f;

	// テキスト
	const float kTextKeys = 3.5f;
	worldTransformKeys_.Initialize();
	worldTransformKeys_.scale_ = {kTextKeys, kTextKeys, kTextKeys};
	worldTransformKeys_.rotation_.y = 0.99f * std::numbers::pi_v<float>;
	worldTransformKeys_.translation_.x = -23.0f;
	worldTransformKeys_.translation_.y = +15.0f;

	const float kTextSpaceKey = 2.5f;
	worldTransformSpaceKeys_.Initialize();
	worldTransformSpaceKeys_.scale_ = {kTextSpaceKey, kTextSpaceKey, kTextSpaceKey};
	worldTransformSpaceKeys_.rotation_.y = 0.99f * std::numbers::pi_v<float>;
	worldTransformSpaceKeys_.translation_.x = -22.0f;
	worldTransformSpaceKeys_.translation_.y = +11.0f;

	const float kTextUpKey = 2.5f;
	worldTransformUpKeys_.Initialize();
	worldTransformUpKeys_.scale_ = {kTextUpKey, kTextUpKey, kTextUpKey};
	worldTransformUpKeys_.rotation_.y = 0.99f * std::numbers::pi_v<float>;
	worldTransformUpKeys_.translation_.x = -13.0f;
	worldTransformUpKeys_.translation_.y = +6.0f;

	const float kTextLeftAndRightKey = 2.5f;
	worldTransformLeftAndRightKeys_.Initialize();
	worldTransformLeftAndRightKeys_.scale_ = {kTextLeftAndRightKey, kTextLeftAndRightKey, kTextLeftAndRightKey};
	worldTransformLeftAndRightKeys_.rotation_.y = 0.99f * std::numbers::pi_v<float>;
	worldTransformLeftAndRightKeys_.translation_.x = -11.5f;
	worldTransformLeftAndRightKeys_.translation_.y = +2.0f;

	const float kTextRule = 3.5f;
	worldTransformRule_.Initialize();
	worldTransformRule_.scale_ = {kTextRule, kTextRule, kTextRule};
	worldTransformRule_.rotation_.y = 0.99f * std::numbers::pi_v<float>;
	worldTransformRule_.translation_.x = -25.0f;
	worldTransformRule_.translation_.y = -6.0f;

	const float kTextRuleDescription_1 = 2.0f;
	worldTransformRuleDescription_1_.Initialize();
	worldTransformRuleDescription_1_.scale_ = {kTextRuleDescription_1, kTextRuleDescription_1, kTextRuleDescription_1};
	worldTransformRuleDescription_1_.rotation_.y = 0.99f * std::numbers::pi_v<float>;
	worldTransformRuleDescription_1_.translation_.x = 0.0f;
	worldTransformRuleDescription_1_.translation_.y = -11.0f;

	const float kTextRuleDescription_2 = 2.0f;
	worldTransformRuleDescription_2_.Initialize();
	worldTransformRuleDescription_2_.scale_ = {kTextRuleDescription_2, kTextRuleDescription_2, kTextRuleDescription_2};
	worldTransformRuleDescription_2_.rotation_.y = 0.99f * std::numbers::pi_v<float>;
	worldTransformRuleDescription_2_.translation_.x = -6.0f;
	worldTransformRuleDescription_2_.translation_.y = -16.0f;

	const float kDeathBlockText = 2.0f;
	worldTransformDeathBlockText_.Initialize();
	worldTransformDeathBlockText_.scale_ = {kDeathBlockText, kDeathBlockText, kDeathBlockText};
	worldTransformDeathBlockText_.rotation_.y = 0.99f * std::numbers::pi_v<float>;
	worldTransformDeathBlockText_.translation_.x = +7.0f;
	worldTransformDeathBlockText_.translation_.y = +16.5f;

	const float kJumpBlockText = 2.0f;
	worldTransformJumpBlockText_.Initialize();
	worldTransformJumpBlockText_.scale_ = {kJumpBlockText, kJumpBlockText, kJumpBlockText};
	worldTransformJumpBlockText_.rotation_.y = 0.99f * std::numbers::pi_v<float>;
	worldTransformJumpBlockText_.translation_.x = +16.0f;
	worldTransformJumpBlockText_.translation_.y = +16.5f;

	const float kGoalText = 2.0f;
	worldTransformGoalText_.Initialize();
	worldTransformGoalText_.scale_ = {kGoalText, kGoalText, kGoalText};
	worldTransformGoalText_.rotation_.y = 0.99f * std::numbers::pi_v<float>;
	worldTransformGoalText_.translation_.x = +25.0f;
	worldTransformGoalText_.translation_.y = +16.5f;

	worldTransformSkydome_.Initialize();
}

void TutorialScene::Update() {
	if (Input::GetInstance()->PushKey(DIK_SPACE) != 0 && !Input ::GetInstance()->TriggerKey(DIK_SPACE) == 0) {
		finished_ = true;
	}

	viewProjection_.TransferMatrix();
	worldTransformPlayer_.UpdateMatrix();
	worldTransformDeathBlock_.UpdateMatrix();
	worldTransformJumpBlock_.UpdateMatrix();
	worldTransformGoal_.UpdateMatrix();
	worldTransformKeys_.UpdateMatrix();
	worldTransformSpaceKeys_.UpdateMatrix();
	worldTransformUpKeys_.UpdateMatrix();
	worldTransformLeftAndRightKeys_.UpdateMatrix();
	worldTransformRule_.UpdateMatrix();
	worldTransformRuleDescription_1_.UpdateMatrix();
	worldTransformRuleDescription_2_.UpdateMatrix();
	worldTransformDeathBlockText_.UpdateMatrix();
	worldTransformJumpBlockText_.UpdateMatrix();
	worldTransformGoalText_.UpdateMatrix();
}

void TutorialScene::Draw() {
	DirectXCommon* dxCommon_ = DirectXCommon::GetInstance();
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	Model::PreDraw(commandList);

	modelSkydome_->Draw(worldTransformSkydome_, viewProjection_);

	// モデル描画
	modelPlayer_->Draw(worldTransformPlayer_, viewProjection_);
	modelDeathBlock_->Draw(worldTransformDeathBlock_, viewProjection_);
	modelJumpBlock_->Draw(worldTransformJumpBlock_, viewProjection_);
	modelGoal_->Draw(worldTransformGoal_, viewProjection_);

	// テキスト描画
	modelKeys_->Draw(worldTransformKeys_, viewProjection_);
	modelSpaceText_->Draw(worldTransformSpaceKeys_, viewProjection_);
	modelUpText_->Draw(worldTransformUpKeys_, viewProjection_);
	modelLeftAndRightText_->Draw(worldTransformLeftAndRightKeys_, viewProjection_);
	modelRuleText_->Draw(worldTransformRule_, viewProjection_);
	modelRuleDescriptionText_1_->Draw(worldTransformRuleDescription_1_, viewProjection_);
	modelRuleDescriptionText_2_->Draw(worldTransformRuleDescription_2_, viewProjection_);
	modelDeathBlockText_->Draw(worldTransformDeathBlockText_, viewProjection_);
	modelJumpBlockText_->Draw(worldTransformJumpBlockText_, viewProjection_);
	modelGoalText_->Draw(worldTransformGoalText_, viewProjection_);

	Model::PostDraw();
}
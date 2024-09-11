#define NOMINMAX

#include "DirectXCommon.h"
#include "Input.h"
#include "TutorialScene.h"
#include <cmath>
#include <numbers>

TutorialScene::~TutorialScene() {
	delete modelSkydome_;
	delete modelPlayer_;
	delete modelKeys_;
	delete modelSpaceText_;
	delete modelUpText_;
	delete modelLeftAndRightText_;
	delete modelRuleText_;
	delete modelRuleDescriptionText_1_;
	delete modelRuleDescriptionText_2_;
}

void TutorialScene::Initialize() {
	modelSkydome_ = Model::CreateFromOBJ("sky", true);
	modelPlayer_ = Model::CreateFromOBJ("player");
	modelKeys_ = Model::CreateFromOBJ("keysText", true);
	modelSpaceText_ = Model::CreateFromOBJ("spaceKeysText", true);
	modelUpText_ = Model::CreateFromOBJ("upKeysText", true);
	modelLeftAndRightText_ = Model::CreateFromOBJ("leftAndRightKeysText", true);
	modelRuleText_ = Model::CreateFromOBJ("ruleText", true);
	modelRuleDescriptionText_1_ = Model::CreateFromOBJ("ruleDescriptionText_1", true);
	modelRuleDescriptionText_2_ = Model::CreateFromOBJ("ruleDescriptionText_2", true);
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

	const float kPlayerScale = 8.0f;
	worldTransformPlayer_.Initialize();
	worldTransformPlayer_.scale_ = {kPlayerScale, kPlayerScale, kPlayerScale};
	worldTransformPlayer_.rotation_.y = 0.9f * std::numbers::pi_v<float>;
	worldTransformPlayer_.translation_.x = -23.0f;
	worldTransformPlayer_.translation_.y = +5.0f;

	const float kTextKeys = 5.0f;
	worldTransformKeys_.Initialize();
	worldTransformKeys_.scale_ = {kTextKeys, kTextKeys, kTextKeys};
	worldTransformKeys_.rotation_.y = 0.99f * std::numbers::pi_v<float>;
	worldTransformKeys_.translation_.x = -25.0f;
	worldTransformKeys_.translation_.y = +15.0f;

	const float kTextSpaceKey = 3.0f;
	worldTransformSpaceKeys_.Initialize();
	worldTransformSpaceKeys_.scale_ = {kTextSpaceKey, kTextSpaceKey, kTextSpaceKey};
	worldTransformSpaceKeys_.rotation_.y = 0.99f * std::numbers::pi_v<float>;
	worldTransformSpaceKeys_.translation_.x = -18.0f;
	worldTransformSpaceKeys_.translation_.y = +11.0f;

	const float kTextUpKey = 3.0f;
	worldTransformUpKeys_.Initialize();
	worldTransformUpKeys_.scale_ = {kTextUpKey, kTextUpKey, kTextUpKey};
	worldTransformUpKeys_.rotation_.y = 0.99f * std::numbers::pi_v<float>;
	worldTransformUpKeys_.translation_.x = -12.0f;
	worldTransformUpKeys_.translation_.y = +6.0f;

	const float kTextLeftAndRightKey = 3.0f;
	worldTransformLeftAndRightKeys_.Initialize();
	worldTransformLeftAndRightKeys_.scale_ = {kTextLeftAndRightKey, kTextLeftAndRightKey, kTextLeftAndRightKey};
	worldTransformLeftAndRightKeys_.rotation_.y = 0.99f * std::numbers::pi_v<float>;
	worldTransformLeftAndRightKeys_.translation_.x = -11.0f;
	worldTransformLeftAndRightKeys_.translation_.y = +3.0f;

	const float kTextRule = 5.0f;
	worldTransformRule_.Initialize();
	worldTransformRule_.scale_ = {kTextRule, kTextRule, kTextRule};
	worldTransformRule_.rotation_.y = 0.99f * std::numbers::pi_v<float>;
	worldTransformRule_.translation_.x = -26.0f;
	worldTransformRule_.translation_.y = -6.0f;

	const float kTextRuleDescription_1 = 3.0f;
	worldTransformRuleDescription_1_.Initialize();
	worldTransformRuleDescription_1_.scale_ = {kTextRuleDescription_1, kTextRuleDescription_1, kTextRuleDescription_1};
	worldTransformRuleDescription_1_.rotation_.y = 0.99f * std::numbers::pi_v<float>;
	worldTransformRuleDescription_1_.translation_.x = 0.0f;
	worldTransformRuleDescription_1_.translation_.y = -11.0f;

	const float kTextRuleDescription_2 = 3.0f;
	worldTransformRuleDescription_2_.Initialize();
	worldTransformRuleDescription_2_.scale_ = {kTextRuleDescription_2, kTextRuleDescription_2, kTextRuleDescription_2};
	worldTransformRuleDescription_2_.rotation_.y = 0.99f * std::numbers::pi_v<float>;
	worldTransformRuleDescription_2_.translation_.x = -6.0f;
	worldTransformRuleDescription_2_.translation_.y = -16.0f;

	worldTransformSkydome_.Initialize();
}

void TutorialScene::Update() {
	if (Input::GetInstance()->PushKey(DIK_SPACE) != 0 && !Input ::GetInstance()->TriggerKey(DIK_SPACE) == 0) {
		finished_ = true;
	}

	viewProjection_.TransferMatrix();
	worldTransformPlayer_.UpdateMatrix();
	worldTransformKeys_.UpdateMatrix();
	worldTransformSpaceKeys_.UpdateMatrix();
	worldTransformUpKeys_.UpdateMatrix();
	worldTransformLeftAndRightKeys_.UpdateMatrix();
	worldTransformRule_.UpdateMatrix();
	worldTransformRuleDescription_1_.UpdateMatrix();
	worldTransformRuleDescription_2_.UpdateMatrix();
}

void TutorialScene::Draw() {
	DirectXCommon* dxCommon_ = DirectXCommon::GetInstance();
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	Model::PreDraw(commandList);

	modelSkydome_->Draw(worldTransformSkydome_, viewProjection_);
	modelPlayer_->Draw(worldTransformPlayer_, viewProjection_);
	modelKeys_->Draw(worldTransformKeys_, viewProjection_);
	modelSpaceText_->Draw(worldTransformSpaceKeys_, viewProjection_);
	modelUpText_->Draw(worldTransformUpKeys_, viewProjection_);
	modelLeftAndRightText_->Draw(worldTransformLeftAndRightKeys_, viewProjection_);
	modelRuleText_->Draw(worldTransformRule_, viewProjection_);
	modelRuleDescriptionText_1_->Draw(worldTransformRuleDescription_1_, viewProjection_);
	modelRuleDescriptionText_2_->Draw(worldTransformRuleDescription_2_, viewProjection_);

	Model::PostDraw();
}
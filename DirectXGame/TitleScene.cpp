#define NOMINMAX

#include "TitleScene.h"
#include "DirectXCommon.h"
#include "Input.h"
#include <cmath>
#include <numbers>

TitleScene::~TitleScene() {
	delete modelTitle_;
	delete modelPress_;
	delete modelPlayer_;
	delete modelSkydome_;
	delete modelGameSelect_;
	delete modelTutorialSelect_;
	delete modelSelectWall_;
	delete modelUpAndDownText_;
	delete modelSpaceText_;
}

void TitleScene::Initialize() {
	modelTitle_ = Model::CreateFromOBJ("text", true);
	modelSkydome_ = Model::CreateFromOBJ("sky", true);
	modelGameSelect_ = Model::CreateFromOBJ("gameSelectText", true);
	modelTutorialSelect_ = Model::CreateFromOBJ("tutorialSelectText", true);
	modelSelectWall_ = Model::CreateFromOBJ("selectWall", true);
	modelUpAndDownText_ = Model::CreateFromOBJ("upAndDownText", true);
	modelSpaceText_ = Model::CreateFromOBJ("spaceText", true);
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

	const float kTextTitle = 10.0f;
	worldTransformTitle_.Initialize();
	worldTransformTitle_.scale_ = {kTextTitle, kTextTitle, kTextTitle};
	worldTransformTitle_.rotation_.y = 0.99f * std::numbers::pi_v<float>;

	const float kTextGameSelect = 7.0f;
	worldTransformGameSelect_.Initialize();
	worldTransformGameSelect_.scale_ = {kTextGameSelect, kTextGameSelect, kTextGameSelect};
	worldTransformGameSelect_.rotation_.y = 0.99f * std::numbers::pi_v<float>;
	worldTransformGameSelect_.translation_.y = -7.0f;

	const float kTextTutorialSelect = 7.0f;
	worldTransformTutorialSelect_.Initialize();
	worldTransformTutorialSelect_.scale_ = {kTextTutorialSelect, kTextTutorialSelect, kTextTutorialSelect};
	worldTransformTutorialSelect_.rotation_.y = 0.99f * std::numbers::pi_v<float>;
	worldTransformTutorialSelect_.translation_.y = -15.0f;

	const float kSelectWall = 7.0f;
	worldTransformSelectWall_.Initialize();
	worldTransformSelectWall_.scale_ = {kSelectWall, kSelectWall, kSelectWall};
	worldTransformSelectWall_.rotation_.y = 0.99f * std::numbers::pi_v<float>;
	if (select_ == 0) {
		worldTransformSelectWall_.translation_.y = -8.0f;
	}

	const float kTextUpAndDown = 3.0f;
	worldTransformUpAndDownText_.Initialize();
	worldTransformUpAndDownText_.scale_ = {kTextUpAndDown, kTextUpAndDown, kTextUpAndDown};
	worldTransformUpAndDownText_.rotation_.y = 0.99f * std::numbers::pi_v<float>;
	worldTransformUpAndDownText_.translation_.x = -33.0f;
	worldTransformUpAndDownText_.translation_.y = -15.0f;

	const float kTextSpace = 2.5f;
	worldTransformSpaceText_.Initialize();
	worldTransformSpaceText_.scale_ = {kTextSpace, kTextSpace, kTextSpace};
	worldTransformSpaceText_.rotation_.y = 0.99f * std::numbers::pi_v<float>;
	worldTransformSpaceText_.translation_.x = -28.0f;
	worldTransformSpaceText_.translation_.y = -16.2f;

	worldTransformSkydome_.Initialize();
}

void TitleScene::Update() {
	counter_ += 1.0f / 60.0f;
	counter_ = std::fmod(counter_, kTimeTitleMove);

	float angle = counter_ / kTimeTitleMove * 2.0f * std::numbers::pi_v<float>;

	worldTransformTitle_.translation_.y = std::sin(angle) + 9.0f;

	if (Input::GetInstance()->PushKey(DIK_DOWN) != 0 && !Input ::GetInstance()->TriggerKey(DIK_DOWN) == 0) {
		select_ = 1;
	}
	if (select_ == 1) {
		worldTransformSelectWall_.translation_.y = -16.0f;
	}
	if (Input::GetInstance()->PushKey(DIK_UP) != 0 && !Input ::GetInstance()->TriggerKey(DIK_UP) == 0) {
		select_ = 0;
	}
	if (select_ == 0) {
		worldTransformSelectWall_.translation_.y = -8.0f;
	}

	if (select_ == 0 && Input::GetInstance()->PushKey(DIK_SPACE) != 0 && !Input ::GetInstance()->TriggerKey(DIK_SPACE) == 0) {
		game_ = true;
	}
	if (select_ == 1 && Input::GetInstance()->PushKey(DIK_SPACE) != 0 && !Input ::GetInstance()->TriggerKey(DIK_SPACE) == 0) {
		tutorial_ = true;
	}

	viewProjection_.TransferMatrix();
	worldTransformTitle_.UpdateMatrix();
	worldTransformGameSelect_.UpdateMatrix();
	worldTransformTutorialSelect_.UpdateMatrix();
	worldTransformSelectWall_.UpdateMatrix();
	worldTransformUpAndDownText_.UpdateMatrix();
	worldTransformSpaceText_.UpdateMatrix();
}

void TitleScene::Draw() {
	DirectXCommon* dxCommon_ = DirectXCommon::GetInstance();
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	Model::PreDraw(commandList);

	modelSkydome_->Draw(worldTransformSkydome_, viewProjection_);
	modelTitle_->Draw(worldTransformTitle_, viewProjection_);
	modelGameSelect_->Draw(worldTransformGameSelect_, viewProjection_);
	modelTutorialSelect_->Draw(worldTransformTutorialSelect_, viewProjection_);
	modelSelectWall_->Draw(worldTransformSelectWall_, viewProjection_);
	modelUpAndDownText_->Draw(worldTransformUpAndDownText_, viewProjection_);
	modelSpaceText_->Draw(worldTransformSpaceText_, viewProjection_);

	Model::PostDraw();
}
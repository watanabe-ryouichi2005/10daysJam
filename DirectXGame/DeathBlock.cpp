#include "DeathBlock.h"
#include "myMath.h"
#include <cassert>
#include <numbers>

void DeathBlock::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position) {
	assert(model);

	model_ = model;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> * 3.0f / 2.0f;
	viewProjection_ = viewProjection;
}

void DeathBlock::Update() {
	// 行列計算
	worldTransform_.UpdateMatrix();
}

void DeathBlock::Draw() {
	// モデル描画
	model_->Draw(worldTransform_, *viewProjection_);
}

Vector3 DeathBlock::GetWorldPosition() {
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}

AABB DeathBlock::GetAABB() {
	Vector3 worldPos = GetWorldPosition();

	AABB aabb;

	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};

	return aabb;
}

void DeathBlock::OnCollision(const Player* player) { (void)player; }
#pragma once

#include "AABB.h"
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class Player;

class JumpBlock {
public:
	/// <summary>
	/// 敵
	/// </summary>
	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);

	void Update();

	void Draw();

	// ワールド座標を取得
	Vector3 GetWorldPosition();

	AABB GetAABB();

	void OnCollision(const Player* player);

private:
	static inline const float kWalkMotionTime = 1.0f;
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;

	Model* model_ = nullptr;
	WorldTransform worldTransform_;
	ViewProjection* viewProjection_ = nullptr;
	Vector3 velocity_ = {};
	float walkTimer = 0.0f;
};
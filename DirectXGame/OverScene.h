#pragma once

#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class OverScene {
public:
	~OverScene();

	void Initialize();

	void Update();

	void Draw();

	bool IsFinished() const { return finished_; };

private:
	static inline const float kTimeTitleMove = 2.0f;

	ViewProjection viewProjection_;
	WorldTransform worldTransformTitle_;
	WorldTransform worldTransformSkydome_;

	Model* modelTitle_ = nullptr;
	Model* modelSkydome_ = nullptr;

	float counter_ = 0.0f;
	bool finished_ = false;
};
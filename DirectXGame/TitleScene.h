#pragma once

#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class TitleScene { 
public:
	~TitleScene();

	void Initialize();

	void Update();

	void Draw();

	bool IsFinished() const { return finished_; };

private:
	static inline const float kTimeTitleMove = 2.0f;

	ViewProjection viewProjection_;
	WorldTransform worldTransformTitle_;
	WorldTransform worldTransformPress_;
	WorldTransform worldTransformPlayer_;
	WorldTransform worldTransformSkydome_;
	WorldTransform worldTransformGameSelect_;
	WorldTransform worldTransformTutorialSelect_;
	WorldTransform worldTransformSelectWall_;

	Model* modelTitle_ = nullptr;
	Model* modelPress_ = nullptr;
	Model* modelPlayer_ = nullptr;
	Model* modelSkydome_ = nullptr;
	Model* modelGameSelect_ = nullptr;
	Model* modelTutorialSelect_ = nullptr;
	Model* modelSelectWall_ = nullptr;

	float counter_ = 0.0f;
	bool finished_ = false;
	bool select_ = 0;
};
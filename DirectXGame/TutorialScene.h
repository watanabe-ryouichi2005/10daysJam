#pragma once

#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class TutorialScene {
public:
	~TutorialScene();

	void Initialize();

	void Update();

	void Draw();

	bool IsFinished() const { return finished_; };

private:
	static inline const float kTimeTitleMove = 2.0f;

	ViewProjection viewProjection_;
	WorldTransform worldTransformSkydome_;
	WorldTransform worldTransformPlayer_;
	WorldTransform worldTransformDeathBlock_;
	WorldTransform worldTransformJumpBlock_;
	WorldTransform worldTransformGoal_;
	WorldTransform worldTransformKeys_;
	WorldTransform worldTransformSpaceKeys_;
	WorldTransform worldTransformUpKeys_;
	WorldTransform worldTransformLeftAndRightKeys_;
	WorldTransform worldTransformRule_;
	WorldTransform worldTransformRuleDescription_1_;
	WorldTransform worldTransformRuleDescription_2_;
	WorldTransform worldTransformDeathBlockText_;
	WorldTransform worldTransformJumpBlockText_;
	WorldTransform worldTransformGoalText_;
	
	Model* modelSkydome_ = nullptr;
	Model* modelPlayer_ = nullptr;
	Model* modelDeathBlock_ = nullptr;
	Model* modelJumpBlock_ = nullptr;
	Model* modelGoal_ = nullptr;
	Model* modelKeys_ = nullptr;
	Model* modelSpaceText_ = nullptr;
	Model* modelUpText_ = nullptr;
	Model* modelLeftAndRightText_ = nullptr;
	Model* modelRuleText_ = nullptr;
	Model* modelRuleDescriptionText_1_ = nullptr;
	Model* modelRuleDescriptionText_2_ = nullptr;
	Model* modelDeathBlockText_ = nullptr;
	Model* modelJumpBlockText_ = nullptr;
	Model* modelGoalText_ = nullptr;

	float counter_ = 0.0f;
	bool finished_ = false;
};
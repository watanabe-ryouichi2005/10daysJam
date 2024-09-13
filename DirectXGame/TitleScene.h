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

	bool IsGame() const { return game_; };
	bool IsTutorial() const { return tutorial_; };

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
	WorldTransform worldTransformUpAndDownText_;
	WorldTransform worldTransformSpaceText_;

	Model* modelTitle_ = nullptr;
	Model* modelPress_ = nullptr;
	Model* modelPlayer_ = nullptr;
	Model* modelSkydome_ = nullptr;
	Model* modelGameSelect_ = nullptr;
	Model* modelTutorialSelect_ = nullptr;
	Model* modelSelectWall_ = nullptr;
	Model* modelUpAndDownText_ = nullptr;
	Model* modelSpaceText_ = nullptr;

	float counter_ = 0.0f;
	bool game_ = false;
	bool tutorial_ = false;
	bool select_ = 0;
};
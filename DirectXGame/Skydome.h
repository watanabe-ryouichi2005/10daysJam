#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class Skydome {
public:
	void Initialize();

	void Update();

	void Draw();

private:
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;

	Model* model_ = nullptr;
};
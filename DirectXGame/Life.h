#include"Model.h"
#include"WorldTransform.h"
#include"ViewProjection.h"
#include"Player.h"
class CameraContoller;
class Life {
public:
	void Init();
	void Update();
	void Draw();

private:
// ビュープロジェクション
	ViewProjection viewProjection_;
	Player* target_ = nullptr;

	// 追従対象とカメラの座標の差（オフセット）
	Vector3 targetOffset_ = {0, 0, -17.4f};

	// カメラ移動範囲
	Rect movableArea_ = {0, 100, 0, 100};
	Vector3 destination_;
	static inline const Rect targetMargin = {-9.0f, 9.0f, -5.0f, 5.0f};
	static inline const float kInterpolationRate_ = 0.1f;

};
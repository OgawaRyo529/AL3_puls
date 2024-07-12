#include<array>
#include "Model.h"
#include "viewProjection.h"
#include"DeathObject.h"

//class DeathParticles {
//public:
//
//void Initialize(const Vector3& position, ViewProjection* viewProjection,ObjectColor objectColor_);
//
//void Update();
//
//void Draw();
//
//
//private:
//	
//	// モデル
//	Model* model_ = nullptr;
//	Model* modeDeath_;
//	ViewProjection* viewProjection_ = nullptr;
//
//	static inline const uint32_t kNumParticles = 8;
//	std::array<WorldTransform, kNumParticles>worldTransform_;
//	//存続時間(消滅までの時間)<秒>
//	static inline const float kDuration = 2.0f;
//	//移動の速さ
//	static inline const float kSpeed = 0.05f;
//	//分割した1個分の角度
//	static inline const float kAngleUnit = 2 / 8;
//
//	//終了フラグ
//	bool isFinished_ = false;
//	//継続時間カウント
//	float counter_ = 0.0f;
//
//};
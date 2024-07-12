#include <algorithm>
#include"DeathParticles.h"
#include"MathUtilityForText.h"

//void DeathParticles::Initialize(const Vector3& position, ViewProjection* viewProjection, ObjectColor objectColor_) {
//	for (auto& worldTransform : worldTransform_) {
//		worldTransform, Initialize( position, viewProjection,objectColor_);
//		worldTransform.translation_= position;
//	}
//	objectColor_.Initalize();
//	color_={ 1,1,1,1 };
//}
//
//void DeathParticles::Update() {
//	for (uint32_t i = 0; i < 8; ++i) {
//		Vector3 velocity = { kSpeed,0,0 };
//		float angle = kAngleUnit * i;
//		Matrix4x4 matrixRotation = MakeRotateZMatrix(angle);
//		velocity = Transform(velocity, matrixRotation);
//		worldTransform_[i].translation_ += velocity;
//	}
//	counter_ += 1.0f / 60.0f;
//
//	color_.w = std::clamp(counter_, 0.0f, 1.0f);
//	//色変更オブジェクトに色の数値を設定する
//	objectColor_.SetColor(color_);
//	//色変更オブジェクトをVRAMに転送
//	objectColor_.TransferMatrix();
//
//
//	//存続時間の上限に達したら
//	if (counter_ >= kDuration) {
//		counter_ = kDuration;
//		//終了扱いにする
//		isFinished_ = true;
//	}
//	//終了なら何もしない
//	if (isFinished_) {
//		return;
//	}
//
//}
//
//void DeathParticles::Draw() {
//	for (auto& worldTransform : worldTransform_) {
//		model_->Draw(worldTransform, *viewProjection_, &objectColor_);
//	}
//	//終了なら何もしない
//	if (isFinished_) {
//		return;
//	}
//}
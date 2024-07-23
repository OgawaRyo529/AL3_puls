#pragma once
#include "GameScene.h"
#include "TextureManager.h"
#include"CameraController.h"
#include <cassert>
#include<cstdint>

GameScene::GameScene() {}

GameScene::~GameScene() {
	for (Enemy* enemy : enemies_) {
		delete enemy;
	}
	delete player_;
	delete model_;
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
			worldTransformBlock = nullptr;
		}
	}
	worldTransformBlocks_.clear();

	delete modelEnemy_;
	delete modelPlayer_;
	delete modelBlock_;
	delete debugCamera_;
	delete modelSkydome_;
	delete mapChipField_;
	delete cameraController;
}


void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

	// 3Dモデルの生成
	// ファイル名を指定してテクスチャを読み込む
	//textureHandle_ = TextureManager::Load("block.jpg");
	// 3Dモデルの生成
	model_ = Model::Create();
	modelSkydome_ = Model::CreateFromOBJ("sphere", true);
	modelEnemy_ = Model::CreateFromOBJ("enemy");
	modelBlock_ = Model::CreateFromOBJ("block");
	modelPlayer_ = Model::CreateFromOBJ("player");

	// mapChipField_->LoadMap
	// ワールドトランスフォームの初期化
	wordTransform_.Initialize();
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();
	// 自キャラの生成
	player_ = new Player();
	// 自キャラの初期化
	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(5, 16);
	player_->Initialize(modelPlayer_ , &viewProjection_, playerPosition);

	viewProjection_.Initialize();
	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	Skydome_ = new skydome();
	Skydome_->Initialize(modelSkydome_, &viewProjection_);
	///要素数(切り取り後)
	mapChipField_ = new MapChipField;
	mapChipField_->LoadMapChipCsv("Resources/blokcs.csv");

	player_->SetMapChipField(mapChipField_);

	GenerateBlocks();

	cameraController = new CameraController;
	cameraController->Initialize();
	cameraController->SetTarget(player_);
	cameraController->Reset();

	CameraController::Rect cameraArea = { 12.0f, 100 - 12.0f, 6.0f, 6.0f };
	cameraController->SetMovableArea(cameraArea);

	Enemy*newEnemy= new Enemy();
	Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(14, 28);
	newEnemy->Initialize(modelEnemy_, &viewProjection_, enemyPosition);

	enemies_.push_back(newEnemy);

	//deathParticles_ = new DeathParticles;
	//deathParticles_->Initialize(modeDeath_, &viewProjection_,&objectColor_);
}


//void GameScene::ChangePhase() {
//	switch (phase_)
//	{
//	default:
//		break;
//	}
//	if (deathParticles_ && deathParticles_->IsFinished()) {
//		finished_ = true;
//	}
//}

void GameScene::Update() {

#ifdef _DEBUG
	if (input_->TriggerKey(DIK_SPACE)) {
		//isDebugCameraActive_ = !isDebugCameraActive_;
		if (isDebugCameraActive_ == true)
			isDebugCameraActive_ = false;
		else
			isDebugCameraActive_ = true;
	}
#endif
	cameraController->Update();
	// カメラ処理
	if (isDebugCameraActive_) {
		// デバッグカメラの更新
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	}
	else {
		viewProjection_.UpdateMatrix();
		viewProjection_.matView = cameraController->GetViewProjection().matView;
		viewProjection_.matProjection = cameraController->GetViewProjection().matProjection;
		// ビュープロジェクション行列の更新と転送
		viewProjection_.TransferMatrix();
	}
	// 自キャラの更新
	player_->Update();

	for (Enemy* enemy : enemies_) {
		enemy->Update();
	}
	//enemy_->Update();
	Skydome_->Update();

	
	// ブロック更新
	for (std::vector<WorldTransform*> worldTransformBlockTate : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlockYoko : worldTransformBlockTate) {
			if (!worldTransformBlockYoko)
				continue;

			// アフィン変換行列の作成
			worldTransformBlockYoko->UpdateMatrix();
			worldTransformBlockYoko->matWorld_ =
				MakeAffineMatrix(worldTransformBlockYoko->scale_, worldTransformBlockYoko->rotation_, worldTransformBlockYoko->translation_);

			// 定数バッファに転送
			worldTransformBlockYoko->TransferMatrix();

		}
	}
	CheckAllCollisions();
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	// 自キャラの描画
	Skydome_->Draw();
	// 縦横ブロック描画
	for (std::vector<WorldTransform*> &worldTransformBlockTate : worldTransformBlocks_) {
		for (WorldTransform*& worldTransformBlockYoko : worldTransformBlockTate) {
			if (!worldTransformBlockYoko)
				continue;

			modelBlock_->Draw(*worldTransformBlockYoko, viewProjection_);
		}
	}
	player_->Draw();
	for (Enemy* enemy : enemies_) {
		enemy->Draw();
	}
	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
void GameScene::GenerateBlocks() {

	// 要素数
	uint32_t kNumBlockVirtical = mapChipField_->GetNumBlockVirtical();
	uint32_t kNumBlockHorizontal = mapChipField_->GetNumBlockHorizontal();
	// ブロック1個分の横幅
	//const float kBlockWidth = 2.0f;
	//const float kBlockHeight = 2.0f;
	// 要素数を変更する
	worldTransformBlocks_.resize(kNumBlockHorizontal);

	// キューブの生成
	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		worldTransformBlocks_[i].resize(kNumBlockHorizontal);
	}

	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {
			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kBlock) {
				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransformBlocks_[i][j] = worldTransform;
				worldTransformBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
			}
		}
	}
}

void GameScene::CheckAllCollisions() {
	//判定対象1と2の座標
	AABB aabb1, aabb2;

#pragma region 
	//自キャラと敵キャラの当たり判定
	{
		aabb1 = player_->GetAABB();

		for (Enemy* enemy : enemies_) {

			aabb2 = enemy->GetAABB();

			if (IsCollision(aabb1, aabb2)) {

				player_->OnCollision(enemy);

				enemy->OnCollision(player_);
			}
		}
	}
#pragma endregion
}
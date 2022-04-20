#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include<random>

using namespace DirectX;
//コンストラクタ
GameScene::GameScene() {}
//デストラクタ
GameScene::~GameScene() { 
	delete model_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();
	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("mario.jpg");
	//3Dモデルの生成
	model_ = Model::Create();
	//worldtransform_.scale_ = {5.0f, 5.0f, 5.0f};
	//worldtransform_.rotation_ = {XM_PI / 4.0f, XM_PI / 4.0f, 0.0f};
	//worldtransform_.translation_ = {10.0f, 10.0f, 10.0f};
	//ワールドトランスフォームの初期化
	//worldtransform_.Initialize();
	viewprojection_.Initialize();
	std::random_device seed_gen;
	std::mt19937_64 engine(seed_gen());
	std::uniform_real_distribution<float> rotDist(0.0f, XM_2PI);
	std::uniform_real_distribution<float> posDist(-10.0f, 10.0f);

	for (size_t i = 0; i < _countof(worldtransform_);i++) 
	{
		worldtransform_[i].scale_ = {1.0f,1.0f,1.0f,};
		worldtransform_[i].rotation_ = {rotDist(engine), rotDist(engine), rotDist(engine)};
		worldtransform_[i].translation_ = {posDist(engine), posDist(engine), posDist(engine)};
		worldtransform_[i].Initialize();
	}

	viewprojection_.Initialize();


}

void GameScene::Update() { 
	////移動
	//XMFLOAT3 move = {0, 0, 0};

	//const float kEyeSpeed = 0.2f;

	//if (input_->PushKey(DIK_W)) {
	//	move = {0, 0, kEyeSpeed};
	//
	//} else if (input_->PushKey(DIK_S)) {
	//	move = {0, 0, -kEyeSpeed};
	//}
	//viewprojection_.eye.x += move.x;
	//viewprojection_.eye.y += move.y;
	//viewprojection_.eye.z += move.z;

	//viewprojection_.UpdateMatrix();

	//debugText_->SetPos(50, 50);
	//debugText_->Printf(
	//  "eye(%f,%f,%f)", viewprojection_.eye.x, viewprojection_.eye.y, viewprojection_.eye.z);
	////注視

	//const float kTargetSpeed = 0.2f;

	//if (input_->PushKey(DIK_LEFT)) {
	//	move = {-kTargetSpeed,0,0};

	//} else if (input_->PushKey(DIK_RIGHT)) {
	//	move = {kTargetSpeed,0,0};
	//}
	//viewprojection_.target.x += move.x;
	//viewprojection_.target.y += move.y;
	//viewprojection_.target.z += move.z;

	//viewprojection_.UpdateMatrix();

	//debugText_->SetPos(50, 70);
	//debugText_->Printf(
	//  "eye(%f,%f,%f)", viewprojection_.target.x, viewprojection_.target.y, viewprojection_.target.z);
	////回転
	//const float kUpRotSpeed = 0.05f;

	//if (input_->PushKey(DIK_SPACE)) {
	//	viewAngle += kUpRotSpeed;

	//	viewAngle = fmodf(viewAngle, XM_2PI);
	//}
	//viewprojection_.up = {cosf(viewAngle), sinf(viewAngle), 0.0f};

	//viewprojection_.UpdateMatrix();

	if (input_->PushKey(DIK_UP)) {
		viewprojection_.fovAngleY += 0.01f;
		viewprojection_.fovAngleY = min(viewprojection_.fovAngleY, XM_PI);
	} else if (input_->PushKey(DIK_DOWN)) {
		viewprojection_.fovAngleY -= 0.01f;
		viewprojection_.fovAngleY = max(viewprojection_.fovAngleY, 0.01f);
	}

	debugText_->SetPos(50, 110);
	debugText_->Printf("fovAngleY(Drgree):%f", XMConvertToDegrees(viewprojection_.fovAngleY));

	if (input_->PushKey(DIK_W)) {
		viewprojection_.nearZ += 0.1f;
	} else if (input_->PushKey(DIK_S)) {
		viewprojection_.nearZ -= 0.1f;
	}
	viewprojection_.UpdateMatrix();

	debugText_->SetPos(50, 130);
	debugText_->Printf("nearZ:%f", viewprojection_.nearZ);

	
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
	for (size_t i = 0; i < _countof(worldtransform_);i++) {
		model_->Draw(worldtransform_[i], viewprojection_, textureHandle_);	
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
	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

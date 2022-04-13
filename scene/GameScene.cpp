#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

using namespace DirectX;
//コンストラクタ
GameScene::GameScene() {}
//デストラクタ
GameScene::~GameScene() { 
	delete sprite_; 
	delete model_;

}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();
	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("mario.jpg");
	//スプライト作成
	sprite_ = Sprite::Create(textureHandle_, {100, 50});
	//3Dモデルの生成
	model_ = Model::Create();
	//ワールドトランスフォームの初期化
	worldtransform_.Initialize();
	//ビュープロジェクトの初期化
	viewprojection_.Initialize();

	soundDataHandle_ = audio_->LoadWave("se_sad03.wav");
	//再生
	audio_->PlayWave(soundDataHandle_);
	//音量設定
	//audio_->SetVolume(soundDataHandle_, 0.1f);
	voicehandle_ = audio_->PlayWave(soundDataHandle_, true);

}

void GameScene::Update() { 
	//スプライトの今の座標を取得
	XMFLOAT2 position = sprite_->GetPosition();
	//移動
	position.x += 2.0f;
	position.y += 1.0f;
	//座標をスプライトに反映
	sprite_->SetPosition(position);
	//スペースキーで音声停止
	if (input_->TriggerKey(DIK_SPACE)) {
		audio_->StopWave(voicehandle_);
	}
	//でバックテキスト
	//debugText_->Print("kaizokuou ni oreha naru.", 50, 50, 1.0f);
	//書式指定付き表示
	//debugText_->SetPos(50, 70);
	//debugText_->Printf("year:%d", 2001);
	//変数の値をインクリメント
	value_++;
	//値を含んだ文字
	std::string strDebug = std::string("valie:")+
	std::to_string(value_);
	//バックテキストの表示
	debugText_->Print(strDebug, 50, 50, 1.0f);
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
	model_->Draw(worldtransform_, viewprojection_, textureHandle_);
	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	sprite_->Draw();
	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

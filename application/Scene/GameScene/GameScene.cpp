#include "GameScene.h"

GameScene::GameScene() {}

GameScene::~GameScene() {

}

// 初期化
void GameScene::Initialize() {
	camera_.Initialize();
	camera_.rotate.x = 0.28f;
	camera_.translate = { 0,10.0f,-30.0f };

	//乱数生成器
	randomEngine_ = particle1_->random();

	emit.count = 3;
	emit.frequency = 0.5f;
	emit.frequencyTime = 0.0f;

	texHandleCircle_ = TextureManager::Load("resources/circle.png");
	modelBunny_.reset(Model::CreateObj("bunny.obj"));
	texHandleUVChecker_ = TextureManager::Load("resources/uvChecker.png");
	modelGround_.reset(Model::CreateObj("terrain.obj"));
	texHandleGround_ = TextureManager::Load("resources/grass.png");
	modelBunny_->SetTexHandle(texHandleUVChecker_);
	modelGround_->SetTexHandle(texHandleGround_);
	sprite_.reset(Sprite::Create({ 0.0f,0.0f }, texHandleUVChecker_));

	particle2_ = std::make_unique<ParticleSystem>();
	particle2_->Initialize("cube.obj");
	particle2_->SetTexHandle(texHandleCircle_);
	particle1_ = std::make_unique<ParticleSystem>();
	particle1_->Initialize("plane.obj");
	particle1_->SetTexHandle(texHandleUVChecker_);
	accelerationField.acceleration = { 15.0f, 0.0f,0.0f };
	accelerationField.area.min = { -1.0f,-1.0f,-1.0f };
	accelerationField.area.max = { 1.0f,1.0f,1.0f };
	
	isLightingBunny_ = true;
	materialBunny_.color = { 1.0f,1.0f,1.0f,1.0f };
	materialBunny_.shininess = 70.0f;
	lightingPropertyBunny_.color = { 1.0f,1.0f,1.0f,1.0f };
	lightingPropertyBunny_.direction = { 0.0f,-1.0f,0.0f };
	lightingPropertyBunny_.intensity = 1.0f;

	isLightingGround_ = true;
	materialGround_.color = { 1.0f,1.0f,1.0f,1.0f };
	materialGround_.shininess = 70.0f;
	lightingPropertyGround_.color = { 1.0f,1.0f,1.0f,1.0f };
	lightingPropertyGround_.direction = { 0.0f,-1.0f,0.0f };
	lightingPropertyGround_.intensity = 1.0f;

	light_ = None; // ライティングの種類

	pointLight_.color = { 1.0f,1.0f,1.0f,1.0f };
	pointLight_.decay = 0.6f;
	pointLight_.intensity = 1.0f;
	pointLight_.radius = 5.0f;

	spotLight_.color = { 1.0f,1.0f,1.0f,1.0f };
	spotLight_.position = { 2.0f,1.25f,0.0f };
	spotLight_.intensity = 4.0f;
	spotLight_.direction = Normalize({ -1.0f, -1.0f, 0.0f });
	spotLight_.distance = 7.0f;
	spotLight_.cosAngle = std::cos(std::numbers::pi_v<float> / 3.0f);
	spotLight_.cosFallooffStart = 1.0f;
	spotLight_.decay = 2.0f;

	worldTransformBunny_.Initialize();
	worldTransformBunny_.rotate.y = 3.1f;

	worldTransformGround_.Initialize();
}

// 更新
void GameScene::Update() {

	modelBunny_->SetLightingProperty(lightingPropertyBunny_); // ライトの設定
	modelBunny_->SetMaterialProperty(materialBunny_); // マテリアルの設定
	modelBunny_->SetCameraData(camera_.translate);
	modelBunny_->SetPointLightProperty(pointLight_);
	modelBunny_->SetSpotLightProperty(spotLight_);
	materialBunny_.enableLighting = isLightingBunny_;

	modelGround_->SetLightingProperty(lightingPropertyGround_); // ライトの設定
	modelGround_->SetMaterialProperty(materialGround_);
	modelGround_->SetCameraData(camera_.translate);
	modelGround_->SetPointLightProperty(pointLight_);
	modelGround_->SetSpotLightProperty(spotLight_);
	materialGround_.enableLighting = isLightingGround_;

	static int currentItem_ = 0;
	const char* item[3] = { "None", "pointLight", "spotLight" };

#ifdef _DEBUG
	ImGui::Begin("Light Type");

	if (ImGui::Combo("LightType", &currentItem_, item, IM_ARRAYSIZE(item))) {
		if (currentItem_ == 0) {
			light_ = None;
			lightingPropertyBunny_.intensity = 1.0f;
			lightingPropertyGround_.intensity = 1.0f;
		}
		else if (currentItem_ == 1) {
			light_ = Point;
			lightingPropertyBunny_.intensity = 1.0f;
			lightingPropertyGround_.intensity = 1.0f;
		}
		else if (currentItem_ == 2) {
			light_ = Spot;
			lightingPropertyBunny_.intensity = 0.0f;
			lightingPropertyGround_.intensity = 0.0f;
		}
	}

	ImGui::End();

	/*-----------------------------
			bunnyの設定
	-------------------------------*/
	ImGui::Begin("BunnySetting");

	if (ImGui::TreeNode("transform")) {
		ImGui::SliderAngle("rotateX", &worldTransformBunny_.rotate.x);
		ImGui::SliderAngle("rotateY", &worldTransformBunny_.rotate.y);
		ImGui::SliderAngle("rotateZ", &worldTransformBunny_.rotate.z);
		ImGui::DragFloat3("translate", &worldTransformBunny_.translate.x, 0.01f);
		ImGui::DragFloat3("scale", &worldTransformBunny_.scale.x, 0.01f);
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("material")) {
		ImGui::ColorEdit4("color", &materialBunny_.color.x);
		ImGui::DragFloat("shininess", &materialBunny_.shininess, 0.01f);
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Light")) {
		ImGui::Checkbox("isLighting", &isLightingBunny_);
		ImGui::ColorEdit3("color", &lightingPropertyBunny_.color.x);
		ImGui::DragFloat3("direction", &lightingPropertyBunny_.direction.x, 0.01f, -1.0f, 1.0f);
		ImGui::DragFloat("intensity", &lightingPropertyBunny_.intensity, 0.01f);
		ImGui::TreePop();
	}

	ImGui::End();

	/*--------------------------
			地面の設定
	-----------------------------*/
	ImGui::Begin("groundSetting");

	if (ImGui::TreeNode("transform")) {
		ImGui::SliderAngle("rotateX", &worldTransformGround_.rotate.x);
		ImGui::SliderAngle("rotateY", &worldTransformGround_.rotate.y);
		ImGui::SliderAngle("rotateZ", &worldTransformGround_.rotate.z);
		ImGui::DragFloat3("translate", &worldTransformGround_.translate.x, 0.01f);
		ImGui::DragFloat3("scale", &worldTransformGround_.scale.x, 0.01f);
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("material")) {
		ImGui::ColorEdit4("color", &materialGround_.color.x);
		ImGui::DragFloat("shininess", &materialGround_.shininess, 0.01f);
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Light")) {
		ImGui::Checkbox("isLighting", &isLightingGround_);
		ImGui::ColorEdit3("color", &lightingPropertyGround_.color.x);
		ImGui::DragFloat3("direction", &lightingPropertyGround_.direction.x, 0.01f, -1.0f, 1.0f);
		ImGui::DragFloat("intensity", &lightingPropertyGround_.intensity, 0.01f);
		ImGui::TreePop();
	}

	ImGui::End();

	/*---------------------------
			PointLightの設定
	------------------------------*/
	ImGui::Begin("PointLightSetting");
	if (ImGui::TreeNode("LightProperty")) {
		ImGui::SliderFloat4("color", &pointLight_.color.x, 0.0f, 1.0f);
		ImGui::SliderFloat3("position", &pointLight_.position.x, -100.0f, 100.0f);
		ImGui::SliderFloat("intensity", &pointLight_.intensity, 0.0f, 10.0f);
		ImGui::SliderFloat("radius", &pointLight_.radius, 0.0f, 100.0f);
		ImGui::SliderFloat("decay", &pointLight_.decay, 0.0f, 10.0f);
		ImGui::TreePop();
	}

	ImGui::End();


	/*---------------------------
			SpotLightの設定
	------------------------------*/
	ImGui::Begin("SpotLightSetting");
	if (ImGui::TreeNode("LightProperty")) {
		ImGui::SliderFloat4("color", &spotLight_.color.x, 0.0f, 1.0f);
		ImGui::SliderFloat3("position", &spotLight_.position.x, -100.0f, 100.0f);
		ImGui::SliderFloat("intensity", &spotLight_.intensity, 0.0f, 10.0f);
		ImGui::SliderFloat("distance", &spotLight_.distance, 0.0f, 100.0f);
		ImGui::DragFloat3("direction", &spotLight_.direction.x, 0.01f, -1.0f, 1.0f);
		ImGui::SliderAngle("cosAngle", &spotLight_.cosAngle);
		ImGui::SliderFloat("cosFallooffStart", &spotLight_.cosFallooffStart, 0.0f, 1.0f);
		ImGui::SliderFloat("decay", &spotLight_.decay, 0.0f, 10.0f);
		ImGui::TreePop();
	}

	ImGui::End();

	/*----------------------------
			パーティクル
	------------------------------*/
	ImGui::Begin("particle");
	ImGui::Checkbox("DrawParticle1", &isDrawParticle1_);
	ImGui::Checkbox("DrawParticle2", &isDrawParticle2_);
	ImGui::End();

	// スプライト
	ImGui::Begin("sprite");
	ImGui::Checkbox("DrawSprite", &isDrawSprite_);
	ImGui::End();

#endif // _DEBUG

	/*----------------------------
			パーティクル
	------------------------------*/
	emit.frequencyTime += dt;
	if (emit.frequency <= emit.frequencyTime) {
		particles1_.splice(particles1_.end(), particle1_->Emission(emit, randomEngine_));
		particles2_.splice(particles2_.end(), particle2_->Emission(emit, randomEngine_));
		emit.frequencyTime -= emit.frequency;
	}


	for (std::list<Particle>::iterator particleItr = particles1_.begin();
		particleItr != particles1_.end(); ++particleItr) {

		if (particle1_->IsCollision(accelerationField.area, (*particleItr).worldTransform.translate)) {
			(*particleItr).velocity = Add((*particleItr).velocity, Multiply(dt, accelerationField.acceleration));
		}

		(*particleItr).worldTransform.translate = Add((*particleItr).worldTransform.translate, Multiply(dt, (*particleItr).velocity));
		(*particleItr).worldTransform.UpdateMatrix();

	}

	for (std::list<Particle>::iterator particleItr2 = particles2_.begin();
		particleItr2 != particles2_.end(); ++particleItr2) {

		if (particle2_->IsCollision(accelerationField.area, (*particleItr2).worldTransform.translate)) {
			(*particleItr2).velocity = Add((*particleItr2).velocity, Multiply(dt, accelerationField.acceleration));
		}

		(*particleItr2).worldTransform.translate = Add((*particleItr2).worldTransform.translate, Multiply(dt, (*particleItr2).velocity));
		(*particleItr2).worldTransform.UpdateMatrix();

	}
	
	camera_.UpdateMatrix();
	worldTransformBunny_.UpdateMatrix();
	worldTransformGround_.UpdateMatrix();
}

// 描画						  
void GameScene::Draw() {
	modelBunny_->Draw(worldTransformBunny_, camera_, light_);
	modelGround_->Draw(worldTransformGround_, camera_, light_);

	if (isDrawParticle1_) {
		particle1_->Draw(particles1_, camera_);
	}

	if (isDrawParticle2_) {
		particle2_->Draw(particles2_, camera_);
	}
	
	if (isDrawSprite_) {
		sprite_->Draw(camera_);
	}
	
}
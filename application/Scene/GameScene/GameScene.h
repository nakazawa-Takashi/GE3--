#pragma once
#include "application/Scene/IScene/IScene.h"
#include "engine/Sprite/Sprite.h"
#include "engine/Model/Model.h"
#include "engine/ParticleSystem/ParticleSystem.h"

class GameScene : public IScene {
public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize()override;

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update()override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw()override;

private:
	Camera camera_{};
	WorldTransform worldTransformBunny_{};
	WorldTransform worldTransformGround_{};
	std::unique_ptr<Model> modelBunny_;
	std::unique_ptr<Model> modelGround_;
	uint32_t texHandleGround_ = 0;
	uint32_t texHandleUVChecker_ = 0;
	bool isLightingBunny_;
	bool isLightingGround_;
	Light light_{};
	PointLight pointLight_{};
	SpotLight spotLight_{};
	Material materialBunny_{};
	Material materialGround_{};
	DirectionalLight lightingPropertyBunny_{};
	DirectionalLight lightingPropertyGround_{};
	std::list<Particle> particles1_ = {};
	std::list<Particle> particles2_ = {};
	std::unique_ptr<ParticleSystem> particle1_ = {};
	std::unique_ptr<ParticleSystem> particle2_ = {};
	uint32_t texHandleCircle_ = 0;
	const float dt = 1.0f / 60.0f;
	Emitter emit{};
	AccelerationField accelerationField{};
	std::mt19937 randomEngine_;
	bool isDrawParticle1_ = false;
	bool isDrawParticle2_ = false;
	bool isDrawSprite_ = false;
	std::unique_ptr<Sprite> sprite_;
};

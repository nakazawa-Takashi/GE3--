#pragma once
#include "application/Scene/IScene/IScene.h"
#include "engine/Camera/Camera.h"
#include  "engine/Sprite/Sprite.h"
#include "engine/Input/Input.h"
#include "application/GameManager/GameManager.h"
#include "engine/Utility/ImGuiManager/ImGuiManager.h"

class TitleScene : public IScene {
public:

	TitleScene();
	~TitleScene();

	void Initialize()override;

	void Update()override;

	void Draw()override;

private:
	Camera camera_;
	std::unique_ptr<Sprite> sprite_;
	std::unique_ptr<Sprite> spritePushA_;
	uint32_t texHandle_ = 0;
	uint32_t texHandlePushA_ = 0;
	GameManager* gameManager_ = nullptr;
};
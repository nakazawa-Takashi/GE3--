#pragma once
#include <memory>
#include "engine/SceneFactory/AbstractSceneFactory.h"

class GameManager {
public:

	static GameManager* GetInstance();

	void Update(); // ゲームループを呼び出す

	void Initialize(); // 初期化

	void Draw();

	void Finalize();

	void SetSceneFactory(std::unique_ptr<AbstractSceneFactory> sceneFactory) { sceneFactory_ = std::move(sceneFactory); }

	void ChangeScene(const std::string& sceneName);

private:
	GameManager() = default;
	~GameManager() = default;
	GameManager(const GameManager&) = delete;
	GameManager& operator=(const GameManager&) = delete;

	std::unique_ptr<IScene> scene_ = nullptr; // 今のシーン
	std::unique_ptr<IScene> nextScene_ = nullptr; // 次のシーン
	std::unique_ptr<AbstractSceneFactory> sceneFactory_ = nullptr;

};

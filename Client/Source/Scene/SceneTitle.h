#pragma once
#include "SceneManager.h"
#include "Graphics\Graphics.h"
#include "Graphics\Sprite.h"
#include "Input\Input.h"
#include "System\Logger.h"

// コンフィグシーン
class SceneTitle : public Scene
{
public:
	SceneTitle() {}
	~SceneTitle() override {}

	// 初期化
	void Initialize() override;

	void Update(float elapsedTime) {};

	// 終了化
	void Finalize() override;

	void Render()override;
private:
	std::unique_ptr<Sprite> logo = nullptr;
	std::unique_ptr<Sprite> background = nullptr;
};

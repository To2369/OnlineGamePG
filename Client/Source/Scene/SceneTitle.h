#pragma once
#include "SceneManager.h"
#include "Graphics\Graphics.h"
#include "Graphics\Sprite.h"
#include "Input\Input.h"
#include "System\Logger.h"

// �R���t�B�O�V�[��
class SceneTitle : public Scene
{
public:
	SceneTitle() {}
	~SceneTitle() override {}

	// ������
	void Initialize() override;

	void Update(float elapsedTime) {};

	// �I����
	void Finalize() override;

	void Render()override;
private:
	std::unique_ptr<Sprite> logo = nullptr;
	std::unique_ptr<Sprite> background = nullptr;
};

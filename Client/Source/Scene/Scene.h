#pragma once
#include <iostream>
#include "Camera/CameraController.h"
#include "Graphics/Sprite.h"
#include "Graphics/Font.h"
#include "Stage/Stage.h"
#include "Character\JobClass.h"

// �V�[��
class Scene
{
public:
	Scene() {};
	virtual ~Scene() {};

	// ������
	virtual void Initialize() = 0;

	// �I����
	virtual void Finalize() = 0;

	// �X�V����
	virtual void Update(float elapsedTime) = 0;

	// �`�揈��
	virtual void Render() = 0;

	// �����������Ă��邩
	bool IsReady() const { return ready; }

	// ���������ݒ�
	void SetReady() { ready = true; }

private:
	bool	ready = false;
};

class SceneBase : public Scene
{
public:
	SceneBase() {};
	~SceneBase() { 
		Finalize(); 
	}
	// ������
	virtual void Initialize();

	// �I����
	virtual void Finalize();

	// �X�V����
	virtual void Update(float elapsedTime);

	virtual void NetrowkUpdate(float elapsedTime) {};

	// �`�揈��
	void Render();

	// �N���b�N���W�擾
	bool GetClickPosition(DirectX::XMFLOAT3 screenPosition, HitResult& hit);

private:
	bool	ready = false;
protected:
	PlayerManager* playerManager = nullptr;
	Stage* stage = nullptr;
	Font* font = nullptr;
	CameraController* cameraController = nullptr;
};
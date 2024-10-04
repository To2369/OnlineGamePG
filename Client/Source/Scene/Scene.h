#pragma once
#include <iostream>
#include "Camera/CameraController.h"
#include "Graphics/Sprite.h"
#include "Graphics/Font.h"
#include "Stage/Stage.h"
#include "Character\JobClass.h"

// シーン
class Scene
{
public:
	Scene() {};
	virtual ~Scene() {};

	// 初期化
	virtual void Initialize() = 0;

	// 終了化
	virtual void Finalize() = 0;

	// 更新処理
	virtual void Update(float elapsedTime) = 0;

	// 描画処理
	virtual void Render() = 0;

	// 準備完了しているか
	bool IsReady() const { return ready; }

	// 準備完了設定
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
	// 初期化
	virtual void Initialize();

	// 終了化
	virtual void Finalize();

	// 更新処理
	virtual void Update(float elapsedTime);

	virtual void NetrowkUpdate(float elapsedTime) {};

	// 描画処理
	void Render();

	// クリック座標取得
	bool GetClickPosition(DirectX::XMFLOAT3 screenPosition, HitResult& hit);

private:
	bool	ready = false;
protected:
	PlayerManager* playerManager = nullptr;
	Stage* stage = nullptr;
	Font* font = nullptr;
	CameraController* cameraController = nullptr;
};
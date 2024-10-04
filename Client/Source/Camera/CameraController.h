#pragma once

#include <DirectXMath.h>

class CameraController
{
public:
	CameraController(){}
	~CameraController(){}

	// 更新処理
	void Update(float elapsedTime);

	// ターゲット位置設定
	void SetTarget(const DirectX::XMFLOAT3& target) { this->target = target; }
	void SetRange(float range) { this->range = range; }
	void SetAngle(const DirectX::XMFLOAT3& angle) { this->angle = angle; }
private:
	DirectX::XMFLOAT3	target = { 0,0,0 };
	DirectX::XMFLOAT3	angle = { 0,0,0 };
	float				rollSpeed = DirectX::XMConvertToRadians(40);
	bool				drag = false;
	float				range = 50.0f;
	float				maxAngleX = DirectX::XMConvertToRadians(45);
	float				minAngleX = DirectX::XMConvertToRadians(-45);
};
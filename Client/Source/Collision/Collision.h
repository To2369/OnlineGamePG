#pragma once

#include <DirectXMath.h>
#include "Graphics/Model.h"

// �q�b�g����
struct HitResult
{
	DirectX::XMFLOAT3	position = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3	normal = { 0.0f,0.0f,0.0f };
	float				distance = 0.0f;
	int					materialIndex = -1;
};

// �R���W����
class Collision
{
public:
	// ���Ƌ��̌�������
	static bool IntersectSphereVsSphere(
		const DirectX::XMFLOAT3& positionA,
		float radiusA,
		const DirectX::XMFLOAT3& positionB,
		float radiusB,
		DirectX::XMFLOAT3& outPositionB
	);
	// ���C�ƃ��f���̌�������
	static bool IntersectRayVsModel(
		const DirectX::XMFLOAT3& start,
		const DirectX::XMFLOAT3& end,
		const Model* model,
		HitResult& result
	);
};


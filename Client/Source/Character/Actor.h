#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "Graphics/Shader.h"
#include "Graphics/Model.h"
#include "Collision/Collision.h"
#include "Stage/Stage.h"

class Actor
{
public:
	// �A�j���[�V����
	enum class Animation
	{
		Idle,
		Walk,
		Run,
		Pickup,
		Revive,
		ImpDefeat,
		Fall,
		EmoteCheer,
		EmoteDance,
		EmoteWave,
		EmoteSitStart,
		EmoteSitLoop,
		EmoteSitEnd,
		BossChargeAttackStart,
		BossChargeAttackLoop,
		BossChargeAttackEnd,
		Stunned,
		BossDefeatIdle,
		FallPose,
		RunStart,
		CarryItemPickup,
		CarryItemPlace,
		CarryItemIdle,
		CarryItemWalk,
		Attack1,
		Attack2,
		SkillHeal,
		SkillShieldBufLoop,
		SkillShieldBufStart,
		SkillPowershotEnd,
		SkillPowershotLoop,
		SkillPowershotStart,
		SkillSneakPoof,
		SkillDashAttackStart,
		SkillDashAttackLoop,
		SkillDashAttackEnd,
		HitreactCritical,
		End
	};

	Actor();
	~Actor();

	virtual void Update(float elapsedTime);

	// �`��
	virtual void Render(ID3D11DeviceContext* context, Shader* shader);

	// �f�o�b�O�`��
	virtual void DrawDebugPrimitive();


	// model�擾
	Model* GetModel() { return model; }

	// ���W�擾
	DirectX::XMFLOAT3 GetPosition() { return position; }

	// ���W�ݒ�
	void SetPosition(DirectX::XMFLOAT3 setPosition) { position = setPosition; }

	// �p�x�擾
	DirectX::XMFLOAT3 GetAngle() { return angle; }

	// �p�x�ݒ�
	void SetAngle(DirectX::XMFLOAT3 setAngle) { angle = setAngle; }

	// �{���擾
	DirectX::XMFLOAT3 GetScale() const { return scale; }

	// �{���ݒ�
	void SetScale(DirectX::XMFLOAT3 setScale) { scale = setScale; }

	// �ړ���擾
	DirectX::XMFLOAT3 GetTargetPosition() const { return targetPosition; }

	// �ړ���ݒ�
	void SetTargetPosition(DirectX::XMFLOAT3 setTargetPosition) { targetPosition = setTargetPosition; }

	// �������f
	void SetAlived(bool alived) { this->alived = alived; }
	bool GetAlived() const { return alived; }
	// �̗͊֘A

	void SetHealth(int health) { this->health = health; }
	int GetHealth() const { return health; }
	float GetTurnSpeed() const { return turnSpeed; }
	// �ړ���擾
	// �^�[�Q�b�g�܂ňړ�
	void MoveToTarget(float elapsedTime, float vx, float vz, float speedRate);

	// �ړ�����
	void Move(float vx, float vz, float speed);

	// ��]
	void Turn(float elapsedTime, float vx, float vz, float speed);
	void LookAt(float vx ,float vz);
	// Transform�X�V
	void UpdateTransform();
	

protected:
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 angle;
	DirectX::XMFLOAT3 scale;
	bool alived = true;
	int health = 10;
	DirectX::XMFLOAT4X4 transform = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
	DirectX::XMFLOAT3 targetPosition = { 0,0,0 };
	Model* model = nullptr;

	float attackRange = 2.5;
	float range = 10.0f;
	float radius = 2.5f;
	float height = 4.0f;
	float maxMoveSpeed = 30.0f;
	float moveSpeed = 16.0f;
	float turnSpeed = DirectX::XMConvertToRadians(360);

	DirectX::XMFLOAT3 velocity = { 0,0,0 };
	float	stepOffset = 1.0f;
};
#include "Character/Actor.h"
#include "Graphics/Graphics.h"


Actor::Actor()
{
	model = nullptr;
	targetPosition = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	scale = DirectX::XMFLOAT3(0.2f, 0.2f, 0.2f);
}

Actor::~Actor()
{
	if (model != nullptr)
	{
		delete model;
		model = nullptr;
	}
}

void Actor::Update(float elapsedTime)
{
	// �I�u�W�F�N�g�s����X�V
	UpdateTransform();

	// �A�j���[�V�����X�V
	model->UpdateAnimation(elapsedTime);

	// ���f���s��X�V
	model->UpdateTransform(transform);
}

void Actor::Render(ID3D11DeviceContext* context, Shader* shader)
{
	shader->Draw(context, model);
}


// �ړI�n�_�ֈړ�
void Actor::MoveToTarget(float elapsedTime, float vx , float vz ,float speedRate)
{
	// �ړ�����
	Move(vx, vz, moveSpeed * speedRate);
	Turn(elapsedTime, vx, vz, turnSpeed * speedRate);
}
void Actor::Move(float vx, float vz, float speed)
{
	// �ړ������x�N�g����ݒ�
	float mx = vx * speed;
	float mz = vz * speed;
	position.x += mx;
	position.z += mz;
}
// ���񏈗�
void Actor::Turn(float elapsedTime, float vx, float vz, float speed)
{
	speed *= elapsedTime;

	// �i�s�x�N�g�����[���x�N�g���̏ꍇ�͏�������K�v�Ȃ�
	float length = sqrtf(vx * vx + vz * vz);
	if (length <= 0.001f)
	{
		return;
	}
	// �i�s�x�N�g����P�ʃx�N�g����
	vx /= length;
	vz /= length;

	// ���g�̉�]�l����O���������߂�
	float frontX = sinf(angle.y);
	float frontZ = cosf(angle.y);

	// ��]�p�����߂邽�߁A2�̒P�ʃx�N�g���̓��ς��v�Z����
	float dot = frontX * vx + frontZ * vz;

	// ���ϒl��-1.0�`1.0�ŕ\������Ă���A2�̒P�ʃx�N�g���̊p�x��
	// �������ق�1.0�ɋ߂Â��Ƃ��������𗘗p���ĉ�]���x�𒲐�����
	float rot = 1.0f - dot;

	// ���E������s�����߂ɂQ�̒P�ʃx�N�g���̊O�ς��v�Z����
	float cross = (frontX * vz) - (frontZ * vx);

	// 2D�̊O�ϒl�����̏ꍇ�����̏ꍇ���ɂ���č��E���肪�s����
	// ���E������s�����Ƃɂ���č��E��]��I������
	if (cross < 0.0f)
	{
		angle.y += rot;
	}
	else
	{
		angle.y -= rot;
	}
}
void Actor::LookAt(float vx, float vz)
{
	vx = vx - position.x;
	vz = vz - position.z;
	// ���������x�N�g���̊p�x���Z�o
	float radian = atan2f(vx, vz);
	angle.y = radian;
}

void Actor::UpdateTransform()
{
	// �X�P�[���s����쐬
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	// ��]�s����쐬
	DirectX::XMMATRIX X = DirectX::XMMatrixRotationX(angle.x);
	DirectX::XMMATRIX Y = DirectX::XMMatrixRotationY(angle.y);
	DirectX::XMMATRIX Z = DirectX::XMMatrixRotationZ(angle.z);

	DirectX::XMMATRIX R = Y * X * Z;

	// �ʒu�s����쐬
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	// �R�̍s���g�ݍ��킹�A���[���h�s����쐬
	DirectX::XMMATRIX W = S * R * T;
	// �v�Z�������[���h�s������o��
	DirectX::XMStoreFloat4x4(&transform, W);
}

void Actor::DrawDebugPrimitive()
{
	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

	// �Փ˔���p�̃f�o�b�O�~����`��
	debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));

}


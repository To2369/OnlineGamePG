#include "Stage.h"


static Stage* instance = nullptr;

// �C���X�^���X�擾
Stage& Stage::Instance()
{
	return *instance;
}
Stage::Stage()
{
	instance = this;
	model = new Model("Data/Model/Environment/BossRoom.mdl");
}

Stage::~Stage()
{
	// �X�e�[�W���f����j��
	delete model;
}

// �X�V����
void Stage::Update(float elapsedTime)
{
	// ���͓��ɂ�邱�Ƃ͂Ȃ�
}

void Stage::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	shader->Draw(dc,model);
}

// ���C�L���X�g
bool Stage::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
	return Collision::IntersectRayVsModel(start, end, model,hit);
}
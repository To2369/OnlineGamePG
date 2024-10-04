#include "Scene.h"
#include "Graphics/Graphics.h"
#include "Input/Input.h"
#include "Camera/Camera.h"
#include "Character/JobClass.h"


// ���������[�N�����p
#define debug_new new(_NORMAL_BLOCK,__FILE__,__LINE__)

void SceneBase::Initialize()
{
	font = new Font("Data/Font/MS Gothic.fnt", 1024 * 16);

	stage = new Stage();

	// �J���������ݒ�
	Graphics& graphics = Graphics::Instance();
	Camera& camera = Camera::Instance();
	camera.SetLookAt(
		DirectX::XMFLOAT3(-20, 50, 0),
		DirectX::XMFLOAT3(DirectX::XMFLOAT3(-70.0f, 0.5f, -45.0f)),
		DirectX::XMFLOAT3(0, 1, 0)
	);

	camera.SetPerspecticeFov(
		DirectX::XMConvertToRadians(45),
		graphics.GetScreenWidth() / graphics.GetScreenHeight(),
		0.1f,
		1000.0f
	);

	// �J�����R���g���[���[������
	cameraController = new CameraController();
	DirectX::XMFLOAT3 target = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	cameraController->SetTarget(target);
	cameraController->SetAngle(DirectX::XMFLOAT3(DirectX::XMConvertToRadians(70.0f), DirectX::XMConvertToRadians(180.0f), 0.0f));
	cameraController->SetRange(50.0f);
	cameraController->Update(1.0f);

}

void SceneBase::Finalize()
{
	if (stage != nullptr)
	{
		delete stage;
		stage = nullptr;
	}
	// �J�����R���g���[���[�I��
	if (cameraController != nullptr)
	{
		delete cameraController;
		cameraController = nullptr;
	}
	if (font != nullptr)
	{
		delete font;
		font = nullptr;
	}
	if (playerManager != nullptr)
	{
		for (Player* player : playerManager->GetPlayers())
		{
			if (player != nullptr)
			{
				delete player;
				player = nullptr;
			}
		}
		delete playerManager;
		playerManager = nullptr;
	}

}

void SceneBase::Update(float elapsedTime)
{

	// �v���C���[�X�V
	if (playerManager->GetPlayers().size() > 0)
	{
		for (Player* player : playerManager->GetPlayers())
		{
			if (player->GetReady())
			{
				player->Update(elapsedTime);
			}
		}
	}
	stage->Update(elapsedTime);
	if (playerManager->GetPlayers().size() >0)
	{
		// �J�����R���g���[���[�X�V����
		Player* player = playerManager->GetMyPlayer();
		if (player != nullptr)
		{
			DirectX::XMFLOAT3 target = playerManager->GetMyPlayer()->GetPosition();
			target.y += 0.5f;
			cameraController->SetTarget(target);

			HWND hWnd;
			hWnd = GetActiveWindow();
			if (hWnd != NULL)
			{
				// �J�����X�V
				cameraController->Update(elapsedTime);
			}
		}

	}
	// �폜�p�z��ɓ����Ă���v���C���[���폜
	playerManager->DeletePlayer();
	NetrowkUpdate(elapsedTime);

}

void SceneBase::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

	// ��ʃN���A�������_�[�^�[�Q�b�g�ݒ�
	FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };	// RGBA(0.0�`1.0)
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);

	// �`�揈��
	RenderContext rc;
	rc.lightDirection = { 0.0f, -1.0f, 0.0f, 0.0f };	// ���C�g�����i�������j

	// �J�����p�����[�^�ݒ�
	Camera& camera = Camera::Instance();
	rc.view = camera.GetView();
	rc.projection = camera.GetProjection();

	Shader* shader = graphics.GetShader();
	shader->Begin(dc, rc);

	// �X�e�[�W�`��
	stage->Render(dc, shader);

	// �v���C���[�`��
	for (Player* player : playerManager->GetPlayers())
	{
		if (player->GetReady())
		{
			player->Render(dc, shader);
		}
	}
	shader->End(dc);

	// ���C�������_���`����s
	graphics.GetLineRenderer()->Render(dc, rc.view, rc.projection);

	// �f�o�b�O�����_���`����s
	graphics.GetDebugRenderer()->Render(dc, rc.view, rc.projection);
}

// �N���b�N���W�擾
bool SceneBase::GetClickPosition(DirectX::XMFLOAT3 screenPosition, HitResult& hit)
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	// �J�����p�����[�^�ݒ�
	Camera& camera = Camera::Instance();

	// �r���[�|�[�g
	D3D11_VIEWPORT viewport;
	UINT numViewports = 1;
	dc->RSGetViewports(&numViewports, &viewport);

	// �ϊ��s��
	DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&camera.GetView());
	DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&camera.GetProjection());
	DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();

	DirectX::XMVECTOR ScreenPosition, WorldPosition;

	// ���C�̎��_���Z�o
	ScreenPosition = DirectX::XMLoadFloat3(&screenPosition);
	WorldPosition = DirectX::XMVector3Unproject(
		ScreenPosition,
		viewport.TopLeftX,
		viewport.TopLeftY,
		viewport.Width,
		viewport.Height,
		viewport.MinDepth,
		viewport.MaxDepth,
		Projection,
		View,
		World
	);
	DirectX::XMFLOAT3 rayStart;
	DirectX::XMStoreFloat3(&rayStart, WorldPosition);

	// ���C�̏I�_���Z�o
	screenPosition.z = 1.0f;
	ScreenPosition = DirectX::XMLoadFloat3(&screenPosition);
	WorldPosition = DirectX::XMVector3Unproject(
		ScreenPosition,
		viewport.TopLeftX,
		viewport.TopLeftY,
		viewport.Width,
		viewport.Height,
		viewport.MinDepth,
		viewport.MaxDepth,
		Projection,
		View,
		World
	);
	DirectX::XMFLOAT3 rayEnd;
	DirectX::XMStoreFloat3(&rayEnd, WorldPosition);

	// ���C�L���X�g
	return Stage::Instance().RayCast(rayStart, rayEnd, hit);
}
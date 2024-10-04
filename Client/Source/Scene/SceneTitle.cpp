#include "Assignment\01ClientAssignment.h"
#include "Assignment\02ClientAssignment.h"
#include "Assignment\03ClientAssignment.h"
#include "Assignment\04ClientAssignment.h"
#include "Assignment\05ClientAssignment.h"
#include "Assignment\06ClientAssignment.h"
#include "Assignment\07ClientAssignment.h"
#include "Assignment\08ClientAssignment.h"
#include "Assignment\09ClientAssignment.h"
#include "Assignment\10ClientAssignment.h"
#include "SceneTitle.h"

// ������
void SceneTitle::Initialize()
{
	logo = std::make_unique<Sprite>("Data/Sprite/UI/ui_title_logo.png");
	background = std::make_unique<Sprite>("Data/Sprite/background.png");
}

// �I����
void SceneTitle::Finalize()
{

}

// �`�揈��
void SceneTitle::Render()
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

	float screenWidth = static_cast<float>(graphics.GetScreenWidth());
	float screenHeight = static_cast<float>(graphics.GetScreenHeight());

	// �w�i�\��
	background;
	float textureWidth = static_cast<float>(background->GetTextureWidth());
	float textureHeight = static_cast<float>(background->GetTextureHeight());
	// �^�C�g���X�v���C�g�`��
	background->Render(dc,
		0, 0, screenWidth, screenHeight,
		0, 0, textureWidth, textureHeight,
		0,
		1, 1, 1, 1);

	// ���S�\��
	textureWidth = static_cast<float>(logo->GetTextureWidth());
	textureHeight = static_cast<float>(logo->GetTextureHeight());
	logo->Render(dc,
		(screenWidth / 2) - (screenWidth / 4), 0, screenWidth / 2, screenHeight / 2,
		0, 0, textureWidth, textureHeight,
		0,
		1, 1, 1, 1);

	// imgui
	ImGui::SetNextWindowPos(ImVec2(screenWidth / 2 - 150, 200), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(300, 370), ImGuiCond_Once);

	// �T�[�o�[�Ɛڑ�
	if (ImGui::Begin("Scene", nullptr, ImGuiWindowFlags_None))
	{
		if (ImGui::Button(u8"Assignment01:�T�[�o�ƒʐM"))
		{
			SceneManager::Instance().ChangeScene(new ClientAssignment01);
		}
		if (ImGui::Button(u8"Assignment02:�L�����N�^�[�ړ��E�U��"))
		{
			SceneManager::Instance().ChangeScene(new ClientAssignment02);
		}
		if (ImGui::Button(u8"Assignment03:�������[�U�Ƃ̒ʐM"))
		{
			SceneManager::Instance().ChangeScene(new ClientAssignment03);
		}
		if (ImGui::Button(u8"Assignment04:RingBuffer"))
		{
			SceneManager::Instance().ChangeScene(new ClientAssignment04);
		}
		if (ImGui::Button(u8"Assignment05:�u���[�h�L���X�g�ʐM"))
		{
			SceneManager::Instance().ChangeScene(new ClientAssignment05);
		}
		if (ImGui::Button(u8"Assignment06:�}���`�L���X�g�ʐM"))
		{
			SceneManager::Instance().ChangeScene(new ClientAssignment06);
		}
		if (ImGui::Button(u8"Assignment07:�Í����ʐMSSL/TLS"))
		{
			SceneManager::Instance().ChangeScene(new ClientAssignment07);
		}
		if (ImGui::Button(u8"Assignment08:�n�b�V����"))
		{
			SceneManager::Instance().ChangeScene(new ClientAssignment08);
		}
		if (ImGui::Button(u8"Assignment09:�閧���Í���"))
		{
			SceneManager::Instance().ChangeScene(new ClientAssignment09);
		}
		if (ImGui::Button(u8"Assignment10:���J���Í���"))
		{
			SceneManager::Instance().ChangeScene(new ClientAssignment10);

		}
	}
	ImGui::End();
}

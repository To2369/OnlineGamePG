#include <string>
#include <fstream>
#include <openssl/evp.h>
#include "09ClientAssignment.h"
#include <algorithm>
#include <windows.h>
#include <commdlg.h>
#include <iomanip>
#include <sstream>
#include <string>

ClientAssignment09::ClientAssignment09()
{

}

ClientAssignment09::~ClientAssignment09()
{

}

void ClientAssignment09::Initialize()
{
	SceneBase::Initialize();

	// Player�ݒ�
	playerManager = new PlayerManager();
	playerManager->SetMyPlayerID(0);

	Player* player = new Player();


	// �e�v���C���[�̃L�����N�^�[�ɍ��킹���ݒ���s��
	// JobClass�̓X�e�[�g�}�V��
	player->SetPlayerID(0);
	player->SetPosition(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	player->SetScale(DirectX::XMFLOAT3(0.02f, 0.02f, 0.02f));
	player->SetModel();

	playerManager->AddPlayer(player);
	playerManager->GetMyPlayer()->SetReady(true);
}

void ClientAssignment09::Finalize()
{

}

void ClientAssignment09::Render()
{
	// ���N���X�̃����_�[�Ăяo��
	SceneBase::Render();
	// imgui
	Graphics& graphics = Graphics::Instance();
	float screenWidth = static_cast<float>(graphics.GetScreenWidth());
	ImGui::SetNextWindowPos(ImVec2(screenWidth / 2 - 250, 100), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(500, 500), ImGuiCond_Once);

	if (ImGui::Begin("AES", nullptr, ImGuiWindowFlags_None))
	{
		ImGui::TextWrapped(u8"�ۑ�_���͂����f�[�^���Í������ĕ���������");
		ImGui::InputText(u8"��������", planeText, sizeof(planeText));
		planeHexText = ConvertingToHexNumber(reinterpret_cast<unsigned char*>(planeText), strlen(planeText));
		ImGui::TextWrapped("%s", planeHexText.c_str(),planeHexText.size());
		if (ImGui::Button(u8"�Í���/������"))
		{
			memset(encode, 0, sizeof(encode));
			memset(decode, 0, sizeof(decode));
			encodeSize = 0;
			decodeSize = 0;

			// �Í�������
			Encrypt(reinterpret_cast<unsigned char*>(planeText), strlen(planeText), encode, &encodeSize);

			// ����������
			Decrypt(encode, encodeSize, decode, &decodeSize);
			Logger::Print("encode:%s\n", encode);
			Logger::Print("decode:%s\n", decode);

		}

		ImGui::Text(u8"�Í���");
		ImGui::TextWrapped("%s", ConvertingToHexNumber(encode,encodeSize).c_str());

		ImGui::Text(u8"������");
		ImGui::TextWrapped("%s", ConvertingToHexNumber(decode, decodeSize).c_str());

		//ImGui::EndChild();
		if (ImGui::Button(u8"�^�C�g����"))
		{
			SceneManager::Instance().ChangeScene(new SceneTitle());
		}

	}
	ImGui::End();
}

void ClientAssignment09::Encrypt(const unsigned char* in, const size_t in_len, unsigned char* out, int* out_len)
{
	// �Í�������������

	// TODO 09_01
	// �Í����R���e�L�X�g����
	EVP_CIPHER_CTX* ctx;

	int f_len = 0;

	// TODO 09_02
	// �Í����R���e�L�X�g������


	// TODO 09_03
	// �Í�������������(�A���S���Y���A���AIV�ݒ�)
	

	// TODO 09_04
	// �Í�������
	

	// TODO 09_05
	// �ŏI�u���b�N�̈Í���
	

	// TODO 09_06
	// �Í����R���e�L�X�g�J��
	

}

void ClientAssignment09::Decrypt(const unsigned char* in, const size_t in_len, unsigned char* out, int* out_len)
{
	// ����������������
	// TODO 09_07
	// �������R���e�L�X�g�쐬
	EVP_CIPHER_CTX* ctx;
	
	int f_len = 0;

	// TODO 09_08
	// �������R���e�L�X�g������
	

	// TODO 09_09
	// ����������������(�A���S���Y���A���AIV��ݒ�)
	

	// TODO 09_10
	// ����������
	

	// TODO 09_11
	// �ŏI�u���b�N�̕�����


	// TODO 09_12
	// �Í����R���e�L�X�g�J��


}

std::string ClientAssignment09::ConvertingToHexNumber(const unsigned char* bytes, const size_t length)
{
	std::ostringstream oss;

	// �e��16�i���ɕϊ����ĕ\������
	for (size_t i = 0; i < length; i++) {
		oss << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(bytes[i]);
	}
	return oss.str();
}
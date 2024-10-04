#include <string>
#include <fstream>
#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/engine.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/types.h>
#include "10ClientAssignment.h"
#include <algorithm>
#include <windows.h>
#include <commdlg.h>
#include <iomanip>
#include <sstream>
#include <string>

ClientAssignment10::ClientAssignment10()
{

}

ClientAssignment10::~ClientAssignment10()
{

}

void ClientAssignment10::Initialize()
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

void ClientAssignment10::Finalize()
{

}

void ClientAssignment10::Render()
{
	// ���N���X�̃����_�[�Ăяo��
	SceneBase::Render();
	// imgui
	Graphics& graphics = Graphics::Instance();
	float screenWidth = static_cast<float>(graphics.GetScreenWidth());
	ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiCond_Once);

	if (ImGui::Begin("Assignment10", nullptr, ImGuiWindowFlags_None))
	{
		if (ImGui::Button(u8"�Í������쐬"))
		{
	// �Í��������쐬
	CreateKey(publicKeyName, privateKeyName);

		}
		if (ImGui::Button(u8"�^�C�g����"))
		{
			SceneManager::Instance().ChangeScene(new SceneTitle());
		}

	}
	ImGui::End();
	ImGui::SetNextWindowPos(ImVec2(screenWidth /2 - 400, 200), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(400, 500), ImGuiCond_Once);
	if (ImGui::Begin(u8"�Í���/������", nullptr, ImGuiWindowFlags_None))
	{
		ImGui::TextWrapped(u8"�ۑ�_���͂����f�[�^���Í������ĕ���������");
		ImGui::InputText(u8"��������", toEncode, sizeof(toEncode));
		planeHexText = ConvertingToHexNumber(reinterpret_cast<unsigned char*>(toEncode), strlen(toEncode));
		ImGui::TextWrapped("%s", planeHexText.c_str(), planeHexText.size());
		if (ImGui::Button(u8"���s"))
		{
			// �Í�������
			Encrypt(publicKeyName, reinterpret_cast<unsigned char*>(toEncode), strlen(toEncode), &publicEncode, &publicEncodeSize);

			// ����������
			Decrypt(privateKeyName, publicEncode, publicEncodeSize, &privateDecode, &privateDecodeSize);
		}

		ImGui::Text(u8"�Í���");
		if (publicEncode != nullptr)
		{
			ImGui::TextWrapped("%s", ConvertingToHexNumber(publicEncode, publicEncodeSize).c_str());
		}
		else
		{
			ImGui::TextWrapped(" ");
		}

		ImGui::Text(u8"������");
		if (privateDecode != nullptr)
		{
			ImGui::TextWrapped("%s", ConvertingToHexNumber(privateDecode, privateDecodeSize).c_str());
		}
		else
		{
			ImGui::TextWrapped(" ");
		}
	}
	ImGui::End();
	ImGui::SetNextWindowPos(ImVec2(screenWidth / 2 , 200), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(400, 500), ImGuiCond_Once);
	if (ImGui::Begin(u8"����/�����m�F", nullptr, ImGuiWindowFlags_None))
	{
		ImGui::TextWrapped(u8"�ۑ�_���͂����f�[�^���������ď����m�F����");
		ImGui::InputText(u8"��������", toSign, sizeof(toSign));
		planeHexText = ConvertingToHexNumber(reinterpret_cast<unsigned char*>(toSign), strlen(toSign));
		ImGui::TextWrapped("%s", planeHexText.c_str(), planeHexText.size());
		if (ImGui::Button(u8"���s"))
		{
			// ��������
			Sign(privateKeyName, reinterpret_cast<unsigned char*>(toSign), strlen(toSign), &privateEncode, &privateEncodeSize);

			// �����m�F����
			Verify(publicKeyName, privateEncode, privateEncodeSize, &publicDecode, &publicDecodeSize);
		}

		ImGui::Text(u8"����");
		if (privateEncode != nullptr)
		{
			ImGui::TextWrapped("%s", ConvertingToHexNumber(privateEncode, privateEncodeSize).c_str());
		}
		else
		{
			ImGui::TextWrapped(" ");
		}

		ImGui::Text(u8"�����m�F");
		if (publicDecode != nullptr)
		{
			ImGui::TextWrapped("%s", ConvertingToHexNumber(publicDecode, publicDecodeSize).c_str());
		}
		else
		{
			ImGui::TextWrapped(" ");
		}
	}
	ImGui::End();
}

int ClientAssignment10::CreateKey(char* publicKeyName, char* privateKeyName)
{
	// ���J���t�@�C���̃I�[�v��
	FILE* fp;
	if (fopen_s(&fp, publicKeyName, "w") != 0) {
		Logger::Print("���J���t�@�C���̃I�[�v���G���[: %s\n", publicKeyName);
		return -1;
	}

	// TODO 10_01
	// 2048�r�b�g��RSA���y�A�𐶐�
	EVP_PKEY* pKey;

	// TODO 10_02
	// ���J�����t�@�C���ɏ����o��



	fclose(fp);

	// �閧���t�@�C���̃I�[�v��
	if (fopen_s(&fp, privateKeyName, "w") != 0) {
		Logger::Print("�閧���t�@�C���̃I�[�v���G���[: %s\n", privateKeyName);
		return -1;
	}

	// TODO 10_03
	// �閧�����t�@�C���ɏ����o��


	fclose(fp);

	// TODO 10_04
	// �����������

	return 0;
}

void ClientAssignment10::Encrypt(const char* filename, const unsigned char* planeText, size_t planeSize, unsigned char** encode, size_t* encodeSize)
{
	// ���J���t�@�C�����J��
	FILE* fp;	
	if (fopen_s(&fp, filename, "r") != 0) {
		Logger::Print("�t�@�C���̃I�[�v���G���[:%s\n",filename);
		return;
	}

	// TODO 10_05
	// �t�@�C���|�C���^������J����ǂݍ���
	EVP_PKEY* pKey;


	// TODO 10_06
	// �Í����p�̃R���e�L�X�g�𐶐�
	EVP_PKEY_CTX* ctx;

	
	// TODO 10_07
	// �Í����̏�����
	


	// TODO 10_08
	// �p�f�B���O�ݒ�



	// TODO 10_09
	// �Í�����̃o�b�t�@�T�C�Y�𐄑�
	


	// TODO 10_10
	// �Í����f�[�^�p�̃��������m��
	


	// TODO 10_11
	// ���ۂɈÍ��������s
	

	
	// fclose
	fclose(fp);

	// TODO 10_12
	// ���\�[�X�����



}

void ClientAssignment10::Decrypt(const char* filename, const unsigned char* encodeText, size_t encodeSize, unsigned char** decode, size_t* decodeSize)
{
	// �閧���t�@�C���̃I�[�v��
	FILE* fp;
	if (fopen_s(&fp, filename, "r") != 0) {
		Logger::Print("�t�@�C���̃I�[�v���G���[: %s\n", filename);
		return;
	}

	// TODO 10_13
	// �閧�����t�@�C������ǂݍ���
	EVP_PKEY* pKey;


	// TODO 10_14
	// EVP�R���e�L�X�g�̐���
	EVP_PKEY_CTX* ctx;


	// TODO 10_15
	// �������̏�����



	// TODO 10_16
	// �p�f�B���O�ݒ�



	// TODO 10_17
	// ��������̃T�C�Y���擾



	// TODO 10_18
	// �������f�[�^�p�̃������𐄑�



	// TODO 10_19
	// �����������s


	// fclose
	fclose(fp);

	// TODO 10_20
	// ���\�[�X�̉��
	

}

void ClientAssignment10::Sign(char* filename, unsigned char* planeText, size_t planeSize, unsigned char** encode, size_t* encodeSize)
{ 
	// �閧���t�@�C���̃I�[�v��
	FILE* fp;
	if (fopen_s(&fp, filename, "r") != 0) {
		Logger::Print("�t�@�C���̃I�[�v���G���[: %s\n", filename);
		return;
	}

	// TODO 10_21
	// �閧�����t�@�C������ǂݍ���
	EVP_PKEY* pKey;


	// TODO 10_22
	// �����R���e�L�X�g�̐���
	EVP_PKEY_CTX* ctx;

	// TODO 10_23
	// �����̏�����


	// TODO 10_24
	// �p�f�B���O�ݒ�


	// TODO 10_25
	// ������̃T�C�Y���擾


	// TODO 10_26
	// �����f�[�^�p�̃��������m��


	// TODO 10_27
	// ���������s


	// fclose
	fclose(fp);

	// TODO 10_28
	// ���\�[�X�̉��


}

void ClientAssignment10::Verify(char* filename, unsigned char* encodeText, size_t encodeSize, unsigned char** decode, size_t* decodeSize)
{
	// ���J���t�@�C���̃I�[�v��
	FILE* fp;
	if (fopen_s(&fp, filename, "r") != 0) {
		Logger::Print("�t�@�C���̃I�[�v���G���[: %s\n", filename);
		return;
	}

	// TODO 10_29
	// ���J�����t�@�C������ǂݍ���
	EVP_PKEY* pKey;


	// TODO 10_30
	// �����m�F�R���e�L�X�g�̐���
	EVP_PKEY_CTX* ctx;


	// TODO 10_31
	// �����m�F�̏�����


	// TODO 10_32
	// �p�f�B���O�ݒ�


	// TODO 10_33
	// �����m�F�̃T�C�Y���擾


	// TODO 10_34
	// �����m�F�f�[�^�p�̃��������m��


	// TODO 10_35
	// �����m�F�����s


	// fclose
	fclose(fp);

	// TODO 10_36
	// ���\�[�X�̉��


}

std::string ClientAssignment10::ConvertingToHexNumber(const unsigned char* bytes, const size_t length)
{
	std::ostringstream oss;

	// �e��16�i���ɕϊ����ĕ\������
	for (size_t i = 0; i < length; i++) {
		oss << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(bytes[i]);
	}
	return oss.str();
}
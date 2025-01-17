#include <string>
#include <fstream>
#include <WinSock2.h>		// WinSock version 2���g�p���邽�߂̃w�b�_
#include <WS2tcpip.h>		// WinSock version 2�Ŏg�p����l�X�ȃI�v�V�����̃w�b�_
#include <openssl/sha.h>
#include <openssl/md5.h>
#include <openssl/evp.h>
#include "08ClientAssignment.h"
#include <algorithm>
#include <windows.h>
#include <commdlg.h>
#include <iomanip>
#include <sstream>
#include <string>

ClientAssignment08::ClientAssignment08()
{

}

ClientAssignment08::~ClientAssignment08()
{

}

void ClientAssignment08::Initialize()
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

void ClientAssignment08::Finalize()
{

}

void ClientAssignment08::Render()
{
	// ���N���X�̃����_�[�Ăяo��
	SceneBase::Render();
	// imgui
	Graphics& graphics = Graphics::Instance();
	float screenWidth = static_cast<float>(graphics.GetScreenWidth());
	ImGui::SetNextWindowPos(ImVec2(screenWidth / 2 - 150, 100), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(300, 480), ImGuiCond_Once);

	if (ImGui::Begin(u8"Hash�l", nullptr, ImGuiWindowFlags_None))
	{
		ImGui::TextWrapped(u8"�ۑ�1_�t�@�C����hash������");
		ImGui::TextWrapped(u8"%s", path.c_str());
		if (ImGui::Button(u8"�t�@�C���I��"))
		{
			OPENFILENAME openFileName;//�t�@�C����
			wchar_t filePathW[256]{ 0 };

			// �\���̏�����
			ZeroMemory(&openFileName, sizeof(openFileName));
			openFileName.lStructSize = sizeof(openFileName);			// �\���̂̃T�C�Y
			openFileName.hwndOwner = NULL;								// �I�[�i�[�E�B���h�E�̃n���h��
			openFileName.lpstrFile = filePathW;							// �I�����ꂽ�t�@�C�����̃o�b�t�@
			openFileName.nMaxFile = sizeof(filePathW) / sizeof(wchar_t);	// �t�@�C�����̃o�b�t�@�T�C�Y
			openFileName.lpstrFilter = L"All Files\0*.*\0";				// �t�B���^��ݒ�
			openFileName.nFilterIndex = 1;								// �f�t�H���g�őI������t�B���^�̃C���f�b�N�X
			openFileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; // �_�C�A���O�̓���t���O
			if (GetOpenFileName(&openFileName)) 
			{
				// wchar_t��char(ascii)�ϊ�
				char filePath[256] = { 0 };
				::WideCharToMultiByte(CP_ACP, 0, filePathW, -1, filePath, sizeof(filePath), nullptr, nullptr);
				// wchar_t��char(utf8)�ϊ�
				char outputPath[256] = { 0 };
				::WideCharToMultiByte(CP_UTF8, 0, filePathW, -1, outputPath, sizeof(outputPath), nullptr, nullptr);
				// ���ʂ��o��
				Logger::Print("Open File:%s\n", filePath);
				path = outputPath;
				// �n�b�V�����֐����s
				md5Hash = MD5Hashing(filePath);
			}
		}
		ImGui::Text("MD5");
		ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(250, 50), ImGuiWindowFlags_NoTitleBar);
		ImGui::TextWrapped("%s", md5Hash.c_str());
		ImGui::EndChild();

		ImGui::TextWrapped(u8"�ۑ�2_�e�L�X�g��hash������");

		ImGui::InputText("SHA512", planeText, 512);
		if (ImGui::Button(u8"sha512�n�b�V����"))
		{
			sha512Hash = Sha512Hashing(planeText);
		}
		ImGui::Text("SHA512");
		ImGui::BeginChild(ImGui::GetID((void*)1), ImVec2(250, 140), ImGuiWindowFlags_NoTitleBar);
		ImGui::TextWrapped("%s", sha512Hash.c_str());
		ImGui::EndChild();
		if (ImGui::Button(u8"�^�C�g����"))
		{
			SceneManager::Instance().ChangeScene(new SceneTitle());
		}

	}
	ImGui::End();
}

std::string ClientAssignment08::MD5Hashing(std::string data)
{
	// �ǂݍ��񂾃f�[�^��ۑ�
	std::string readData;

	std::cout << "md5 crypto file name : " << data << std::endl;

	// �t�@�C���̃I�[�v��
	std::ifstream readFile(data, std::ios::in | std::ios::binary);
	if (!readFile) {

		// �t�@�C�����I�[�v���ł��Ȃ�����
		std::cout << "�t�@�C���I�[�v�����s" << std::endl;
		return "";
	}

	int bufferSize = 1024;

	// �o�b�t�@�̊m��
	char buffer[1024];

	/*
		�t�@�C���̍Ō�܂œǂݍ���
	*/
	while (!readFile.eof()) {

		// �t�@�C����ǂݍ���
		readFile.read(buffer, bufferSize);

		// �ǂݍ��񂾃T�C�Y����������֒ǉ�
		readData.append(buffer, readFile.gcount());
	}
	// TODO 08_01
	// md�R���e�L�X�g�ɕK�v�ȏ���ݒ�
	//���L�L�q3�s���ۑ�v�����g�ɍ��킹�C��
	EVP_MD_CTX* mdctx{};
	unsigned int md5Length = 0;// �����T�C�Y�擾
	unsigned char* md5{};//�������m�ۂ��ď�����

	// TODO 08_02
	// MD�R���e�L�X�g������


	// TODO 08_03
	// �n�b�V�����������l���w��


	// TODO 08_04
	// �n�b�V�������s
	
	// TODO 08_05
	// md5��16�i��2���\���ɕϊ�
	std::string hashData;

	// TODO 08_06
	// MD�R���e�L�X�g�J��

	return hashData;
}

std::string ClientAssignment08::Sha512Hashing(std::string data)
{
	// TODO 08_07
	// SHA512�R���e�L�X�g�ɕK�v�ȏ���ݒ�
	//���L3�s��ύX
	EVP_MD_CTX* sha512ctx{};
	unsigned int shaLength = 0;// �����T�C�Y�擾
	unsigned char* sha512{};//�������m�ۂ��ď�����

	// TODO 08_08
	// SHA512������


	// TODO 08_09
	// �n�b�V�����������l���w��
	

	// TODO 08_10
	// �n�b�V�������s
	
	
	std::string hashData;
	// TODO 08_11
	// sha512��16�i��2���\���ɕϊ�

	// TODO 08_12
	// SHA256�R���e�L�X�g�J��


	return hashData;
}

std::string ClientAssignment08::ConvertingToHexNumber(const unsigned char* bytes,const size_t length)
{
	std::ostringstream oss;

	// �e��16�i���ɕϊ����ĕ\������
	for (size_t i = 0; i < length; i++) {
		oss << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(bytes[i]);
	}
	return oss.str();
}
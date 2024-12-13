#include <string>
#include <fstream>
#include <WinSock2.h>		// WinSock version 2���g�p���邽�߂̃w�b�_
#include <WS2tcpip.h>		// WinSock version 2�Ŏg�p����l�X�ȃI�v�V�����̃w�b�_
// OpenSSL�ǂݍ���
#include <openssl/crypto.h>		// openssl�̈Í����w�b�_
#include <openssl/ssl.h>		// openssl��ssl�ʐM�w�b�_
#include <openssl/err.h>		// openssl�̃G���[�w�b�_

#include "07ClientAssignment.h"
#include <algorithm>

//----------�萔----------------------------------------
#define PL_POS DirectX::XMFLOAT3(0.0f,0.0f,0.0f)
#define PL_SIZE DirectX::XMFLOAT3(0.2f,0.2f,0.2f)
#define WIND_POS ImVec2(screenWidth/2-150,200)
#define WIND_SIZE ImVec2(300,100)
#define DOWNLOAD_PORT "443"

#define DATA_KEEP_SIZE 4096  	//�����T�C�Y���m��
#define BUFFER_SIZE	1024
#define REQUEST_BUF_SIZE 1024
//---------------------------------------------------------------------------

//------------------------------------------------------------//
//�R���X�g���N�^
ClientAssignment07::ClientAssignment07()
{

}

//------------------------------------------------------------//
//�f�X�g���N�^
ClientAssignment07::~ClientAssignment07()
{

}

//------------------------------------------------------------//
//������
void ClientAssignment07::Initialize()
{
	SceneBase::Initialize();

	// Player�ݒ�
	playerManager = new PlayerManager();
	playerManager->SetMyPlayerID(0);

	Player* player = new Player();


	// �e�v���C���[�̃L�����N�^�[�ɍ��킹���ݒ���s��
	// JobClass�̓X�e�[�g�}�V��
	player->SetPlayerID(0);
	player->SetPosition( PL_POS );
	player->SetScale( PL_SIZE );
	player->SetModel();

	playerManager->AddPlayer(player);
	playerManager->GetMyPlayer()->SetReady(true);
	SSL_load_error_strings();
	OpenSSL_add_ssl_algorithms();
}

//------------------------------------------------------------//
//�I������
void ClientAssignment07::Finalize()
{
	if (recvTh.joinable())
	{
		recvTh.join();
	}
	OPENSSL_cleanup();
	CRYPTO_cleanup_all_ex_data();
	ERR_free_strings();
}

//------------------------------------------------------------//
//�����_�[����
void ClientAssignment07::Render()
{
	// ���N���X�̃����_�[�Ăяo��
	SceneBase::Render();
	// imgui
	Graphics& graphics = Graphics::Instance();
	float screenWidth = static_cast<float>(graphics.GetScreenWidth());
	ImGui::SetNextWindowPos(WIND_POS, ImGuiCond_Once);
	ImGui::SetNextWindowSize(WIND_SIZE, ImGuiCond_Once);
	if (ImGui::Begin("Download", nullptr, ImGuiWindowFlags_None))
	{
		if (ImGui::Button("Start"))
		{
			// TODO 07_01
			// �_�E�����[�h�t�@�C���̎w��
			// �z�X�g��(�h���C������)
			std::string hostname = "";
			// �p�X
			std::string path = "";
			// �t�@�C����
			std::string filename = "";

			if (recvTh.joinable())recvTh.join();
			recvTh = std::thread(&ClientAssignment07::FileDownload, this, hostname, path, filename);
		}
		if (ImGui::Button(u8"�^�C�g����"))
		{
			SceneManager::Instance().ChangeScene(new SceneTitle());
		}

	}
	ImGui::End();
}

//------------------------------------------------------------//
//�t�@�C���̃_�E�����[�h�@SSL/TLS�@���C����
void ClientAssignment07::FileDownload(const std::string& hostname, const std::string& path, const std::string& filename)
{
	// TODO 07_02
	// OpenSSL�̏�����

	WSADATA wsaData;
	SOCKET sock = INVALID_SOCKET;
	addrinfo hints = {}, * addrInfo = nullptr;

	try {
		// WSA������
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
			throw std::runtime_error("WSA���������s");
		}

		// �ʐM�����ݒ�
		hints.ai_family = AF_INET;			  // IPv4�ݒ�
		hints.ai_socktype = SOCK_STREAM;		// TCP�ݒ�
		hints.ai_protocol = IPPROTO_TCP;

		// �h���C���ƃ|�[�g�ԍ�����A�h���X�����擾����
		const std::string port = DOWNLOAD_PORT;
		if (getaddrinfo(hostname.c_str(), port.c_str(), &hints, &addrInfo) != 0) {
			throw std::runtime_error("�h���C������A�h���X�擾�Ɏ��s���܂���");
		}

		// �\�P�b�g�쐬
		sock = socket(addrInfo->ai_family, addrInfo->ai_socktype, addrInfo->ai_protocol);
		if (sock == INVALID_SOCKET) {
			throw std::runtime_error("�\�P�b�g�̐����Ɏ��s���܂���");
		}

		// �T�[�o�ɐڑ�
		if (connect(sock, addrInfo->ai_addr, static_cast<int>(addrInfo->ai_addrlen)) == SOCKET_ERROR) {
			throw std::runtime_error("connect�Ɏ��s���܂���");
		}

		// TODO 07_03
		// SSL�R���e�L�X�g�쐬
		SSL_CTX* ctx;

		// TODO 07_04
		// SSL�I�u�W�F�N�g�쐬
		SSL* ssl;

		// TODO 07_05
		// SSL�I�u�W�F�N�g�Ƀ\�P�b�g���֘A�t����


		// TODO 07_06
		// �T�[�o�ɐڑ�
		

		// TODO 07_07
		// HTTP���N�G�X�g���쐬
		char request[REQUEST_BUF_SIZE];
		
		
		// TODO 07_08
		// ���N�G�X�g���M


		Logger::Print("�T�[�o����̃��X�|���X\n");
		std::vector<char> data;
		data.reserve(DATA_KEEP_SIZE); // �����T�C�Y���m��
		char buf[BUFFER_SIZE];
		int size = 0;

		// TODO 07_09
		// �f�[�^��M���[�v



		if (size < 0) {
			throw std::runtime_error("��M�G���[");
		}

		// HTTP�w�b�_�[�̍폜
		auto headerEnd = std::search(data.begin(), data.end(), "\r\n\r\n", "\r\n\r\n" + 4);
		if (headerEnd != data.end()) {
			data.erase(data.begin(), headerEnd + 4);
		}

		// �t�@�C�������o��
		std::ofstream writingFile(filename, std::ios::binary);
		if (!writingFile) {
			throw std::runtime_error("�t�@�C���̃I�[�v���Ɏ��s���܂���");
		}
		writingFile.write(data.data(), data.size());

		// TODO 07_10
		// ���\�[�X���


		closesocket(sock);
		WSACleanup();
	}
	catch (const std::exception& ex) {
		Logger::Print("�G���[:%s\n",ex.what());
		if (sock != INVALID_SOCKET) closesocket(sock);
		WSACleanup();
	}
}
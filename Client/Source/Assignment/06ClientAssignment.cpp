#include <WinSock2.h>
#include <ws2tcpip.h>
#include "06ClientAssignment.h"



ClientAssignment06::ClientAssignment06()
{
	input[0] = '\0';
}

ClientAssignment06::~ClientAssignment06()
{

}

void ClientAssignment06::Initialize()
{
	SceneBase::Initialize();
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		// ���������s
		Logger::Print("WSA Initialize Failed.\n");
		return;
	}

	// �ڑ�����ݒ�
	addr.sin_family = AF_INET;
	addr.sin_port = htons(7000);

	// �ڑ���IP�ݒ�
	addr.sin_addr.S_un.S_un_b.s_b1 = 127;
	addr.sin_addr.S_un.S_un_b.s_b2 = 0;
	addr.sin_addr.S_un.S_un_b.s_b3 = 0;
	addr.sin_addr.S_un.S_un_b.s_b4 = 1;

	// ���M����ۂ̃|�[�g���w�肷��
	self.sin_family = AF_INET;
	self.sin_port = htons(7001);

	// �ڑ���IP�ݒ�
	self.sin_addr.S_un.S_un_b.s_b1 = 0;
	self.sin_addr.S_un.S_un_b.s_b2 = 0;
	self.sin_addr.S_un.S_un_b.s_b3 = 0;
	self.sin_addr.S_un.S_un_b.s_b4 = 0;


	// �\�P�b�g�쐬
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == INVALID_SOCKET) {
		Logger::Print("create socket Failed.\n");
		return;
	}
	// �m���u���b�L���O�ݒ�
	u_long mode = 1;
	if (ioctlsocket(sock, FIONBIO, &mode) != 0)
	{
		Logger::Print("Nonblocking Mode Failed.\n");
		return;
	}
	// TODO 06_06
	// �}���`�L���X�g�\�P�b�g�쐬(�T�[�o�����M�p)


	// TODO 06_07
	// �}���`�L���X�g�A�h���X�̐ݒ�


	// TODO 06_08
	// �}���`�L���X�g�O���[�v�֓o�^����( join )
	// �}���`�L���X�g�O���[�v�p�\����ip_mreq���g�p����
	// �o�^������C���^�[�t�F�[�X�̃A�h���X�ݒ���s������



	// TODO 06_09
	// �}���`�L���X�g�A�h���X�Ƀ\�P�b�g��o�^


	// TODO 06_10
	// �m���u���b�L���O�ݒ�


	// sendto�֐��ŃT�[�o�Ƀf�[�^�𑗐M����B
	int size = sendto(sock, "connect", 8, 0, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));
	if (size < 0)
	{
		Logger::Print("connected failed. error code:%d\n", WSAGetLastError());
	}
	else
	{
		Logger::Print("Connect Success\n");
	}

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

	// ��M�X���b�h����
	th = std::thread(&ClientAssignment06::recvThread, this);
}

void ClientAssignment06::Finalize()
{
	// �}���`�X���b�h�̃��[�v�t���O�����낷
	loop = false;

	// �X���b�h�̏I���܂őҋ@
	th.join();

	// server�I������
	int r = closesocket(sock);
	if (r != 0) {
		int err = WSAGetLastError();
		Logger::Print("Close Socket Failed.error_code:%d\n", err);
	}
	int wsacleanup = WSACleanup();
}

void ClientAssignment06::Render()
{
	// ���N���X�̃����_�[�Ăяo��
	SceneBase::Render();
	// imgui
	ImGui::SetNextWindowPos(ImVec2(50, 50), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(300, 620), ImGuiCond_Once);
	if (ImGui::Begin("Chat", nullptr, ImGuiWindowFlags_None))
	{
		ImGui::Text("Message");
		ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(250, 470), ImGuiWindowFlags_NoTitleBar);
		for (std::string message : messages) {
			ImGui::Text(u8"%s", message.c_str());
		}
		ImGui::EndChild();
		ImGui::Spacing();

		ImGui::InputText("Message", input, sizeof(input));
		if (ImGui::Button("Send"))
		{
			if (strcmp(input, "") != 0)
			{
				// ���M����
				int size = sendto(sock, input, static_cast<int>(strlen(input) + 1), 0, reinterpret_cast<sockaddr*>(&addr), static_cast<int>(sizeof(addr)));
				if (size < 0)
				{
					Logger::Print("Error Code:%d\n" ,WSAGetLastError());
				}
			}
			input[0] = '\0';
		}
		if (ImGui::Button(u8"�^�C�g����"))
		{
			SceneManager::Instance().ChangeScene(new SceneTitle());
		}

	}
	ImGui::End();
}

void ClientAssignment06::recvThread()
{
	int size = sizeof(sockaddr_in);
	do {
		char buffer[256]{};
		int r = recvfrom(multicastSock, buffer, sizeof(buffer), 0, reinterpret_cast<sockaddr*>(&recvAddr), &size);
		if (r > 0)
		{
			messages.emplace_back(buffer);
		}
		else
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				Logger::Print("recvfrom failed. error code:%d\n", WSAGetLastError());
			}
		}
	} while (loop);

}
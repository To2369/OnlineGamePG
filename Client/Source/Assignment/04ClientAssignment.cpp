#include "04ClientAssignment.h"

// ���������[�N�����p
#define debug_new new(_NORMAL_BLOCK,__FILE__,__LINE__)


ClientAssignment04::ClientAssignment04() :messages(15)
{
	input.buffer[0] = '\0';
}

ClientAssignment04::~ClientAssignment04()
{

}

void ClientAssignment04::Initialize()
{
	SceneBase::Initialize();
	// WinsockAPI��������
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		// ���������s
		Logger::Print("WSA Initialize Failed.\n");
		return;
	}


	// DNS�֖₢���킹
	addrinfo hints;				// DNS�֖₢���킹�ɕK�v�ȏ���ݒ�
	addrinfo* addrInfo = NULL;	// �擾�����A�h���X�������ɕۑ������
	// �[���N���A
	ZeroMemory(&hints, sizeof(addrinfo));
	// �ݒ肷�����ݒ�
	hints.ai_family = AF_INET;				// IPv4�Ŏ擾
	hints.ai_socktype = SOCK_STREAM; 		// TCP�ʐM��DNS�T�[�o�փA�N�Z�X

	const char hostname[] = "localhost";	// �h���C���w��
	const char port[] = "7000";				// �|�[�g�ԍ��w��
	// DNS�֖₢���킹0�̂Ƃ�����Ɋ���
	if (getaddrinfo(hostname, port, &hints, &addrInfo) != 0) {
		Logger::Print("getaddrinfo error.\n");

		return;
	}

	// WinsockAPI��������
	// �擾����IP�A�h���X��sockaddr_in�ɕϊ���sin_addr����
	addr.sin_addr = reinterpret_cast<sockaddr_in*>(addrInfo->ai_addr)->sin_addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(7000);


	// �\�P�b�g�쐬
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == INVALID_SOCKET) {
		Logger::Print("create socket Failed.\n");
		return;
	}

	// �m���u���b�L���O�ݒ�
	u_long mode = 1;
	int m = ioctlsocket(sock, FIONBIO, &mode);
	if (m != 0)
	{
		Logger::Print("Nonblocking Mode Failed.\n");
		return;
	}

	// �N���C�A���g�����T�[�o�֒m�点�邽�߂�sendto�֐����ŏ��Ɏ��s����B
	PlayerMessage message;
	message.cmd = NetworkTag::Message;
	strcpy_s(message.buffer, "connect");
	int size = sendto(sock, reinterpret_cast<char*>(&message), sizeof(message), 0, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));
	if (size < 0)
	{
		Logger::Print("connected failed. error code:%d\n", WSAGetLastError());
	}
	Logger::Print("Connect Success\n");

	// Player�ݒ�
	playerManager = debug_new PlayerManager();
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
	recvTh = std::thread(&ClientAssignment04::recvThread, this);
}

void ClientAssignment04::Finalize()
{

	// �}���`�X���b�h�̃��[�v�t���O�����낷
	loop = false;

	// �\�P�b�g���u���b�L���O�Ȃ̂ŏI���R�[�h�𑗂�
	PlayerLogout logout{};
	logout.cmd = NetworkTag::Logout;
	logout.id = playerManager->GetMyPlayerID();

	int s = sendto(sock, reinterpret_cast<char*>(&logout), sizeof(logout), 0,
		reinterpret_cast<sockaddr*>(&addr), static_cast<int>(sizeof(addr)));
	if (s < 0)
	{
		Logger::Print("finalize send failed. error code:%d\n");
	}

	// �X���b�h�̏I���܂őҋ@
	recvTh.join();

	// server�I������
	int r = closesocket(sock);
	if (r != 0) {
		int err = WSAGetLastError();
		Logger::Print("Close Socket Failed.error_code:%d\n", err);
	}

	int wsacleanup = WSACleanup();
}

void ClientAssignment04::Render()
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

		for (int i = 0; i < messages.size();++i) {
			ImGui::Text(u8"%s", messages.at(i).buffer);
		}

		ImGui::EndChild();
		ImGui::Spacing();
		input.cmd = NetworkTag::Message;
		ImGui::InputText("Message", input.buffer, sizeof(input.buffer));
		if (ImGui::Button("Send"))
		{
			if (strcmp(input.buffer, "") != 0)
			{
				// ���M����
				int size = sendto(sock, reinterpret_cast<char *>(&input), sizeof(input), 0, reinterpret_cast<sockaddr*>(&addr), static_cast<int>(sizeof(addr)));
				if (size < 0)
				{
					Logger::Print("Error Code:%d\n");
				}
			}
			input.buffer[0] = '\0';
		}
		if (ImGui::Button(u8"�^�C�g����"))
		{
			SceneManager::Instance().ChangeScene(new SceneTitle());
		}
	}
	ImGui::End();
}


void ClientAssignment04::recvThread()
{
	int size = sizeof(sockaddr_in);
	do {
		char buffer[2048]{};
		int r = recvfrom(sock, reinterpret_cast<char*>(&buffer), sizeof(buffer), 0, reinterpret_cast<sockaddr*>(&recvAddr), &size);
		if (r > 0)
		{
			// �^�O�������擾�������𕪊򂳂���
			short type = 0;
			memcpy_s(&type, sizeof(short), buffer, sizeof(short));
			switch (static_cast<NetworkTag>(type))
			{
				case NetworkTag::Message:
				{
					PlayerMessage message;
					memcpy_s(&message, sizeof(message), buffer, sizeof(message));

					// TODO 04_04
					// ��M�������b�Z�[�W�������O�o�b�t�@�ɕۑ�����B

					break;
				}
			}
		}
	} while (loop);
}

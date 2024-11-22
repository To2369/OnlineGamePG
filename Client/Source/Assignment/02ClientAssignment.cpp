#include "02ClientAssignment.h"

//-�萔----------------------------------------------

#define BUFFER_SIZE	128
#define XYZ000	DirectX::XMFLOAT3(0.0f,0.0f,0.0f)
#define PLAYER_SIZE DirectX::XMFLOAT3(0.02f,0.02f,0.02f)
#define MESS_WIND_SIZE_X	250
#define MESS_WIND_SIZE_Y	400
#define COM_WIND_SIZE_X 	420
#define COM_WIND_SIZE_Y		650
#define	COM_WIND_POS_X		50
#define COM_WIND_POS_Y		30
#define PORT			7000
#define PORTSTR			"7000"
//-------�R���X�g���N�^-------------------------------------------------
ClientAssignment02::ClientAssignment02()
{
	input[0] = '\0';
}

//-------�f�X�g���N�^-------------------------------------------------
ClientAssignment02::~ClientAssignment02()
{
	
}

//-------���������\�b�h-------------------------------------------------
void ClientAssignment02::Initialize()
{
	//���������\�b�h���ƑO��쐬�������̂̃R�s�[���y�[�X�g��OK�ł����A
	//�Ō�̃X���b�h�����グ��
	//th = std::thread(&ClientAssignment02::recvThread, this);
	//�Ȃ̂Œ���

	SceneBase::Initialize();
	// TODO 01_10
	// WinsockAPI��������
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		// ���������s 
		Logger::Print("WSA Initialize Failed.\n");
		return;
	}

	addrinfo hints;			// DNS�֖₢���킹�ɕK�v�ȏ���ݒ�
	addrinfo* addrInfo = NULL;	// �擾�����A�h���X�������ɕۑ������
	// �[���N���A
	ZeroMemory(&hints, sizeof(addrinfo));
	// TODO 01_11
	// DNS�₢���킹�Ɋւ������ݒ�
	// DNS�֖₢���킹

	hints.ai_family = AF_INET;   // IPv4�Ŏ擾 
	hints.ai_socktype = SOCK_STREAM;  // TCP�ʐM��DNS�T�[�o�փA�N�Z�X 

	const char hostname[] = "localhost"; // �h���C���w�� 
	const char port[] = "7000";  // �|�[�g�ԍ��w�� 
	// DNS�֖₢���킹0�̂Ƃ�����Ɋ��� 
	if (getaddrinfo(hostname, port, &hints, &addrInfo) != 0) {
		Logger::Print("getaddrinfo error.\n");
		return;
	}

	// TODO 01_12
	// WinsockAPI��������
	// �擾����IP�A�h���X��sockaddr_in�ɕϊ���sin_addr����
	addr.sin_addr = reinterpret_cast<sockaddr_in*>(addrInfo->ai_addr)->sin_addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);

	// TODO 01_13
	// �\�P�b�g�쐬
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == INVALID_SOCKET) {
		Logger::Print("create socket Failed.\n");
		return;
	}
	// TODO 01_14
	// �m���u���b�L���O�ݒ�
	u_long mode = 1;
	int m = ioctlsocket(sock, FIONBIO, &mode);
	if (m != 0)
	{
		Logger::Print("Nonblocking Mode Failed.\n");
		return;
	}

	// TODO 01_15
	// �N���C�A���g�����T�[�o�֒m�点�邽�߂�sendto�֐����ŏ��Ɏ��s����B


	int size = sendto(sock, "connect", 8, 0, reinterpret_cast<sockaddr*>(&addr),
		sizeof(addr));
	if (size < 0)
	{
		Logger::Print("connected failed. error code:%d\n", WSAGetLastError());
	}

	Logger::Print("Connect Success\n");

	// Player�ݒ�
	playerManager = new PlayerManager();
	playerManager->SetMyPlayerID(0);

	Player* player = new Player();

	// �e�v���C���[�̃L�����N�^�[�ɍ��킹���ݒ���s��
	// JobClass�̓X�e�[�g�}�V��
	player->SetPlayerID(0);
	player->SetPosition(XYZ000);
	player->SetScale(PLAYER_SIZE);
	player->SetState(Player::State::Idle);
	player->SetModel();
	player->SetPlayerID(0);



	playerManager->AddPlayer(player);
	playerManager->GetMyPlayer()->SetReady(true);

	// ��M�X���b�h����
	th = std::thread(&ClientAssignment02::recvThread, this);

}

//-------�I�����\�b�h-------------------------------------------------
void ClientAssignment02::Finalize()
{
	//�I�����\�b�h���O��쐬�������̂̃R�s�[���y�[�X�g��OK�ł��B

	// �}���`�X���b�h�̃��[�v�t���O�����낷
	loop = false;

	// �X���b�h�̏I���܂őҋ@
	th.join();

	// TODO 01_18
	// �\�P�b�g�I��
	if (closesocket(sock) != 0) {
		int err = WSAGetLastError();
		Logger::Print("Close Socket Failed.error_code:%d\n", err);
		{
		}
	}

	// TODO 01_19
	// WSA�I������
	if (WSACleanup() != 0)
	{
		int err = WSAGetLastError();
		Logger::Print("Cleanup WinsockAPI Failed.%d\n", err);
	}

}

//-------�`�惁�\�b�h-------------------------------------------------
void ClientAssignment02::Render()
{
	// ���N���X�̃����_�[�Ăяo��
	SceneBase::Render();
	// imgui
	ImGui::SetNextWindowPos(ImVec2(50, 50), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(300, 100), ImGuiCond_Once);
	if (ImGui::Begin("Title", nullptr, ImGuiWindowFlags_None))
	{
		if (ImGui::Button(u8"�^�C�g����"))
		{
			SceneManager::Instance().ChangeScene(new SceneTitle());
		}
	}
	ImGui::End();
}

//-------�X�V�������\�b�h-------------------------------------------------
void ClientAssignment02::NetrowkUpdate(float elapsedTime)
{
	// ����L�����N�^�[�擾
	Player* player = playerManager->GetMyPlayer();

	// �J�����R���g���[���[�X�V����
	if (player != nullptr)
	{
		HWND hWnd;
		hWnd = GetActiveWindow();
		if (hWnd != NULL)
		{
			// �L�[���͔���(���̓f�[�^�͂��̂܂܃T�[�o�ɑ��炸�X�V)
			switch (player->GetState())
			{
			case Player::State::Idle:
				if (player->InputMove(elapsedTime))
				{
					if (player)player->GetJobClass()->ChangeState(Player::State::Move);
				}
				if (player->InputAttack())
				{
					player->GetJobClass()->ChangeState(Player::State::Attack);
				}
				break;
			case Player::State::Move:
				if (!player->InputMove(elapsedTime))
				{
					player->GetJobClass()->ChangeState(Player::State::Idle);
				}
				if (player->InputAttack())
				{
					player->GetJobClass()->ChangeState(Player::State::Attack);
				}
				break;
			case Player::State::Attack:
				if (!player->GetModel()->IsPlayAnimation())
				{
					player->GetJobClass()->ChangeState(Player::State::Idle);
				}
				break;
			}
		}
	}

	PlayerInformation plInfo{};
	PlayerInformation plInfoOld{};
	// ���M�f�[�^
	plInfo.position = player->GetPosition();
	plInfo.angle = player->GetAngle();
	plInfo.state = player->GetState();
	int size = 0;
	//����V�K�쐬����C01
	// TODO 02_10
	// plInfo���T�[�o�ɑ��M����
	size = sendto(sock, reinterpret_cast<char*>(&plInfo), sizeof(PlayerInformation),
		0, reinterpret_cast<sockaddr*>(&addr), static_cast<int>(sizeof(sockaddr_in)));
}


//-------�T�[�o�[����̎�M�X���b�h-------------------------------------------------

void ClientAssignment02::recvThread()
{
	int len = sizeof(sockaddr_in);
	do {
		PlayerInformation plInfo{};
		//����V�K�쐬����C02
		// TODO 02_11
		// �f�[�^��M���s���A
		//�v���C���[������M����
		// ��M�f�[�^��plInfo�ɕۑ������Ńv���C���[�ɂ͔��f���Ȃ�
		int size = recvfrom(sock,
				reinterpret_cast<char*>(&plInfo),
				sizeof(PlayerInformation), 0,
			reinterpret_cast<sockaddr*>(&recvAddr), &len);
		if (size < 0)
		{
			WSAGetLastError();
		}


	} while (loop);
}
#include <WinSock2.h>
#include "03ClientAssignment.h"

// ���������[�N�����p
#define debug_new new(_NORMAL_BLOCK,__FILE__,__LINE__)


ClientAssignment03::ClientAssignment03()
{
	input[0] = '\0';
}

ClientAssignment03::~ClientAssignment03()
{
	
}

void ClientAssignment03::Initialize()
{
	SceneBase::Initialize();
	int wsaStartUp = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (wsaStartUp != 0)
	{
		// ���������s
		Logger::Print("WSA Initialize Failed.");
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
		Logger::Print("create socket Failed.");
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
	// �V�K���[�U��ID�̓_�~�[�f�[�^�Ƃ���-1�ɂ��Ă���
	PlayerLogin login{};
	login.cmd = NetworkTag::Login;
	login.id = -1;
	// TODO 03_07
	// login�f�[�^�̑��M




	// Player�ݒ�
	playerManager = debug_new PlayerManager();

	// ��M�X���b�h����
	recvTh = std::thread(&ClientAssignment03::recvThread, this);
}


void ClientAssignment03::Finalize()
{

	// �}���`�X���b�h�̃��[�v�t���O�����낷
	loop = false;

	PlayerLogout logout{};
	logout.cmd = NetworkTag::Logout;
	logout.id = playerManager->GetMyPlayerID();
	int size = 0;
	
	// TODO 03_10
	// logout�f�[�^�𑗐M



	// �X���b�h�̏I���܂őҋ@
	recvTh.join();

	// server�I������
	if (closesocket(sock) != 0) {
		int err = WSAGetLastError();
		Logger::Print("Close Socket Failed.error_code:%d\n", err);
		{
		}
	}

	// WSA�I������
	if (WSACleanup() != 0)
	{
		int err = WSAGetLastError();
		Logger::Print("Cleanup WinsockAPI Failed.%d\n", err);
	}
}

void ClientAssignment03::Render()
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

void ClientAssignment03::NetrowkUpdate(float elapsedTime)
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


		// ���t���[�����M���铯���f�[�^
		PlayerInformation plInfo{};
		plInfo.cmd = NetworkTag::Sync;

		// ���M�f�[�^
		plInfo.id = player->GetPlayerID();
		plInfo.position = player->GetPosition();
		plInfo.angle = player->GetAngle();
		plInfo.state = player->GetState();

		// TODO 03_08
		// �����f�[�^�𑗐M

	}
}

void ClientAssignment03::recvThread()
{
	int size = sizeof(sockaddr_in);
	do {
		char buffer[2048]{};
		int r = recvfrom(sock, reinterpret_cast<char*>(&buffer), sizeof(buffer), 0, reinterpret_cast<sockaddr*>(&recvAddr), &size);
		if (r > 0)
		{
			// TODO 03_09
			// ��M�f�[�^�̏����̃R�[�h��ǂݗ������s�����ƁB
			
			// ��M�f�[�^����l�b�g���[�N�^�O���擾
			short type = 0;
			memcpy_s(&type, sizeof(short), buffer, sizeof(short));
			switch (static_cast<NetworkTag>(type))
			{
				case NetworkTag::Sync:
				{
					// Sync�^�O�̏ꍇ
					// ��M�f�[�^��plInfo�ɃR�s�[����
					PlayerInformation plInfo{};
					memcpy_s(&plInfo, sizeof(PlayerInformation), buffer, sizeof(PlayerInformation));

					// ��M�f�[�^�����O�o��
					Logger::Print("recv data id:%d\n", plInfo.id);
					Logger::Print("position x:%d", plInfo.position.x);
					Logger::Print("y:%d", plInfo.position.y);
					Logger::Print("z:%d\n", plInfo.position.z);
					Logger::Print("angle x:%d", plInfo.angle.x);
					Logger::Print("y:%d", plInfo.angle.y);
					Logger::Print("z:%d", plInfo.angle.z);
					Logger::Print("state:%d\n", static_cast<int>(plInfo.state));

					bool inserted = false;
					// id���o�^����Ă��邩�m�F
					for (Player* player : playerManager->GetPlayers())
					{
						if (player->GetPlayerID() == plInfo.id)
						{
							// �o�^�σt���O�𗧂Ă�
							inserted = true;
						}
					}
					// �o�^����Ă��Ȃ��ꍇ�A�N���C�A���g����Ƀ��O�C�����Ă���
					// �v���C���[�̃f�[�^�̂��ߐV�K�o�^���s���B
					if (!inserted)
					{
						// �V�K�o�^
						Player* player = debug_new Player();
						player->SetPlayerID(plInfo.id);
						player->SetPosition(plInfo.position);
						player->SetAngle(plInfo.angle);
						player->SetScale(DirectX::XMFLOAT3(0.02f, 0.02f, 0.02f));
						player->SetModel();
						player->GetJobClass()->ChangeState(plInfo.state);
						player->SetReady(true);

						playerManager->AddPlayer(player);
					}
					else
					{
						// ���łɓo�^����Ă���ꍇ�A�f�[�^�X�V
						Player* player = playerManager->GetPlayer(plInfo.id);
						player->SetPosition(plInfo.position);
						player->SetAngle(plInfo.angle);
						player->SetScale(DirectX::XMFLOAT3(0.02f, 0.02f, 0.02f));
						player->GetJobClass()->ChangeState(plInfo.state);
					}
					Logger::Print("Synchronize Data id:%d\n", std::to_string(plInfo.id));

					break;
				}
				case NetworkTag::Login:
				{
					// Login�^�O�̏ꍇ
					// ��M�f�[�^��Login�ɃR�s�[����
					PlayerLogin login{};
					memcpy_s(&login, sizeof(PlayerLogin), buffer, sizeof(PlayerLogin));

					// �v���C���[�V�K�o�^
					Player* player = debug_new Player();
					player->SetPlayerID(login.id);
					player->SetPosition(DirectX::XMFLOAT3(0, 0, 0));
					player->SetScale(DirectX::XMFLOAT3(0.02f, 0.02f, 0.02f));
					player->SetAngle(DirectX::XMFLOAT3(0, 0, 0));
					player->SetModel();
					player->GetJobClass()->ChangeState(Player::State::Idle);
					player->SetReady(true);
					playerManager->AddPlayer(player);

					// ��M����ID�����񂾂��ݒ�
					if (playerManager->GetMyPlayerID() < 0)
					{
						playerManager->SetMyPlayerID(login.id);
						Logger::Print("My Player ID:%d\n", login.id);
					}
					break;
				}
				case NetworkTag::Logout:
				{
					// Logout�^�O�̏ꍇ
					// ��M�f�[�^��Logout�ɃR�s�[����
					PlayerLogout logout{};
					memcpy_s(&logout, sizeof(PlayerLogout), buffer, sizeof(PlayerLogout));
					// ���O�A�E�g�����v���C���[���폜�p�z��ɕۑ�
					// �폜���̂�Update�֐��̍Ō�Ɏ��s���Ă���
					playerManager->ErasePlayer(logout.id);
					Logger::Print("Delete Player ID:%d\n", logout.id);
					break;
				}
			}
		}
	} while (loop);
}

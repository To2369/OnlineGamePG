#include "03ServerAssignment.h"



// �T�[�o���R�}���h���̓X���b�h
void ServerAssignment03::Exit()
{
	while (loop) {
		std::string input;
		std::cin >> input;
		if (input == "exit")
		{
			loop = false;
		}
	}
}

void ServerAssignment03::Execute()
{
	// WinsockAPI��������
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		// ���������s
		std::cout << "WSA Initialize Failed." << std::endl;
		return;
	}

	// �T�[�o�̎�t�ݒ�
	struct sockaddr_in addr {};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(7000);
	addr.sin_addr.S_un.S_addr = INADDR_ANY;//"0.0.0.0"

	// �\�P�b�g�̍쐬
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == INVALID_SOCKET) {
		std::cout << "Create Socket Failed." << std::endl;
		// 9.WSA�̉��
		WSACleanup();
		return;
	}

	// �m���u���b�L���O
	u_long mode = 1;
	if (ioctlsocket(sock, FIONBIO, &mode) != 0)
	{
		std::cout << "Nonblocking Mode Failed." << std::endl;
		return;
	}

	// �\�P�b�g�Ǝ�t����R�Â���
	if (bind(sock, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) != 0)
	{
		std::cout << "Bind Failed." << std::endl;
		return;
	}

	std::cout << "Server Initialize OK." << std::endl;

	// �N���C�A���g����̎�t����
	int size = sizeof(struct sockaddr_in);

	// �T�[�o������R�}���h���͂ŏI�������܂Ń��[�v����B
	// �L�[�{�[�h��exit����͂���ƃ��[�v�𔲂��邽�߂̕ʃX���b�h��p��
	std::thread th(&ServerAssignment03::Exit, this);

	do {
		sockaddr_in temp{};
		char buffer[2048]{};
		int len = sizeof(sockaddr);
		int size = 0;
		// TODO 03_01
		// ��M�f�[�^��buffer�ɕۑ�
		// ���M�҂̏���temp�ɕۑ�


		if (size > 0)
		{
			// TODO 03_02
			// if(1)�̓_�~�[�R�[�h�̂��߁AHasSameData�֐����g�p���Ĕ��f����R�[�h�ɏC�����Ȃ���
			// HasSameData�̎d�l�͊֐���ǂݍ��ނ���
			if(1)
			{
				// �V�K�N���C�A���g�̏ꍇ
				// ��M�f�[�^����l�b�g���[�N�^�O���擾
				NetworkTag type;
				memcpy_s(&type, sizeof(short), buffer, sizeof(short));

				// �擾�����^�O�����Ƃɏ����𕪊�
				switch (type)
				{
					case NetworkTag::Login:
					{
						// Login�^�O�̏ꍇ

						// login�Ɏ�M�f�[�^���R�s�[����
						PlayerLogin login{};
						memcpy_s(&login, sizeof(PlayerLogin), buffer, sizeof(PlayerLogin));

						// ID������U�莟�̐ڑ��҂̂��߂�ID�����Z���Ă���
						login.id = giveID;
						++giveID;

						// �v���C���[�쐬
						Player player{};
						player.id = login.id;
						player.position = DirectX::XMFLOAT3(0, 0, 0);
						player.angle = DirectX::XMFLOAT3(0, 0, 0);
						player.state = Player::State::Idle;

						// �V�K�N���C�A���g��ǉ�
						Client* newClient = new Client;
						newClient->addr = temp;
						newClient->player = player;
						clients.emplace_back(newClient);
						std::cout << "new client insert." << std::endl;

						for (Client* client : clients)
						{
							// TODO 03_03
							// �S�N���C�A���g��login�ʒm


							PlayerInformation plInfo{};
							plInfo.cmd = NetworkTag::Sync;
							plInfo.id = client->player.id;
							plInfo.position = client->player.position;
							plInfo.angle = client->player.angle;
							plInfo.state = client->player.state;
							// TODO 03_04
							// plInfo�𑗐M�҂ɑ��M


						}

						break;
					}
					default:
					{
						break;
					}
				}
			}
			else
			{
				// �����̃N���C�A���g�̏ꍇ
				// ��M�f�[�^����l�b�g���[�N�^�O���擾
				NetworkTag type;
				memcpy_s(&type, sizeof(short), buffer, sizeof(short));

				switch (type)
				{
					case NetworkTag::Sync:
					{
						// Sync�^�O�̏ꍇ
						// ��M�f�[�^��plInfo�ɃR�s�[����
						PlayerInformation plInfo{};
						memcpy_s(&plInfo, sizeof(PlayerInformation), buffer, sizeof(PlayerInformation));

						// ��M�f�[�^��\��
						std::cout << "position :(" << plInfo.position.x << "," << plInfo.position.y << "," << plInfo.position.z << ")" << std::endl;
						std::cout << "angle :(" << plInfo.angle.x << "," << plInfo.angle.y << "," << plInfo.angle.z << ")" << std::endl;
						std::cout << "state : " << static_cast<int>(plInfo.state) << std::endl;

						// �����N���C�A���g�S���ɑ��M
						for (Client* client : clients)
						{
							// �v���C���[�̃T�[�o���X�V
							if (client->player.id == plInfo.id)
							{
								client->player.position = plInfo.position;
								client->player.angle = plInfo.angle;
								client->player.state = plInfo.state;
							}
							else
							{
								// TODO 03_05
								// �v���C���[���𑗐M����


								if (size < 0)
								{
									std::cout << "sendto failed. error code : " << WSAGetLastError() << std::endl;
								}
							}
						}
						break;
					}
					case NetworkTag::Logout:
					{
						// ���O�A�E�g�f�[�^�̏ꍇ
						// ��M�f�[�^��logoput�ɃR�s�[����
						PlayerLogout logout{};
						memcpy_s(&logout, sizeof(PlayerLogout), buffer, sizeof(PlayerLogout));

						for (auto it = clients.begin(); it != clients.end();)
						{
							// ���M�҂�id�������Ƃ�
							if ((*it)->player.id == logout.id) {
								// �f�[�^���폜
								it = clients.erase(it);
							}
							else {
								// TODO 03_06
								// logout�f�[�^�𑗐M�҈ȊO�ɑ��M


								++it; // ���̗v�f�ɐi��
							}
						}
						break;
					}
				}
			}
		}
		else
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				std::cout << "recvfrom failed. error code : " << WSAGetLastError() << std::endl;
			}
		}
	} while (loop);
	th.join();


	// �N���C�A���g�I������
	for (Client* client : clients)
	{
		if (client != nullptr)
		{
			delete client;
			client = nullptr;
		}
	}

	// �T�[�o�\�P�b�g�̐ؒf
	if (closesocket(sock) != 0) {
		int err = WSAGetLastError();
		std::cout << "Close Socket Failed.error_code:" << err << "." << std::endl;
	}

	// WSA�I��
	if (WSACleanup() != 0)
	{
		std::cout << "Cleanup WinsockAPI Failed." << std::endl;

	}
	std::cout << "Cleanup WinsockAPI Success." << std::endl;
}

bool ServerAssignment03::HasSameData(const std::vector<Client*>& vec, const sockaddr_in& target)
{
	for (const Client* client : vec)
	{
		if (client->addr.sin_addr.S_un.S_addr == target.sin_addr.S_un.S_addr &&
			client->addr.sin_port == target.sin_port)
		{
			return true;
		}
	}
	return false;
}
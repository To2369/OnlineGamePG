#include "05ServerAssignment.h"



// �T�[�o���R�}���h���̓X���b�h
void ServerAssignment05::Exit()
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

void ServerAssignment05::Execute()
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
		// WSA�̉��
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
	std::thread th(&ServerAssignment05::Exit, this);

	do {
		sockaddr_in temp{};
		char buffer[2048]{};
		int len = sizeof(sockaddr);
		int size = recvfrom(sock, reinterpret_cast<char*>(&buffer), sizeof(buffer), 0, reinterpret_cast<sockaddr*>(&temp), &len);
		if (size > 0)
		{
			// �u���[�h�L���X�g�ʐM�Ŏ�M�����f�[�^���`�F�b�N
			NetworkTag type;
			memcpy_s(&type, sizeof(short), buffer, sizeof(short));
			switch (type)
			{
				case NetworkTag::Search:
				{
					// search�R�}���h�Ȃ�e�L�X�g��Ԃ��B
					ServerSearch search;
					search.cmd = NetworkTag::Search;
					char message[] = { "Search Hit Server Online Game.\n" };
					strcpy_s(search.messasge, sizeof(message), message);
					std::cout << "Clients Search Access." << std::endl;
					sendto(sock, reinterpret_cast<char*>(&search), sizeof(search), 0, reinterpret_cast<sockaddr*>(&temp), len);
					break;
				}
			}
		}
	} while (loop);
	th.join();

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

#include "04ServerAssignment.h"



// �T�[�o���R�}���h���̓X���b�h
void ServerAssignment04::Exit()
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

void ServerAssignment04::Execute()
{
	// WinsockAPI��������
	WSADATA wsaData;
	// �o�[�W�������w�肷��ꍇMAKEWORD�}�N���֐����g�p����
	int wsaStartUp = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (wsaStartUp != 0)
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
	int m = ioctlsocket(sock, FIONBIO, &mode);
	if (m != 0)
	{
		std::cout << "Nonblocking Mode Failed." << std::endl;
		return;
	}

	// �\�P�b�g�Ǝ�t����R�Â���
	int r = bind(sock, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));
	if (r != 0)
	{
		std::cout << "Bind Failed." << std::endl;
		return;
	}

	std::cout << "Server Initialize OK." << std::endl;

	// �N���C�A���g����̎�t����
	int size = sizeof(struct sockaddr_in);

	// �T�[�o������R�}���h���͂ŏI�������܂Ń��[�v����B
	// �L�[�{�[�h��exit����͂���ƃ��[�v�𔲂��邽�߂̕ʃX���b�h��p��
	std::thread th(&ServerAssignment04::Exit, this);

	do {
		sockaddr_in temp{};
		char buffer[2048]{};
		int len = sizeof(sockaddr);
		int size = recvfrom(sock, reinterpret_cast<char*>(&buffer), sizeof(buffer), 0, reinterpret_cast<sockaddr*>(&temp), &len);
		if (size > 0)
		{
			NetworkTag type;
			memcpy_s(&type, sizeof(short), buffer, sizeof(short));
			switch (type)
			{
				case NetworkTag::Message:
				{
					PlayerMessage message;
					memcpy_s(&message, sizeof(message), buffer, sizeof(message));
					std::cout << "cmd:" << static_cast<int>(message.cmd) << std::endl;
					std::cout << "buffer:" << message.buffer << std::endl;

					size = sendto(sock, reinterpret_cast<char*>(&message), sizeof(message), 0,
						reinterpret_cast<sockaddr*>(&temp), static_cast<int>(sizeof(message)));
					break;
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

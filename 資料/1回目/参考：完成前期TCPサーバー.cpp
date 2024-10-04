#include "05ServerAssignment.h"



//----------------------------------------------------------------------------------------
// �T�[�o���I���R�}���h���̓X���b�h
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

//----------------------------------------------------------------------------------------
//���s����
void ServerAssignment05::Execute()
{
	// Server�ۑ� �T�[�o���ݒ�
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
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(7000);
	addr.sin_addr.S_un.S_addr = INADDR_ANY;//"0.0.0.0"

	// �\�P�b�g�̍쐬
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		std::cout << "Create Socket Failed." << std::endl;
		// 9.WSA�̉��
		WSACleanup();
		return;
	}

	// �\�P�b�g�Ǝ�t����R�Â���
	int r = bind(sock, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));
	if (r != 0)
	{
		std::cout << "Bind Failed." << std::endl;
		return;
	}
	// ��t���J�n����
	r = listen(sock, 10);
	if (r != 0)
	{
		std::cout << "Listen Failed." << std::endl;
		return;
	}
	std::cout << "Server Initialize OK." << std::endl;

	// �N���C�A���g����̎�t����
	int size = sizeof(struct sockaddr_in);

	// �ڑ��v������������
	client.sock = accept(sock, reinterpret_cast<struct sockaddr*>(&client.addr), &size);
	if (client.sock == INVALID_SOCKET)
	{
		// �ڑ����s
		std::cout << "Connect Failed." << std::endl;
		return;
	}
	else
	{
		// �ڑ��ʒm
		std::cout << "Connect Success:IPAddress(";
		std::cout << static_cast<int>(client.addr.sin_addr.S_un.S_un_b.s_b1) << ",";
		std::cout << static_cast<int>(client.addr.sin_addr.S_un.S_un_b.s_b2) << ",";
		std::cout << static_cast<int>(client.addr.sin_addr.S_un.S_un_b.s_b3) << ",";
		std::cout << static_cast<int>(client.addr.sin_addr.S_un.S_un_b.s_b4) << ")." << std::endl;
	}

	// �T�[�o������R�}���h���͂ŏI�������܂Ń��[�v����B
	// �L�[�{�[�h��exit����͂���ƃ��[�v�𔲂��邽�߂̕ʃX���b�h��p��
	std::thread th(&ServerAssignment05::Exit, this);

	do {

		//��M������Ԃ�
		char buffer[256]{};
		int r = recv(client.sock, buffer, sizeof(buffer), 0);	//��M
		if (r > 0)
		{//������M�����̂�
			std::cout << "recv : " << buffer << std::endl;
			int s = send(client.sock, buffer, sizeof(buffer), 0); //����Ԃ�
			std::cout << "send : " << buffer << std::endl;
		}

	} while (loop);
	th.join();

	// �N���C�A���g�̃\�P�b�g�ؒf
	r = closesocket(client.sock);
	if (r != 0) {
		int err = WSAGetLastError();
		std::cout << "Close Socket Failed.error_code:" << err << "." << std::endl;
	}
	// ��t�\�P�b�g�̐ؒf
	r = closesocket(sock);
	if (r != 0) {
		int err = WSAGetLastError();
		std::cout << "Close Socket Failed.error_code:" << err << "." << std::endl;
	}

	// WSA�I��
	int wsaCleanup = WSACleanup();
	if (wsaCleanup != 0)
	{
		std::cout << "Cleanup WinsockAPI Failed." << std::endl;

	}
	std::cout << "Cleanup WinsockAPI Success." << std::endl;
}
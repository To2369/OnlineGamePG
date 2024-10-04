#include "06ServerAssignment.h"



// �T�[�o���R�}���h���̓X���b�h
void ServerAssignment06::Exit()
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

void ServerAssignment06::Execute()
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
	struct sockaddr_in addr;
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

	// TODO 06_01
	// �}���`�L���X�g�\�P�b�g�쐬(�N���C�A���g�֑��M�p)
	SOCKET multicastSock;
	struct sockaddr_in multicastAddr;
	

	// �}���`�L���X�gIP�A�h���X�w��
	// inetpton IP�A�h���X�̕�����𐔒l�^�֕ϊ�����
	// �����ł̓}���`�L���X�g�A�h���X���w�肷��
	// �}���`�L���X�g�A�h���X�̎��
	//  ���                �A�h���X�͈�                  ����
	// �����N���[�J��       224.0.0.0 �` 224.0.0.255      ����Z�O�����g��Ŏg�p����A�h���X�B���͈̔͂̃A�h���X��
	//                                                    �ʏ�ATTL�l�u1�v�ő��M����邽�߁A���[�^�œ]������Ȃ��B
	//                                                    �܂��\�񂳂�Ă���A�h���X�ł�����B
	// �O���[�o���X�R�[�v	224.0.1.0 �` 238.255.255.255  ��Ƃ��A�g�D�Ԃ�C���^�[�l�b�g�Ŏg�p���邽�߂̃A�h���X�B
	//                                                    �A�v���P�[�V�����́A�A�h���X�̈ꕔ��ICANN�ŗ\�񂵂Ďg�p�B
	// �v���C�x�[�g�X�R�[�v	239.0.0.0 �` 239.255.255.255  ��Ƃ��A�g�D���Ŏg�p���邽�߂̃A�h���X�B
	//                                                    �u239.0.0.0 / 24�v�Ɓu239.128.0.0 / 24�v�͎g�p���Ȃ����������B

	// TODO 06_02
	// inet_pton�c�W���e�L�X�g�\���`���̃l�b�g���[�N�A�h���X�𐔒l�o�C�i���`���ɕϊ�


	// TODO 06_03
	// ���[�J��IP�ݒ�(��2�����Ɏw��)
	ULONG localAddress = 0;
	// ���[�J��IP�͊e���̒[���p�ɕς��Ă�������
	

	// TODO 06_04
	// �}���`�L���X�g�I�v�V�����̐ݒ�(���M��)
	// ��1����,socket
	// ��2����,option�̎��
	// ��3����,���[�J��IP�A�h���X�̎w��
	// ��4����,���[�J��IP�A�h���X�̃T�C�Y


	std::cout << "Server Initialize OK." << std::endl;

	// �N���C�A���g����̎�t����
	int size = sizeof(struct sockaddr_in);

	// �T�[�o������R�}���h���͂ŏI�������܂Ń��[�v����B
	// �L�[�{�[�h��exit����͂���ƃ��[�v�𔲂��邽�߂̕ʃX���b�h��p��
	std::thread th(&ServerAssignment06::Exit, this);

	do {
		char buffer[1024];
		int len = sizeof(sockaddr);
		int size = recvfrom(sock, buffer, sizeof(buffer), 0, reinterpret_cast<sockaddr*>(&client), &len);
		if (size > 0)
		{
			std::cout << buffer << std::endl;
			// TODO 06_05
			// �}���`�L���X�g�A�h���X�őS���ɑ��M
		}
	} while (loop);
	th.join();

	// �T�[�o�\�P�b�g�ؒf
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
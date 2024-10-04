#include "02ServerAssignment.h"



// �T�[�o���R�}���h���̓X���b�h
void ServerAssignment02::Exit()
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

void ServerAssignment02::Execute()
{
	// TODO 02_01
	// WinsockAPI��������
	WSADATA wsaData;


	// TODO 02_02
	// �T�[�o�̎�t�ݒ�


	// TODO 02_03
	// �\�P�b�g�̍쐬
	SOCKET sock;


	// TODO 02_04
	// �m���u���b�L���O
	u_long mode = 1;


	// TODO 02_05
	// �\�P�b�g�Ǝ�t����R�Â���


	std::cout << "Server Initialize OK." << std::endl;

	// �N���C�A���g����̎�t����
	int size = sizeof(struct sockaddr_in);

	// �T�[�o������R�}���h���͂ŏI�������܂Ń��[�v����B
	// �L�[�{�[�h��exit����͂���ƃ��[�v�𔲂��邽�߂̕ʃX���b�h��p��
	std::thread th(&ServerAssignment02::Exit, this);

	do {
		// �v���C���[���\����
		PlayerInformation plInfo{};
		int len = sizeof(sockaddr);
		int size = 0;
		// TODO 02_06
		// �v���C���[���f�[�^��M�B��M�f�[�^��plInfo�ɕۑ����邱��


		if (size > 0)
		{
			// �v���C���[���\��
			std::cout << "position :(" << plInfo.position.x << "," << plInfo.position.y << "," << plInfo.position.z << ")" << std::endl;
			std::cout << "angle :(" << plInfo.angle.x << "," << plInfo.angle.y << "," << plInfo.angle.z << ")" << std::endl;
			std::cout << "state : " << static_cast<int>(plInfo.state) << std::endl;

			// TODO 02_07 plInfo���N���C�A���g�ɑ��M


			if (size < 0)
			{
				std::cout << "sendto failed. error code : " << WSAGetLastError() << std::endl;
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

	// TODO 02_08
	// �T�[�o�\�P�b�g�̐ؒf


	// TODO 02_09
	// WSA�I��


	std::cout << "Cleanup WinsockAPI Success." << std::endl;
}
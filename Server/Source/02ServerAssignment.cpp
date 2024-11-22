#include "02ServerAssignment.h"


//---------------�萔---------------------------
#define	NONBLOCKING_MODE 1
#define	BUFFER_SIZE	1024
#define PORT 7000

//------------------------------------------------------------------------------
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

//-----���s����----------------------------------------------------------------
void ServerAssignment02::Execute()
{

	WSADATA wsaData;

	// TODO 01_01 �ۑ�쐬���鎖
	// WinsockAPI��������
		//(WSAStartup�֐�)
	int wsaStartUp = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (wsaStartUp != 0)
	{
		// ���������s
		std::cout << "WSA Initialize Failed." << std::endl;
		return;
	}

	// TODO 01_02 �ۑ�쐬���鎖
	// �T�[�o�̎�t�ݒ�
	struct sockaddr_in addr;
		//[addr.sin_family]�̐ݒ�
		//[addr.sin_port]�̐ݒ�
		//[addr.sin_addr.S_un.S_addr]�̐ݒ�

	addr.sin_family = AF_INET;
	addr.sin_port = htons(7000);
	addr.sin_addr.S_un.S_addr = INADDR_ANY;

	// TODO 01_03 �ۑ�쐬���鎖
	// �\�P�b�g�̍쐬
	SOCKET sock;		
		//socket�֐��Ń\�P�b�g�쐬�F[�C���^�[�l�b�g�ڑ�]�A[UDP�ڑ�] �ݒ�
		//�����ł��Ȃ���΃G���[�\�����āAWSACleanup()���I��
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == INVALID_SOCKET) {
		std::cout << "Create Socket Failed." << std::endl;
		// 9.WSA�̉��
		WSACleanup();
		return;
	}
	// TODO 01_04 �ۑ�쐬���鎖
	// �m���u���b�L���O�ݒ�
	u_long mode = NONBLOCKING_MODE;
		//(ioctlsocket�֐��ł̃m���u���b�N�w��)
	int res = ioctlsocket(sock, FIONBIO, &mode);//�~�܂�Ȃ� 
	if (res != 0) 
	{
		std::cout << "1_4 ERR" << std::endl;
		return;
	}
	// TODO 01_05 �ۑ�쐬���鎖
	// �\�P�b�g�Ǝ�t����R�Â���
		//(bind�֐�)
	res = bind(sock, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));
	std::cout << "Server Initialize OK." << std::endl;

	// �N���C�A���g����̎�t����
	int size = sizeof(struct sockaddr_in);

//���������܂ł͉ۑ�1�̃��\�b�h���ƃR�s�[���y�[�X�g��OK�ł�����

	// �T�[�o������R�}���h���͂ŏI�������܂Ń��[�v����B
	// �L�[�{�[�h��exit����͂���ƃ��[�v�𔲂��邽�߂̕ʃX���b�h�𗧂��グ��
	std::thread th(&ServerAssignment02::Exit, this);

	PlayerInformation plInfoOld;//�\���ۑ���\���p�@�X�g�b�N���Ă����ĐV�����f�[�^�Ɣ�r
	ZeroMemory(&plInfoOld, sizeof(PlayerInformation));

	do {
		// �v���C���[���\����
		PlayerInformation plInfo{};
		int len = sizeof(sockaddr);
		int size = 0;
		//����V�K�쐬����S01
		// TODO 02_01
		// �\�P�b�gsock����v���C���[���f�[�^��M(recvfrom�֐�)
		// recvfrom�̎�M�f�[�^�ۑ��ꏊ��plInfo
		size = recvfrom(sock, reinterpret_cast<char*>(&plInfo), sizeof(plInfo), 0,
			reinterpret_cast<sockaddr*>(&client), &len);


		if (size > 0)	//�f�[�^��M�����I
		{
			//�\���ۑ�02_s
			//position(x,y,z)/angle/state)��plInfoOld�̃f�[�^�Ɣ�r�A
			//	�S�ē����Ȃ牺�ɂ���A�v���C���[���\�����s��Ȃ��悤�ɂ���B
			//��r�㎟��p�ɁAplInfo��plInfoOld�ɃX�g�b�N
			if (plInfo.position.x != plInfoOld.position.x ||
				plInfo.position.y != plInfoOld.position.y ||
				plInfo.position.z != plInfoOld.position.z ||
				plInfo.angle.x != plInfoOld.angle.x ||
				plInfo.angle.y != plInfoOld.angle.y ||
				plInfo.angle.z != plInfoOld.angle.z ||
				plInfo.state != plInfoOld.state)
			{
				// �v���C���[���\��
				std::cout << "position :(" << plInfo.position.x << "," << plInfo.position.y << "," << plInfo.position.z << ")" << std::endl;
				std::cout << "angle :(" << plInfo.angle.x << "," << plInfo.angle.y << "," << plInfo.angle.z << ")" << std::endl;
				std::cout << "state : " << static_cast<int>(plInfo.state) << std::endl;
			}
			plInfoOld = plInfo;
			//����V�K�쐬����S02
			// TODO 02_02 plInfo���N���C�A���g�ɑ��M(����Ԃ�)
			//sendoto�̖߂�l��size�ɑ���i�����G���[���f���Ă��܂��B�j
			size = sendto(sock, reinterpret_cast<char*>(&plInfo), sizeof(plInfo), 0,
				reinterpret_cast<sockaddr*>(&client), static_cast<int>(sizeof(sockaddr_in)));





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
	th.join();			//�R�}���h���̓X���b�h�I����҂�

	//�������牺���A�O��̃R�s�[���y�[�X�g��OK
	// TODO 01_08
	// �T�[�o�\�P�b�g�̐ؒf
	res = closesocket(sock);
	if (res <= 0) {//ERR 
		//�\�P�b�g���� 
		std::cout << "1_8 ERR" << std::endl;
	}

	// TODO 01_09
	// WSA�I��
	res = WSACleanup();
	if (res != 0) {
		std::cout << "1_9ERR" << std::endl;
	}

	std::cout << "Cleanup WinsockAPI Success." << std::endl;
}
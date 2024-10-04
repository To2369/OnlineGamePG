#include "02ClientAssignment.h"



ClientAssignment02::ClientAssignment02()
{
	input[0] = '\0';
}

ClientAssignment02::~ClientAssignment02()
{
	
}

void ClientAssignment02::Initialize()
{
	SceneBase::Initialize();
	// TODO 02_10
	// WinsockAPI��������


	addrinfo hints;				// DNS�֖₢���킹�ɕK�v�ȏ���ݒ�
	addrinfo* addrInfo = NULL;	// �擾�����A�h���X�������ɕۑ������
	// �[���N���A
	ZeroMemory(&hints, sizeof(addrinfo));
	// TODO 01_11
	// DNS�₢���킹�Ɋւ������ݒ�

	const char hostname[] = "";	// �h���C���w��
	const char port[] = "";		// �|�[�g�ԍ��w��
	// DNS�֖₢���킹


	// TODO 02_12
	// WinsockAPI��������
	// �擾����IP�A�h���X��sockaddr_in�ɕϊ���sin_addr����


	// TODO 02_13
	// �\�P�b�g�쐬

	// TODO 02_14
	// �m���u���b�L���O�ݒ�
	u_long mode = 1;


	// TODO 02_15
	// �N���C�A���g�����T�[�o�֒m�点�邽�߂�sendto�֐����ŏ��Ɏ��s����B


	Logger::Print("Connect Success\n");

	// Player�ݒ�
	playerManager = new PlayerManager();
	playerManager->SetMyPlayerID(0);

	Player* player = new Player();

	// �e�v���C���[�̃L�����N�^�[�ɍ��킹���ݒ���s��
	// JobClass�̓X�e�[�g�}�V��
	player->SetPlayerID(0);
	player->SetPosition(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	player->SetScale(DirectX::XMFLOAT3(0.02f, 0.02f, 0.02f));
	player->SetState(Player::State::Idle);
	player->SetModel();

	playerManager->AddPlayer(player);
	playerManager->GetMyPlayer()->SetReady(true);

	// ��M�X���b�h����
	th = std::thread(&ClientAssignment02::recvThread, this);

}

void ClientAssignment02::Finalize()
{
	// �}���`�X���b�h�̃��[�v�t���O�����낷
	loop = false;

	// �X���b�h�̏I���܂őҋ@
	th.join();

	// TODO 01_18
	// �\�P�b�g�I��


	// TODO 01_19
	// WSA�I������


}

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
	// ���M�f�[�^
	plInfo.position = player->GetPosition();
	plInfo.angle = player->GetAngle();
	plInfo.state = player->GetState();

	int size = 0;
	// TODO 02_16
	// plInfo���T�[�o�ɑ��M����


}

void ClientAssignment02::recvThread()
{
	int len = sizeof(sockaddr_in);
	do {
		// TODO 02_17
		// �v���C���[������M
		// ��M�f�[�^��plInfo�ɕۑ�
		PlayerInformation plInfo{};


	} while (loop);
}
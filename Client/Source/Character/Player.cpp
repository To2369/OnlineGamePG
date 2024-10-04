#include "Character/Player.h"
#include "Graphics/Graphics.h"
#include "Camera/Camera.h"
#include "Collision/Collision.h"
#include "Stage/Stage.h"
#include "Input/Input.h"
#include "Character/JobClass.h"

// ���������[�N�����p
#define debug_new new(_NORMAL_BLOCK,__FILE__,__LINE__)


Player::Player()
{
	playerID = 0;
	radius = 2.5f;
	height = 3.8f;
	position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	angle = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	jobType = Player::JobType::ArcherF;
	jobClass = new ArcherClass(this);
	ready = false;
	scale.x = 0.02f;
	scale.y = 0.02f;
	scale.z = 0.02f;
	state = Player::State::Idle;
}

Player::Player(int id)
{
	playerID = id;
	radius = 2.5f;
	height = 3.8f;
	position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	angle = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	jobType = Player::JobType::ArcherF;
	jobClass = new ArcherClass(this);
	ready = false;
	scale.x = 0.02f;
	scale.y = 0.02f;
	scale.z = 0.02f;
	state = Player::State::Idle;
}
Player::~Player()
{
	if (model != nullptr)
	{
		delete model;
		model = nullptr;
	}
	if (jobClass != nullptr)
	{
		delete jobClass;
		jobClass = nullptr;
	}
}
void Player::Update(float elapsedTime)
{
	// ��M�����f�[�^�ɍ��킹�čX�V
	jobClass->Update(elapsedTime);

	// �I�u�W�F�N�g�s����X�V
	UpdateTransform();

	// �A�j���[�V�����X�V
	model->UpdateAnimation(elapsedTime);

	// ���f���s��X�V
	model->UpdateTransform(transform);
}

void Player::Render(ID3D11DeviceContext* context, Shader* shader)
{
	shader->Draw(context, model);
}


void Player::SetJobType(JobType jobType)
{
	this->jobType = jobType;
}

void Player::SetModel()
{
	if (jobType == Player::JobType::End)return;
	if (model != nullptr)
	{
		delete model;
		model = nullptr;
	}
	switch (this->jobType)
	{
	case JobType::ArcherF:
		model = debug_new Model("Data/Model/Character/ArcherGirl.mdl");
		break;
	case JobType::ArcherM:
		model = debug_new Model("Data/Model/Character/ArcherBoy.mdl");
		break;
	case JobType::MageF:
		model = debug_new Model("Data/Model/Character/MageGirl.mdl");
		break;
	case JobType::MageM:
		model = debug_new Model("Data/Model/Character/MageBoy.mdl");
		break;
	case JobType::RogueF:
		model = debug_new Model("Data/Model/Character/RogueGirl.mdl");
		break;
	case JobType::RogueM:
		model = debug_new Model("Data/Model/Character/RogueBoy.mdl");
		break;
	case JobType::TankF:
		model = debug_new Model("Data/Model/Character/TankGirl.mdl");
		break;
	case JobType::TankM:
		model = debug_new Model("Data/Model/Character/TankBoy.mdl");
		break;
	default:
		model = debug_new Model("Data/Model/Character/ArcherGirl.mdl");
		break;
	}
}

DirectX::XMFLOAT3 Player::GetMoveVec() const
{
	// ���͏����擾
	GamePad& gamePad = Input::Instance().GetGamePad();
	float ax = gamePad.GetAxisLX();
	float ay = gamePad.GetAxisLY();

	// �J���������ƃX�e�B�b�N�̓��͒l�ɂ���Đi�s�������v�Z����
	Camera& camera = Camera::Instance();
	const DirectX::XMFLOAT3& cameraRight = camera.GetRight();
	const DirectX::XMFLOAT3& cameraFront = camera.GetFront();

	// �ړ��x�N�g����XZ���ʂɐ����ȃx�N�g���Ȃ�悤�ɂ���

	// �J�����E�����x�N�g����XZ�P�ʃx�N�g���ɕϊ�
	float cameraRightX = cameraRight.x;
	float cameraRightZ = cameraRight.z;
	float cameraRightLength = sqrtf(cameraRightX * cameraRightX + cameraRightZ * cameraRightZ);
	if (cameraRightLength > 0.0f)
	{
		// �P�ʃx�N�g����
		cameraRightX /= cameraRightLength;
		cameraRightZ /= cameraRightLength;
	}

	// �J�����O�����x�N�g����XZ�P�ʃx�N�g���ɕϊ�
	float cameraFrontX = cameraFront.x;
	float cameraFrontZ = cameraFront.z;
	float cameraFrontLength = sqrtf(cameraFrontX * cameraFrontX + cameraFrontZ * cameraFrontZ);
	if (cameraFrontLength > 0.0f)
	{
		// �P�ʃx�N�g����
		cameraFrontX /= cameraFrontLength;
		cameraFrontZ /= cameraFrontLength;
	}

	// �X�e�B�b�N�̐������͒l���J�����E�����ɔ��f���A
	// �X�e�B�b�N�̐������͒l���J�����O�����ɔ��f���A
	// �i�s�x�N�g�����v�Z����
	DirectX::XMFLOAT3 vec;
	vec.x = (cameraRightX * ax) + (cameraFrontX * ay);
	vec.z = (cameraRightZ * ax) + (cameraFrontZ * ay);
	// Y�������ɂ͈ړ����Ȃ�
	vec.y = 0.0f;

	return vec;

}

bool Player::InputMove(float elapsedTime)
{
	// �i�s�x�N�g���擾
	DirectX::XMFLOAT3 moveVec = GetMoveVec();

	// �ړ�����
	Move(moveVec.x, moveVec.z, moveSpeed * elapsedTime);

	// ���񏈗�
	Turn(elapsedTime, moveVec.x, moveVec.z, turnSpeed);

	// �i�s�x�N�g�����[���x�N�g���łȂ��ꍇ�͓��͂��ꂽ
	return moveVec.x != 0.0f || moveVec.y != 0.0f || moveVec.z != 0.0f;
}

bool Player::InputAttack()
{
	GamePad& gamePad = Input::Instance().GetGamePad();

	if (gamePad.GetButtonDown() & GamePad::BTN_B)
	{
		return true;
	}

	return false;
}

Player* PlayerManager::GetPlayer(int id)
{
	for (Player* player : players)
	{
		if (player->GetPlayerID() == id)
		{
			return player;
		}
	}
	return nullptr;
}

Player* PlayerManager::GetMyPlayer()
{
	for (Player* player : players)
	{
		if (player->GetPlayerID() == myPlayerId)
		{
			return player;
		}
	}
	return nullptr;
}
void PlayerManager::ErasePlayer(int id)
{
	int i = 0;
	for (Player* player : players)
	{
		if (player->GetPlayerID() == id)
		{
			player->SetReady(false);
			deleteList.emplace_back(player);
			break;
		}
		++i;
	}
}

void PlayerManager::DeletePlayer()
{
	for (Player* player : deleteList)
	{
		// std::vector����v�f���폜����ꍇ�̓C�e���[�^�[�ō폜���Ȃ���΂Ȃ�Ȃ�
		std::vector<Player*>::iterator it = std::find(players.begin(), players.end(), player);
		if (it != players.end())
		{
			players.erase(it);
		}
		delete player;
	}
	deleteList.clear();
}

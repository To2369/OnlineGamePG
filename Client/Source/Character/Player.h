#pragma once
#include "Character/Actor.h"
const int MAX_CLIENT = 4;
class JobClass;

class Player:public Actor
{
public:
	enum class State
	{
		Idle,
		Move,
		Attack,
		Skill,
		End
	};
	enum class JobType
	{
		ArcherF = 0,
		ArcherM,
		MageF,
		MageM,
		RogueF,
		RogueM,
		TankF,
		TankM,
		End
	};
	enum Information
	{

	};
	// �R���X�g���N�^
	Player();
	Player(int id);

	// �f�X�g���N�^
	~Player();

	// �X�V
	void Update(float elapsedTime);

	// �`��
	void Render(ID3D11DeviceContext* context, Shader* shader);

	// ID
	short GetPlayerID() const{ return playerID; }
	void SetPlayerID(int id) { playerID = id; }

	// �X�e�[�g
	Player::State GetState() { return state; }
	void SetState(Player::State state) { this->state = state; }
	// �W���u�w��
	JobType GetJobType() const { return jobType; }
	void SetJobType(JobType jobType);

	// Job�X�e�[�g�}�V��
	JobClass* GetJobClass() { return jobClass; }
	void SetJobClass(JobClass* jobClass) { this->jobClass = jobClass; }

	// �����t���O
	bool GetReady() const { return ready; }
	void SetReady(bool flg) { ready = flg; }

	// ���f���ǂݍ���
	void SetModel();

	// �X�e�B�b�N���͒l����ړ��x�N�g�����擾
	DirectX::XMFLOAT3 GetMoveVec() const;
	// �ړ����͏���
	bool InputMove(float elapsedTime);
	// �U�����͏���
	bool InputAttack();

	// �V�[�P���X�ԍ��擾
	int GetSequenceNum() const { return sequenceNum; }
	// �V�[�P���X�ԍ��ݒ�
	void SetSequenceNum(int sequenceNum) { this->sequenceNum = sequenceNum; }

protected:
	short playerID;
	JobType jobType = JobType::ArcherF;
	JobClass* jobClass = nullptr;
	Player::State state;
	bool ready = false;
	int sequenceNum = 0;

};

class PlayerManager
{
private:
	std::vector<Player*> players;
	short myPlayerId = -1;
	std::vector<Player*> deleteList;
public:
	PlayerManager() {}
	~PlayerManager() {}

	static PlayerManager& Instance()
	{
		static PlayerManager instance;
		return instance;
	}

	void AddPlayer(Player* player) { players.emplace_back(player); };
	std::vector<Player*> GetPlayers() { return players; }
	Player* GetPlayer(int id);
	Player* GetMyPlayer();
	void SetMyPlayerID(int id) { myPlayerId = id; }
	short GetMyPlayerID() const { return myPlayerId; }
	void ErasePlayer(int id);
	void DeletePlayer();

};
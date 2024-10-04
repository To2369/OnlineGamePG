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
	// コンストラクタ
	Player();
	Player(int id);

	// デストラクタ
	~Player();

	// 更新
	void Update(float elapsedTime);

	// 描画
	void Render(ID3D11DeviceContext* context, Shader* shader);

	// ID
	short GetPlayerID() const{ return playerID; }
	void SetPlayerID(int id) { playerID = id; }

	// ステート
	Player::State GetState() { return state; }
	void SetState(Player::State state) { this->state = state; }
	// ジョブ指定
	JobType GetJobType() const { return jobType; }
	void SetJobType(JobType jobType);

	// Jobステートマシン
	JobClass* GetJobClass() { return jobClass; }
	void SetJobClass(JobClass* jobClass) { this->jobClass = jobClass; }

	// 準備フラグ
	bool GetReady() const { return ready; }
	void SetReady(bool flg) { ready = flg; }

	// モデル読み込み
	void SetModel();

	// スティック入力値から移動ベクトルを取得
	DirectX::XMFLOAT3 GetMoveVec() const;
	// 移動入力処理
	bool InputMove(float elapsedTime);
	// 攻撃入力処理
	bool InputAttack();

	// シーケンス番号取得
	int GetSequenceNum() const { return sequenceNum; }
	// シーケンス番号設定
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
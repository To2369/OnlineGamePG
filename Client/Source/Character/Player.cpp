#include "Character/Player.h"
#include "Graphics/Graphics.h"
#include "Camera/Camera.h"
#include "Collision/Collision.h"
#include "Stage/Stage.h"
#include "Input/Input.h"
#include "Character/JobClass.h"

// メモリリーク調査用
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
	// 受信したデータに合わせて更新
	jobClass->Update(elapsedTime);

	// オブジェクト行列を更新
	UpdateTransform();

	// アニメーション更新
	model->UpdateAnimation(elapsedTime);

	// モデル行列更新
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
	// 入力情報を取得
	GamePad& gamePad = Input::Instance().GetGamePad();
	float ax = gamePad.GetAxisLX();
	float ay = gamePad.GetAxisLY();

	// カメラ方向とスティックの入力値によって進行方向を計算する
	Camera& camera = Camera::Instance();
	const DirectX::XMFLOAT3& cameraRight = camera.GetRight();
	const DirectX::XMFLOAT3& cameraFront = camera.GetFront();

	// 移動ベクトルはXZ平面に水平なベクトルなるようにする

	// カメラ右方向ベクトルをXZ単位ベクトルに変換
	float cameraRightX = cameraRight.x;
	float cameraRightZ = cameraRight.z;
	float cameraRightLength = sqrtf(cameraRightX * cameraRightX + cameraRightZ * cameraRightZ);
	if (cameraRightLength > 0.0f)
	{
		// 単位ベクトル化
		cameraRightX /= cameraRightLength;
		cameraRightZ /= cameraRightLength;
	}

	// カメラ前方向ベクトルをXZ単位ベクトルに変換
	float cameraFrontX = cameraFront.x;
	float cameraFrontZ = cameraFront.z;
	float cameraFrontLength = sqrtf(cameraFrontX * cameraFrontX + cameraFrontZ * cameraFrontZ);
	if (cameraFrontLength > 0.0f)
	{
		// 単位ベクトル化
		cameraFrontX /= cameraFrontLength;
		cameraFrontZ /= cameraFrontLength;
	}

	// スティックの水平入力値をカメラ右方向に反映し、
	// スティックの垂直入力値をカメラ前方向に反映し、
	// 進行ベクトルを計算する
	DirectX::XMFLOAT3 vec;
	vec.x = (cameraRightX * ax) + (cameraFrontX * ay);
	vec.z = (cameraRightZ * ax) + (cameraFrontZ * ay);
	// Y軸方向には移動しない
	vec.y = 0.0f;

	return vec;

}

bool Player::InputMove(float elapsedTime)
{
	// 進行ベクトル取得
	DirectX::XMFLOAT3 moveVec = GetMoveVec();

	// 移動処理
	Move(moveVec.x, moveVec.z, moveSpeed * elapsedTime);

	// 旋回処理
	Turn(elapsedTime, moveVec.x, moveVec.z, turnSpeed);

	// 進行ベクトルがゼロベクトルでない場合は入力された
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
		// std::vectorから要素を削除する場合はイテレーターで削除しなければならない
		std::vector<Player*>::iterator it = std::find(players.begin(), players.end(), player);
		if (it != players.end())
		{
			players.erase(it);
		}
		delete player;
	}
	deleteList.clear();
}

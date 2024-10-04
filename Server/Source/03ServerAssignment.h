#pragma once
#include <Winsock2.h>
#include <iostream>
#include <thread>
#include <string>
#include <vector>
#include <DirectXMath.h>
#include "Player.h"
#include <NetworkTag.h>


class ServerAssignment03
{
public:
	struct Client
	{
		sockaddr_in addr;
		Player player;
	};

	void Execute();
	void Exit();

	// �o�^�ς̃N���C�A���g�����f���s��
	bool HasSameData(const std::vector<Client*>& vec, const sockaddr_in& target);

private:
	// �N���C�A���g���
	std::vector<struct Client*> clients;
	bool loop = true;
	int giveID = 0;
};
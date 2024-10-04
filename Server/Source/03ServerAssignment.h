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

	// 登録済のクライアントか判断を行う
	bool HasSameData(const std::vector<Client*>& vec, const sockaddr_in& target);

private:
	// クライアント情報
	std::vector<struct Client*> clients;
	bool loop = true;
	int giveID = 0;
};
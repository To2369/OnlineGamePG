#pragma once
#include <Winsock2.h>
#include <iostream>
#include <thread>
#include <string>
#include <vector>
#include <DirectXMath.h>
#include "Player.h"
#include <NetworkTag.h>

class ServerAssignment05
{
public:
	ServerAssignment05() = default;
	struct Client
	{
		sockaddr_in addr{};
		Player player;
	};
	void Execute();
	void Exit();

private:
	// クライアント情報
	std::vector<struct Client*> clients;


	bool loop = true;
	int giveID = 0;
};
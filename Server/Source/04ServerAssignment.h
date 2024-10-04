#pragma once
#include <Winsock2.h>
#include <iostream>
#include <thread>
#include <string>
#include <vector>
#include <DirectXMath.h>
#include "Player.h"
#include <NetworkTag.h>

class ServerAssignment04
{
public:
	ServerAssignment04() = default;
	struct Client
	{
		sockaddr_in addr{};
		Player player;
	};


	void Execute();
	void Exit();
private:
	bool loop = true;
	int giveID = 0;
};
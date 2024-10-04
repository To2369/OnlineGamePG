#pragma once
#include <Winsock2.h>
#include <iostream>
#include <thread>
#include <string>
#include <vector>
#include <DirectXMath.h>
#include "Player.h"
#include <NetworkTag.h>

class ServerAssignment02
{
public:
	void Execute();
	void Exit();
private:
	// クライアント情報
	struct sockaddr_in client {};
	bool loop = true;
};
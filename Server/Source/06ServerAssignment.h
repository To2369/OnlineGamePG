#pragma once
#include <Winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <thread>
#include <string>
#include <vector>

class ServerAssignment06
{
public:
	void Execute();
	void Exit();

private:
	// �N���C�A���g���
	struct sockaddr_in client {};
	bool loop = true;
};
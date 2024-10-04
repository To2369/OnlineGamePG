#pragma once
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <thread>
#include "Scene\SceneTitle.h"
#include "Graphics\Graphics.h"
#include "Input\Input.h"
#include <NetworkTag.h>


class ClientAssignment02 : public SceneBase
{
public:

	ClientAssignment02();
	~ClientAssignment02();

	void Initialize();
	void Finalize();

	void Render();
	void NetrowkUpdate(float elapsedTime);
	void recvThread();

private:
	std::vector<std::string> messages;
	char input[32];
	WSADATA wsaData{};
	struct sockaddr_in addr {};
	struct sockaddr_in recvAddr {};
	SOCKET sock{};
	std::thread th;
	bool loop = true;
};
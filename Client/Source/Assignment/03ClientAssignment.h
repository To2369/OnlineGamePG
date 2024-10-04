#pragma once
#include <thread>
#include "Scene\SceneTitle.h"
#include "Graphics\Graphics.h"
#include "Input\Input.h"
#include <NetworkTag.h>

class ClientAssignment03 : public SceneBase
{
public:
	ClientAssignment03();
	~ClientAssignment03();

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
	struct sockaddr_in self {};
	struct sockaddr_in recvAddr {};
	SOCKET sock{};
	std::thread recvTh;
	// std::thread sendTh;
	bool loop = true;
};
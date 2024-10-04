#pragma once
#include <thread>
#include "Scene\SceneTitle.h"
#include "Graphics\Graphics.h"
#include "Input\Input.h"
#include <NetworkTag.h>

class ClientAssignment05 : public SceneBase
{
public:
	ClientAssignment05();
	~ClientAssignment05();

	void Initialize();
	void Finalize();

	void Render();
	void NetrowkUpdate(float elapsedTime);
	void recvThread();
private:
	std::vector<std::string> messages;
	PlayerMessage input;
	WSADATA wsaData{};
	struct sockaddr_in addr {};
	struct sockaddr_in self {};
	struct sockaddr_in recvAddr {};
	SOCKET sock{};
	std::thread recvTh;
	bool loop = true;
};
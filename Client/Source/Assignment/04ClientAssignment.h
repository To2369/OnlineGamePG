#pragma once
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <thread>
#include "Scene\SceneTitle.h"
#include "Graphics\Graphics.h"
#include "Input\Input.h"
#include "04RingBuffer.h"
#include <NetworkTag.h>


class ClientAssignment04 : public SceneBase
{
public:
	ClientAssignment04();
	~ClientAssignment04();

	void Initialize();
	void Finalize();

	void Render();
	void recvThread();
private:

	RingBuffer<PlayerMessage> messages;
	PlayerMessage input;
	WSADATA wsaData{};
	struct sockaddr_in addr {};
	struct sockaddr_in self {};
	struct sockaddr_in recvAddr {};
	SOCKET sock{};
	std::thread recvTh;
	bool loop = true;
};
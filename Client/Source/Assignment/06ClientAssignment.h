#pragma once
#include <thread>
#include "Scene\SceneTitle.h"
#include "Graphics\Graphics.h"
#include "Input\Input.h"

class ClientAssignment06 : public SceneBase
{
public:
	ClientAssignment06();
	~ClientAssignment06();

	void Initialize();
	void Finalize();

	void Render();

	void recvThread();

private:
	std::vector<std::string> messages;
	char input[32];
	WSADATA wsaData{};
	struct sockaddr_in addr {};
	struct sockaddr_in self {};
	struct sockaddr_in recvAddr {};
	SOCKET sock{};
	std::thread th;
	bool loop = true;
	SOCKET multicastSock;
	struct sockaddr_in multicastAddress;
	struct ip_mreq mr;
};
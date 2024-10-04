#pragma once
#include <Winsock2.h>
#include <iostream>
#include <thread>
#include <string>
#include <vector>

class ServerAssignment01
{
public:
	void Execute();
	void Exit();

private:
	// クライアント情報
	struct sockaddr_in client {};
	bool loop = true;
};
#include "04ServerAssignment.h"



// サーバ側コマンド入力スレッド
void ServerAssignment04::Exit()
{
	while (loop) {
		std::string input;
		std::cin >> input;
		if (input == "exit")
		{
			loop = false;
		}
	}
}

void ServerAssignment04::Execute()
{
	// WinsockAPIを初期化
	WSADATA wsaData;
	// バージョンを指定する場合MAKEWORDマクロ関数を使用する
	int wsaStartUp = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (wsaStartUp != 0)
	{
		// 初期化失敗
		std::cout << "WSA Initialize Failed." << std::endl;
		return;
	}

	// サーバの受付設定
	struct sockaddr_in addr {};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(7000);
	addr.sin_addr.S_un.S_addr = INADDR_ANY;//"0.0.0.0"

	// ソケットの作成
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == INVALID_SOCKET) {
		std::cout << "Create Socket Failed." << std::endl;
		// 9.WSAの解放
		WSACleanup();
		return;
	}

	// ノンブロッキング
	u_long mode = 1;
	int m = ioctlsocket(sock, FIONBIO, &mode);
	if (m != 0)
	{
		std::cout << "Nonblocking Mode Failed." << std::endl;
		return;
	}

	// ソケットと受付情報を紐づける
	int r = bind(sock, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));
	if (r != 0)
	{
		std::cout << "Bind Failed." << std::endl;
		return;
	}

	std::cout << "Server Initialize OK." << std::endl;

	// クライアントからの受付処理
	int size = sizeof(struct sockaddr_in);

	// サーバ側からコマンド入力で終了されるまでループする。
	// キーボードでexitを入力するとループを抜けるための別スレッドを用意
	std::thread th(&ServerAssignment04::Exit, this);

	do {
		sockaddr_in temp{};
		char buffer[2048]{};
		int len = sizeof(sockaddr);
		int size = recvfrom(sock, reinterpret_cast<char*>(&buffer), sizeof(buffer), 0, reinterpret_cast<sockaddr*>(&temp), &len);
		if (size > 0)
		{
			NetworkTag type;
			memcpy_s(&type, sizeof(short), buffer, sizeof(short));
			switch (type)
			{
				case NetworkTag::Message:
				{
					PlayerMessage message;
					memcpy_s(&message, sizeof(message), buffer, sizeof(message));
					std::cout << "cmd:" << static_cast<int>(message.cmd) << std::endl;
					std::cout << "buffer:" << message.buffer << std::endl;

					size = sendto(sock, reinterpret_cast<char*>(&message), sizeof(message), 0,
						reinterpret_cast<sockaddr*>(&temp), static_cast<int>(sizeof(message)));
					break;
				}
			}
		}
		else
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				std::cout << "recvfrom failed. error code : " << WSAGetLastError() << std::endl;
			}
		}
	} while (loop);
	th.join();

	// サーバソケットの切断
	if (closesocket(sock) != 0) {
		int err = WSAGetLastError();
		std::cout << "Close Socket Failed.error_code:" << err << "." << std::endl;
	}

	// WSA終了
	if (WSACleanup() != 0)
	{
		std::cout << "Cleanup WinsockAPI Failed." << std::endl;

	}
	std::cout << "Cleanup WinsockAPI Success." << std::endl;
}

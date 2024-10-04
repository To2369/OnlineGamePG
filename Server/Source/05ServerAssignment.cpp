#include "05ServerAssignment.h"



// サーバ側コマンド入力スレッド
void ServerAssignment05::Exit()
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

void ServerAssignment05::Execute()
{
	// WinsockAPIを初期化
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
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
		// WSAの解放
		WSACleanup();
		return;
	}

	// ノンブロッキング
	u_long mode = 1;
	if (ioctlsocket(sock, FIONBIO, &mode) != 0)
	{
		std::cout << "Nonblocking Mode Failed." << std::endl;
		return;
	}

	// ソケットと受付情報を紐づける
	if (bind(sock, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) != 0)
	{
		std::cout << "Bind Failed." << std::endl;
		return;
	}

	std::cout << "Server Initialize OK." << std::endl;

	// クライアントからの受付処理
	int size = sizeof(struct sockaddr_in);

	// サーバ側からコマンド入力で終了されるまでループする。
	// キーボードでexitを入力するとループを抜けるための別スレッドを用意
	std::thread th(&ServerAssignment05::Exit, this);

	do {
		sockaddr_in temp{};
		char buffer[2048]{};
		int len = sizeof(sockaddr);
		int size = recvfrom(sock, reinterpret_cast<char*>(&buffer), sizeof(buffer), 0, reinterpret_cast<sockaddr*>(&temp), &len);
		if (size > 0)
		{
			// ブロードキャスト通信で受信したデータかチェック
			NetworkTag type;
			memcpy_s(&type, sizeof(short), buffer, sizeof(short));
			switch (type)
			{
				case NetworkTag::Search:
				{
					// searchコマンドならテキストを返す。
					ServerSearch search;
					search.cmd = NetworkTag::Search;
					char message[] = { "Search Hit Server Online Game.\n" };
					strcpy_s(search.messasge, sizeof(message), message);
					std::cout << "Clients Search Access." << std::endl;
					sendto(sock, reinterpret_cast<char*>(&search), sizeof(search), 0, reinterpret_cast<sockaddr*>(&temp), len);
					break;
				}
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

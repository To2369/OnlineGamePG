#include "05ServerAssignment.h"



//----------------------------------------------------------------------------------------
// サーバ側終了コマンド入力スレッド
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

//----------------------------------------------------------------------------------------
//実行処理
void ServerAssignment05::Execute()
{
	// Server課題 サーバ情報設定
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
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(7000);
	addr.sin_addr.S_un.S_addr = INADDR_ANY;//"0.0.0.0"

	// ソケットの作成
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		std::cout << "Create Socket Failed." << std::endl;
		// 9.WSAの解放
		WSACleanup();
		return;
	}

	// ソケットと受付情報を紐づける
	int r = bind(sock, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));
	if (r != 0)
	{
		std::cout << "Bind Failed." << std::endl;
		return;
	}
	// 受付を開始する
	r = listen(sock, 10);
	if (r != 0)
	{
		std::cout << "Listen Failed." << std::endl;
		return;
	}
	std::cout << "Server Initialize OK." << std::endl;

	// クライアントからの受付処理
	int size = sizeof(struct sockaddr_in);

	// 接続要求を処理する
	client.sock = accept(sock, reinterpret_cast<struct sockaddr*>(&client.addr), &size);
	if (client.sock == INVALID_SOCKET)
	{
		// 接続失敗
		std::cout << "Connect Failed." << std::endl;
		return;
	}
	else
	{
		// 接続通知
		std::cout << "Connect Success:IPAddress(";
		std::cout << static_cast<int>(client.addr.sin_addr.S_un.S_un_b.s_b1) << ",";
		std::cout << static_cast<int>(client.addr.sin_addr.S_un.S_un_b.s_b2) << ",";
		std::cout << static_cast<int>(client.addr.sin_addr.S_un.S_un_b.s_b3) << ",";
		std::cout << static_cast<int>(client.addr.sin_addr.S_un.S_un_b.s_b4) << ")." << std::endl;
	}

	// サーバ側からコマンド入力で終了されるまでループする。
	// キーボードでexitを入力するとループを抜けるための別スレッドを用意
	std::thread th(&ServerAssignment05::Exit, this);

	do {

		//受信＆送り返し
		char buffer[256]{};
		int r = recv(client.sock, buffer, sizeof(buffer), 0);	//受信
		if (r > 0)
		{//何か受信したので
			std::cout << "recv : " << buffer << std::endl;
			int s = send(client.sock, buffer, sizeof(buffer), 0); //送り返し
			std::cout << "send : " << buffer << std::endl;
		}

	} while (loop);
	th.join();

	// クライアントのソケット切断
	r = closesocket(client.sock);
	if (r != 0) {
		int err = WSAGetLastError();
		std::cout << "Close Socket Failed.error_code:" << err << "." << std::endl;
	}
	// 受付ソケットの切断
	r = closesocket(sock);
	if (r != 0) {
		int err = WSAGetLastError();
		std::cout << "Close Socket Failed.error_code:" << err << "." << std::endl;
	}

	// WSA終了
	int wsaCleanup = WSACleanup();
	if (wsaCleanup != 0)
	{
		std::cout << "Cleanup WinsockAPI Failed." << std::endl;

	}
	std::cout << "Cleanup WinsockAPI Success." << std::endl;
}
#include "06ServerAssignment.h"



// サーバ側コマンド入力スレッド
void ServerAssignment06::Exit()
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

void ServerAssignment06::Execute()
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
	struct sockaddr_in addr;
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

	// TODO 06_01
	// マルチキャストソケット作成(クライアントへ送信用)
	SOCKET multicastSock;
	struct sockaddr_in multicastAddr;
	

	// マルチキャストIPアドレス指定
	// inetpton IPアドレスの文字列を数値型へ変換する
	// ここではマルチキャストアドレスを指定する
	// マルチキャストアドレスの種類
	//  種類                アドレス範囲                  説明
	// リンクローカル       224.0.0.0 ～ 224.0.0.255      同一セグメント上で使用するアドレス。この範囲のアドレスは
	//                                                    通常、TTL値「1」で送信されるため、ルータで転送されない。
	//                                                    また予約されているアドレスでもある。
	// グローバルスコープ	224.0.1.0 ～ 238.255.255.255  企業が、組織間やインターネットで使用するためのアドレス。
	//                                                    アプリケーションは、アドレスの一部をICANNで予約して使用。
	// プライベートスコープ	239.0.0.0 ～ 239.255.255.255  企業が、組織内で使用するためのアドレス。
	//                                                    「239.0.0.0 / 24」と「239.128.0.0 / 24」は使用しない事が推奨。

	// TODO 06_02
	// inet_pton…標準テキスト表示形式のネットワークアドレスを数値バイナリ形式に変換


	// TODO 06_03
	// ローカルIP設定(第2引数に指定)
	ULONG localAddress = 0;
	// ローカルIPは各自の端末用に変えてください
	

	// TODO 06_04
	// マルチキャストオプションの設定(送信側)
	// 第1引数,socket
	// 第2引数,optionの種類
	// 第3引数,ローカルIPアドレスの指定
	// 第4引数,ローカルIPアドレスのサイズ


	std::cout << "Server Initialize OK." << std::endl;

	// クライアントからの受付処理
	int size = sizeof(struct sockaddr_in);

	// サーバ側からコマンド入力で終了されるまでループする。
	// キーボードでexitを入力するとループを抜けるための別スレッドを用意
	std::thread th(&ServerAssignment06::Exit, this);

	do {
		char buffer[1024];
		int len = sizeof(sockaddr);
		int size = recvfrom(sock, buffer, sizeof(buffer), 0, reinterpret_cast<sockaddr*>(&client), &len);
		if (size > 0)
		{
			std::cout << buffer << std::endl;
			// TODO 06_05
			// マルチキャストアドレスで全員に送信
		}
	} while (loop);
	th.join();

	// サーバソケット切断
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
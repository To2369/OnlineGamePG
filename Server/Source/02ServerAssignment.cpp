#include "02ServerAssignment.h"



// サーバ側コマンド入力スレッド
void ServerAssignment02::Exit()
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

void ServerAssignment02::Execute()
{
	// TODO 02_01
	// WinsockAPIを初期化
	WSADATA wsaData;


	// TODO 02_02
	// サーバの受付設定


	// TODO 02_03
	// ソケットの作成
	SOCKET sock;


	// TODO 02_04
	// ノンブロッキング
	u_long mode = 1;


	// TODO 02_05
	// ソケットと受付情報を紐づける


	std::cout << "Server Initialize OK." << std::endl;

	// クライアントからの受付処理
	int size = sizeof(struct sockaddr_in);

	// サーバ側からコマンド入力で終了されるまでループする。
	// キーボードでexitを入力するとループを抜けるための別スレッドを用意
	std::thread th(&ServerAssignment02::Exit, this);

	do {
		// プレイヤー情報構造体
		PlayerInformation plInfo{};
		int len = sizeof(sockaddr);
		int size = 0;
		// TODO 02_06
		// プレイヤー情報データ受信。受信データはplInfoに保存すること


		if (size > 0)
		{
			// プレイヤー情報表示
			std::cout << "position :(" << plInfo.position.x << "," << plInfo.position.y << "," << plInfo.position.z << ")" << std::endl;
			std::cout << "angle :(" << plInfo.angle.x << "," << plInfo.angle.y << "," << plInfo.angle.z << ")" << std::endl;
			std::cout << "state : " << static_cast<int>(plInfo.state) << std::endl;

			// TODO 02_07 plInfoをクライアントに送信


			if (size < 0)
			{
				std::cout << "sendto failed. error code : " << WSAGetLastError() << std::endl;
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

	// TODO 02_08
	// サーバソケットの切断


	// TODO 02_09
	// WSA終了


	std::cout << "Cleanup WinsockAPI Success." << std::endl;
}
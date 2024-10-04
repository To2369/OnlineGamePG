//--------------------------------------------------
//オンラインゲームプログラミング２　課題1_1(サーバー側)
//		今回は、このサーバー側プログラムと、配布の確認用クライアント実行サンプルの組み合わせで
//		実行、データのやり取りができるか確認します。
//

#include "01ServerAssignment.h"
#define	NONBLOCKING_MODE 1
#define	BUFFER_SIZE	1024

//----------------------------------------------------------------------------------------
// サーバ側修了確認スレッド
void ServerAssignment01::Exit()
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

//-----実行処理-----------------------------------------------------------------------------------
void ServerAssignment01::Execute()
{

	WSADATA wsaData;
	// TODO 01_01 課題作成する事
	// WinsockAPIを初期化(WSAStartup関数)


	// TODO 01_02 課題作成する事
	// サーバの受付設定([addr.sin_family],[addr.sin_port],[addr.sin_addr.S_un.S_addr]の設定)
	struct sockaddr_in addr;

	// TODO 01_03 課題作成する事
	// ソケットの作成	(socket関数でのソケット作成：インターネット、UDP接続設定)
	SOCKET sock;		
	
	// TODO 01_04 課題作成する事
	// ノンブロッキング設定(ioctlsocket関数でのノンブロック指定)
	u_long mode = NONBLOCKING_MODE;

	// TODO 01_05 課題作成する事
	// ソケットと受付情報を紐づける(bind関数)

	std::cout << "Server Initialize OK." << std::endl;

	// クライアントからの受付処理
	int size = sizeof(struct sockaddr_in);

	// サーバ側からコマンド入力で終了されるまでループする。
	// キーボードでexitを入力するとループを抜けるための別スレッドを用意
	std::thread th(&ServerAssignment01::Exit, this);

	do {
		char buffer[BUFFER_SIZE];
		int len = sizeof(sockaddr);

		int size = 0;

		// TODO 01_06 課題作成する事
		// データ受信(recvfrom関数)			


		if (size > 0)//何か受信した！
		{
			std::cout << buffer << std::endl;

			// TODO 01_07 課題作成する事
			// データ送信（sendto関数で送り返し）


			if (size <= 0)
			{
				std::cout << "send error:" << WSAGetLastError() << std::endl;
			}
		}
		else
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK) //エラーか来なかっただけかの判断
			{
				std::cout << "recvfrom failed. error code : " << WSAGetLastError() << std::endl;
			}
		}
	} while (loop);
	th.join();

	// TODO 01_08 課題作成する事
	// サーバソケット切断(closesocket関数)


	// TODO 01_09 課題作成する事
	// WSA終了(WSACleanup関数)


	std::cout << "Cleanup WinsockAPI Success." << std::endl;
}
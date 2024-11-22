#include "02ServerAssignment.h"


//---------------定数---------------------------
#define	NONBLOCKING_MODE 1
#define	BUFFER_SIZE	1024
#define PORT 7000

//------------------------------------------------------------------------------
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

//-----実行処理----------------------------------------------------------------
void ServerAssignment02::Execute()
{

	WSADATA wsaData;

	// TODO 01_01 課題作成する事
	// WinsockAPIを初期化
		//(WSAStartup関数)


	// TODO 01_02 課題作成する事
	// サーバの受付設定
	struct sockaddr_in addr;
		//[addr.sin_family]の設定
		//[addr.sin_port]の設定
		//[addr.sin_addr.S_un.S_addr]の設定

	// TODO 01_03 課題作成する事
	// ソケットの作成
	SOCKET sock;		
		//socket関数でソケット作成：[インターネット接続]、[UDP接続] 設定
		//生成できなければエラー表示して、WSACleanup()し終了
	
	// TODO 01_04 課題作成する事
	// ノンブロッキング設定
	u_long mode = NONBLOCKING_MODE;
		//(ioctlsocket関数でのノンブロック指定)

	// TODO 01_05 課題作成する事
	// ソケットと受付情報を紐づける
		//(bind関数)

	std::cout << "Server Initialize OK." << std::endl;

	// クライアントからの受付処理
	int size = sizeof(struct sockaddr_in);

//↑↑ここまでは課題1のメソッドごとコピー＆ペーストでOKです↑↑

	// サーバ側からコマンド入力で終了されるまでループする。
	// キーボードでexitを入力するとループを抜けるための別スレッドを立ち上げる
	std::thread th(&ServerAssignment02::Exit, this);

	PlayerInformation plInfoOld;//予備課題情報表示用　ストックしておいて新しいデータと比較
	ZeroMemory(&plInfoOld, sizeof(PlayerInformation));

	do {
		// プレイヤー情報構造体
		PlayerInformation plInfo{};
		int len = sizeof(sockaddr);
		int size = 0;
		//今回新規作成部分S01
		// TODO 02_01
		// ソケットsockからプレイヤー情報データ受信(recvfrom関数)
		// recvfromの受信データ保存場所はplInfo





		if (size > 0)	//データ受信した！
		{
			//予備課題02_s
			//position(x,y,z)/angle/state)をplInfoOldのデータと比較、
			//	全て同じなら下にある、プレイヤー情報表示を行わないようにする。
			//比較後次回用に、plInfoをplInfoOldにストック


	



			// プレイヤー情報表示
			std::cout << "position :(" << plInfo.position.x << "," << plInfo.position.y << "," << plInfo.position.z << ")" << std::endl;
			std::cout << "angle :(" << plInfo.angle.x << "," << plInfo.angle.y << "," << plInfo.angle.z << ")" << std::endl;
			std::cout << "state : " << static_cast<int>(plInfo.state) << std::endl;
			//今回新規作成部分S02
			// TODO 02_02 plInfoをクライアントに送信(送り返す)
			//sendotoの戻り値はsizeに代入（直下エラー判断しています。）






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
	th.join();			//コマンド入力スレッド終了を待つ

	//ここから下も、前回のコピー＆ペーストでOK
	// TODO 01_08
	// サーバソケットの切断


	// TODO 01_09
	// WSA終了


	std::cout << "Cleanup WinsockAPI Success." << std::endl;
}
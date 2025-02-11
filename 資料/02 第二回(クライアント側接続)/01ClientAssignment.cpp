﻿#include "01ClientAssignment.h"

//-定数----------------------------------------------

#define BUFFER_SIZE	128
#define XYZ000	DirectX::XMFLOAT3(0.0f,0.0f,0.0f)
#define PLAYER_SIZE DirectX::XMFLOAT3(0.02f,0.02f,0.02f)
#define MESS_WIND_SIZE_X	250
#define MESS_WIND_SIZE_Y	400
#define COM_WIND_SIZE_X 	420
#define COM_WIND_SIZE_Y		650
#define	COM_WIND_POS_X		50
#define COM_WIND_POS_Y		30
#define PORT				7000

//-------コンストラクタ-------------------------------------------------
ClientAssignment01::ClientAssignment01()
{
	input[0] = '\0';
}

//-------デストラクタ-------------------------------------------------
ClientAssignment01::~ClientAssignment01()
{

}


//-------初期化メソッド-------------------------------------------------
void ClientAssignment01::Initialize()
{
	SceneBase::Initialize();

	// TODO 01_10
	// WinsockAPIを初期化


	addrinfo hints;				// DNSへ問い合わせに必要な情報を設定
	addrinfo* addrInfo = NULL;	// 取得したアドレスがここに保存される
	// ゼロクリア
	ZeroMemory(&hints, sizeof(addrinfo));
	// TODO 01_11
	// DNS問い合わせに関する情報を設定
	//プリント記述あり


	const char hostname[] = "";			// ドメイン指定
	const char port[] = "";				// ポート番号指定
	// DNSへ問い合わせ


	// TODO 01_12
	// WinsockAPIを初期化
	// 取得したIPアドレスをsockaddr_inに変換しsin_addrを代入
	//sin_addr, sin_family, sin_portの設定

	// TODO 01_13
	// ソケット作成
	//socket関数でソケットsock作成(UDP接続)


	// TODO 01_14
	// ノンブロッキング設定(ioctlsocket)
	u_long mode = 1;


	// TODO 01_15
	// クライアント情報をサーバへ知らせるためにsendto関数を最初に実行する。
	//sockから適当な文字列(例："CONNECT")をaddrへ送る


	Logger::Print("Connect Success\n");
	
	// Player設定
	playerManager = new PlayerManager();
	playerManager->SetMyPlayerID(0);

	Player* player = new Player();

	// 各プレイヤーのキャラクターに合わせた設定を行う
	// JobClassはステートマシン
	player->SetPlayerID(0);
	player->SetPosition(XYZ000);
	player->SetScale(PLAYER_SIZE);
	player->SetModel();

	playerManager->AddPlayer(player);
	playerManager->GetMyPlayer()->SetReady(true);

	// 受信スレッド実装
	th = std::thread(&ClientAssignment01::recvThread, this);
}


//-------終了メソッド-------------------------------------------------
void ClientAssignment01::Finalize()
{
	// マルチスレッドのループフラグを下ろす
	loop = false;

	// スレッドの終了まで待機
	th.join();

	// TODO 01_18
	// ソケット終了(closesocket)


	// TODO 01_19
	// WSA終了処理(WSACleanup)


}


//-------描画メソッド-------------------------------------------------
void ClientAssignment01::Render()
{
	// 基底クラスのレンダー呼び出し
	SceneBase::Render();
	// imgui
	ImGui::SetNextWindowPos(ImVec2(COM_WIND_POS_X, COM_WIND_POS_Y), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(COM_WIND_SIZE_X, COM_WIND_SIZE_Y), ImGuiCond_Once);
	if (ImGui::Begin("Chat", nullptr, ImGuiWindowFlags_None))
	{
		ImGui::TextWrapped(u8"課題\nサーバにメッセージを送信する。\nサーバは送られてきたデータをクライアントに送信する。\nクライアントはサーバから受信したデータをウィンドウに表示。");
		ImGui::Text("Message");
		ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(MESS_WIND_SIZE_X, MESS_WIND_SIZE_Y), ImGuiWindowFlags_NoTitleBar);
		for (std::string message : messages) {
			ImGui::Text(u8"%s", message.c_str());
		}
		ImGui::EndChild();
		ImGui::Spacing();

		ImGui::InputText("Message", input, sizeof(input));
		if (ImGui::Button("Send"))
		{
			if (strcmp(input, "") != 0)
			{
				int size=0; 
				// TODO 01_16
				// 入力されたデータを送信
				//socketから入力データ(input)をaddrへ送る(sendto)、戻り値(送信バイト)はsizeへ代入



				if (size < 0)
				{
					Logger::Print("Error Code:%d\n",WSAGetLastError());
				}
			}
			input[0] = '\0';
		}
		if (ImGui::Button(u8"タイトルへ"))
		{
			SceneManager::Instance().ChangeScene(new SceneTitle());
		}

	}
	ImGui::End();
}

//-------受信スレッド-------------------------------------------------
void ClientAssignment01::recvThread()
{
	int len = sizeof(sockaddr_in);
	do {
		char buffer[BUFFER_SIZE]{};
		int size = 0;

		// TODO 01_17
		//sockからbufferへ受信(recvfrom)する、相手アドレスはaddressへ、戻り値として受信データ数が返ってくるので、sizeへ代入
		//戻り値の受信データ数(size)が、>0ならなにか受信したので、メッセージウィンドウへ受信データ(buffer)を送って(messages.emplace_back関数)表示する
		// データ受信



	} while (loop);

}
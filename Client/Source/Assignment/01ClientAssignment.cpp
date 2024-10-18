#include "01ClientAssignment.h"

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
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		Logger::Print("1_1Err\n");
		return;
	}
	addrinfo hints;				// DNSへ問い合わせに必要な情報を設定
	addrinfo* addrInfo = NULL;	// 取得したアドレスがここに保存される
	// ゼロクリア
	ZeroMemory(&hints, sizeof(addrinfo));
	// TODO 01_11
	// DNS問い合わせに関する情報を設定
	const char hostname[] = "localhost";			// ドメイン指定
	const char port[] = "7000";				// ポート番号指定
	// DNSへ問い合わせ

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	if (getaddrinfo(hostname, port, &hints, &addrInfo) != 0)
	{
		Logger::Print("getaddrinfo error.\n");
		return;
	}

	// TODO 01_12
	// WinsockAPIを初期化
	// 取得したIPアドレスをsockaddr_inに変換しsin_addrを代入
	addr.sin_addr = reinterpret_cast<sockaddr_in*>(addrInfo->ai_addr)->sin_addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(7000);

	// TODO 01_13
	// ソケット作成
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == INVALID_SOCKET)
	{
		Logger::Print("01_13ERR\n");
		return;
	}

	// TODO 01_14
	// ノンブロッキング設定
	u_long mode = 1;
	int m = ioctlsocket(sock, FIONBIO, &mode);
	if (m!=0)
	{
		Logger::Print("01_14ERR\n");
		return;
	}
	// TODO 01_15
	// クライアント情報をサーバへ知らせるためにsendto関数を最初に実行する。
	int size = sendto(sock, "connect", strlen("connect") + 1, 0,
		reinterpret_cast<sockaddr*>(&addr), sizeof(addr));
	if (size<0)
	{
		Logger::Print("01_15ERR\n",WSAGetLastError());
		return;
	}

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
	// ソケット終了
	if (closesocket(sock)!=0)
	{
		Logger::Print("01_18ERR\n", WSAGetLastError());
	}

	// TODO 01_19
	// WSA終了処理
	if (WSACleanup() != 0)
	{
		Logger::Print("01_19ERR\n", WSAGetLastError());
	}

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
				// TODO 01_16
				// 入力されたデータを送信
				int size = sendto(sock, input, static_cast<int>(strlen(input) + 1),
					0, reinterpret_cast<sockaddr*>(&addr), static_cast<int>(sizeof(addr)));
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
		// データ受信
		size = recvfrom(sock, buffer, sizeof(buffer), 0, reinterpret_cast<sockaddr*>(&recvAddr), &len);
		if (size > 0)
		{
			messages.push_back(buffer);
		}

	} while (loop);

}
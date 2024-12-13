#include "04ClientAssignment.h"

// メモリリーク調査用
#define debug_new new(_NORMAL_BLOCK,__FILE__,__LINE__)


ClientAssignment04::ClientAssignment04() :messages(15)
{
	input.buffer[0] = '\0';
}

ClientAssignment04::~ClientAssignment04()
{

}

void ClientAssignment04::Initialize()
{
	SceneBase::Initialize();
	// WinsockAPIを初期化
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		// 初期化失敗
		Logger::Print("WSA Initialize Failed.\n");
		return;
	}


	// DNSへ問い合わせ
	addrinfo hints;				// DNSへ問い合わせに必要な情報を設定
	addrinfo* addrInfo = NULL;	// 取得したアドレスがここに保存される
	// ゼロクリア
	ZeroMemory(&hints, sizeof(addrinfo));
	// 設定する情報を設定
	hints.ai_family = AF_INET;				// IPv4で取得
	hints.ai_socktype = SOCK_STREAM; 		// TCP通信でDNSサーバへアクセス

	const char hostname[] = "localhost";	// ドメイン指定
	const char port[] = "7000";				// ポート番号指定
	// DNSへ問い合わせ0のとき正常に完了
	if (getaddrinfo(hostname, port, &hints, &addrInfo) != 0) {
		Logger::Print("getaddrinfo error.\n");

		return;
	}

	// WinsockAPIを初期化
	// 取得したIPアドレスをsockaddr_inに変換しsin_addrを代入
	addr.sin_addr = reinterpret_cast<sockaddr_in*>(addrInfo->ai_addr)->sin_addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(7000);


	// ソケット作成
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == INVALID_SOCKET) {
		Logger::Print("create socket Failed.\n");
		return;
	}

	// ノンブロッキング設定
	u_long mode = 1;
	int m = ioctlsocket(sock, FIONBIO, &mode);
	if (m != 0)
	{
		Logger::Print("Nonblocking Mode Failed.\n");
		return;
	}

	// クライアント情報をサーバへ知らせるためにsendto関数を最初に実行する。
	PlayerMessage message;
	message.cmd = NetworkTag::Message;
	strcpy_s(message.buffer, "connect");
	int size = sendto(sock, reinterpret_cast<char*>(&message), sizeof(message), 0, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));
	if (size < 0)
	{
		Logger::Print("connected failed. error code:%d\n", WSAGetLastError());
	}
	Logger::Print("Connect Success\n");

	// Player設定
	playerManager = debug_new PlayerManager();
	playerManager->SetMyPlayerID(0);
	Player* player = new Player();
	// 各プレイヤーのキャラクターに合わせた設定を行う
	// JobClassはステートマシン
	player->SetPlayerID(0);
	player->SetPosition(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	player->SetScale(DirectX::XMFLOAT3(0.02f, 0.02f, 0.02f));
	player->SetModel();

	playerManager->AddPlayer(player);
	playerManager->GetMyPlayer()->SetReady(true);
	// 受信スレッド実装
	recvTh = std::thread(&ClientAssignment04::recvThread, this);
}

void ClientAssignment04::Finalize()
{

	// マルチスレッドのループフラグを下ろす
	loop = false;

	// ソケットがブロッキングなので終了コードを送る
	PlayerLogout logout{};
	logout.cmd = NetworkTag::Logout;
	logout.id = playerManager->GetMyPlayerID();

	int s = sendto(sock, reinterpret_cast<char*>(&logout), sizeof(logout), 0,
		reinterpret_cast<sockaddr*>(&addr), static_cast<int>(sizeof(addr)));
	if (s < 0)
	{
		Logger::Print("finalize send failed. error code:%d\n");
	}

	// スレッドの終了まで待機
	recvTh.join();

	// server終了処理
	int r = closesocket(sock);
	if (r != 0) {
		int err = WSAGetLastError();
		Logger::Print("Close Socket Failed.error_code:%d\n", err);
	}

	int wsacleanup = WSACleanup();
}

void ClientAssignment04::Render()
{
	// 基底クラスのレンダー呼び出し
	SceneBase::Render();
	// imgui
	ImGui::SetNextWindowPos(ImVec2(50, 50), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(300, 620), ImGuiCond_Once);
	if (ImGui::Begin("Chat", nullptr, ImGuiWindowFlags_None))
	{
		ImGui::Text("Message");
		ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(250, 470), ImGuiWindowFlags_NoTitleBar);

		for (int i = 0; i < messages.size();++i) {
			ImGui::Text(u8"%s", messages.at(i).buffer);
		}

		ImGui::EndChild();
		ImGui::Spacing();
		input.cmd = NetworkTag::Message;
		ImGui::InputText("Message", input.buffer, sizeof(input.buffer));
		if (ImGui::Button("Send"))
		{
			if (strcmp(input.buffer, "") != 0)
			{
				// 送信処理
				int size = sendto(sock, reinterpret_cast<char *>(&input), sizeof(input), 0, reinterpret_cast<sockaddr*>(&addr), static_cast<int>(sizeof(addr)));
				if (size < 0)
				{
					Logger::Print("Error Code:%d\n");
				}
			}
			input.buffer[0] = '\0';
		}
		if (ImGui::Button(u8"タイトルへ"))
		{
			SceneManager::Instance().ChangeScene(new SceneTitle());
		}
	}
	ImGui::End();
}


void ClientAssignment04::recvThread()
{
	int size = sizeof(sockaddr_in);
	do {
		char buffer[2048]{};
		int r = recvfrom(sock, reinterpret_cast<char*>(&buffer), sizeof(buffer), 0, reinterpret_cast<sockaddr*>(&recvAddr), &size);
		if (r > 0)
		{
			// タグ部分を取得し処理を分岐させる
			short type = 0;
			memcpy_s(&type, sizeof(short), buffer, sizeof(short));
			switch (static_cast<NetworkTag>(type))
			{
				case NetworkTag::Message:
				{
					PlayerMessage message;
					//データプッシュするために受信データのbuffer(メッセージ内容)からmessageにコピー
					memcpy_s(&message, sizeof(message), buffer, sizeof(message));

					// TODO 04_04
					// 受信したメッセージをリングバッファに保存する。
					messages.push(message);

					break;
				}
			}
		}
	} while (loop);
}

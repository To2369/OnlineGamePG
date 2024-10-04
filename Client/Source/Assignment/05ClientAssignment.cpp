#include <WinSock2.h>
#include "05ClientAssignment.h"

// メモリリーク調査用
#define debug_new new(_NORMAL_BLOCK,__FILE__,__LINE__)


ClientAssignment05::ClientAssignment05()
{
	input.buffer[0] = '\0';
}

ClientAssignment05::~ClientAssignment05()
{

}

void ClientAssignment05::Initialize()
{
	SceneBase::Initialize();
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		// 初期化失敗
		Logger::Print("WSA Initialize Failed.\n");
		return;
	}
	// ソケット作成
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == INVALID_SOCKET) {
		Logger::Print("ソケットの生成に失敗しました\n");
		return;
	}

	// ノンブロッキング設定
	u_long mode = 1;
	if (ioctlsocket(sock, FIONBIO, &mode) != 0)
	{
		Logger::Print("Nonblocking Mode Failed.\n");
		return;
	}

	// 接続先情報設定
	addr.sin_family = AF_INET;
	addr.sin_port = htons(7000);

	// TODO 05_01
	// 接続先IP設定(ブロードキャストアドレスを指定してください)


	// TODO 05_02
	// ブロードキャスト通信の設定をしてください。
	int flg = 1;



	// Player設定
	playerManager = new PlayerManager();
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
	recvTh = std::thread(&ClientAssignment05::recvThread, this);
}

void ClientAssignment05::Finalize()
{

	// マルチスレッドのループフラグを下ろす
	loop = false;

	recvTh.join();
	messages.clear();
	// server終了処理
	int r = closesocket(sock);
	if (r != 0) {
		int err = WSAGetLastError();
		Logger::Print("Close Socket Failed.error_code:\n");
	}

	int wsacleanup = WSACleanup();
}

void ClientAssignment05::Render()
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
		for (std::string message : messages) {
			ImGui::Text(u8"%s", message.c_str());
		}
		ImGui::EndChild();
		ImGui::Spacing();
		ServerSearch search;
		search.cmd = NetworkTag::Search;
		if (ImGui::Button("Search"))
		{
			// 送信処理
			int size = sendto(sock, reinterpret_cast<char *>(&search), sizeof(search), 0, reinterpret_cast<sockaddr*>(&addr), static_cast<int>(sizeof(addr)));
			if (size < 0)
			{
				Logger::Print("Error Code:%d\n", WSAGetLastError());
			}
		}
		if (ImGui::Button(u8"タイトルへ"))
		{
			SceneManager::Instance().ChangeScene(new SceneTitle());
		}
	}
	ImGui::End();
}

void ClientAssignment05::NetrowkUpdate(float elapsedTime)
{

}

void ClientAssignment05::recvThread()
{
	int size = sizeof(sockaddr_in);
	do {
		char buffer[258]{};
		int r = recvfrom(sock, reinterpret_cast<char*>(&buffer), sizeof(buffer), 0, reinterpret_cast<sockaddr*>(&recvAddr), &size);
		if (r > 0)
		{
			// タグ部分を取得し処理を分岐させる
			short type = 0;
			memcpy_s(&type, sizeof(short), buffer, sizeof(short));
			switch (static_cast<NetworkTag>(type))
			{
				case NetworkTag::Search:
				{
					ServerSearch search;
					memcpy_s(&search, sizeof(ServerSearch), buffer, sizeof(ServerSearch));
					// レスポンスのあったメッセージを表示するように変数に格納
					messages.emplace_back(search.messasge);
					break;
				}
			}
		}
	} while (loop);
}

#include <WinSock2.h>
#include "03ClientAssignment.h"

// メモリリーク調査用
#define debug_new new(_NORMAL_BLOCK,__FILE__,__LINE__)


ClientAssignment03::ClientAssignment03()
{
	input[0] = '\0';
}

ClientAssignment03::~ClientAssignment03()
{
	
}

void ClientAssignment03::Initialize()
{
	SceneBase::Initialize();
	int wsaStartUp = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (wsaStartUp != 0)
	{
		// 初期化失敗
		Logger::Print("WSA Initialize Failed.");
		return;
	}

	// 接続先情報設定
	addr.sin_family = AF_INET;
	addr.sin_port = htons(7000);

	// 接続先IP設定
	addr.sin_addr.S_un.S_un_b.s_b1 = 127;
	addr.sin_addr.S_un.S_un_b.s_b2 = 0;
	addr.sin_addr.S_un.S_un_b.s_b3 = 0;
	addr.sin_addr.S_un.S_un_b.s_b4 = 1;

	// 送信する際のポートを指定する
	self.sin_family = AF_INET;
	self.sin_port = htons(7001);

	// 接続先IP設定
	self.sin_addr.S_un.S_un_b.s_b1 = 0;
	self.sin_addr.S_un.S_un_b.s_b2 = 0;
	self.sin_addr.S_un.S_un_b.s_b3 = 0;
	self.sin_addr.S_un.S_un_b.s_b4 = 0;


	// ソケット作成
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == INVALID_SOCKET) {
		Logger::Print("create socket Failed.");
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
	// 新規ユーザのIDはダミーデータとして-1にしておく
	PlayerLogin login{};
	login.cmd = NetworkTag::Login;
	login.id = -1;
	// TODO 03_07
	// loginデータの送信




	// Player設定
	playerManager = debug_new PlayerManager();

	// 受信スレッド実装
	recvTh = std::thread(&ClientAssignment03::recvThread, this);
}


void ClientAssignment03::Finalize()
{

	// マルチスレッドのループフラグを下ろす
	loop = false;

	PlayerLogout logout{};
	logout.cmd = NetworkTag::Logout;
	logout.id = playerManager->GetMyPlayerID();
	int size = 0;
	
	// TODO 03_10
	// logoutデータを送信



	// スレッドの終了まで待機
	recvTh.join();

	// server終了処理
	if (closesocket(sock) != 0) {
		int err = WSAGetLastError();
		Logger::Print("Close Socket Failed.error_code:%d\n", err);
		{
		}
	}

	// WSA終了処理
	if (WSACleanup() != 0)
	{
		int err = WSAGetLastError();
		Logger::Print("Cleanup WinsockAPI Failed.%d\n", err);
	}
}

void ClientAssignment03::Render()
{
	// 基底クラスのレンダー呼び出し
	SceneBase::Render();
	// imgui
	ImGui::SetNextWindowPos(ImVec2(50, 50), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(300, 100), ImGuiCond_Once);
	if (ImGui::Begin("Title", nullptr, ImGuiWindowFlags_None))
	{
		if (ImGui::Button(u8"タイトルへ"))
		{
			SceneManager::Instance().ChangeScene(new SceneTitle());
		}
	}
	ImGui::End();
}

void ClientAssignment03::NetrowkUpdate(float elapsedTime)
{
	// 操作キャラクター取得
	Player* player = playerManager->GetMyPlayer();

	// カメラコントローラー更新処理
	if (player != nullptr)
	{
		HWND hWnd;
		hWnd = GetActiveWindow();
		if (hWnd != NULL)
		{
			// キー入力判定(入力データはそのままサーバに送らず更新)
			switch (player->GetState())
			{
				case Player::State::Idle:
					if (player->InputMove(elapsedTime))
					{
						if (player)player->GetJobClass()->ChangeState(Player::State::Move);
					}
					if (player->InputAttack())
					{
						player->GetJobClass()->ChangeState(Player::State::Attack);
					}
					break;
				case Player::State::Move:
					if (!player->InputMove(elapsedTime))
					{
						player->GetJobClass()->ChangeState(Player::State::Idle);
					}
					if (player->InputAttack())
					{
						player->GetJobClass()->ChangeState(Player::State::Attack);
					}
					break;
				case Player::State::Attack:
					if (!player->GetModel()->IsPlayAnimation())
					{
						player->GetJobClass()->ChangeState(Player::State::Idle);
					}
					break;
			}
		}


		// 毎フレーム送信する同期データ
		PlayerInformation plInfo{};
		plInfo.cmd = NetworkTag::Sync;

		// 送信データ
		plInfo.id = player->GetPlayerID();
		plInfo.position = player->GetPosition();
		plInfo.angle = player->GetAngle();
		plInfo.state = player->GetState();

		// TODO 03_08
		// 同期データを送信

	}
}

void ClientAssignment03::recvThread()
{
	int size = sizeof(sockaddr_in);
	do {
		char buffer[2048]{};
		int r = recvfrom(sock, reinterpret_cast<char*>(&buffer), sizeof(buffer), 0, reinterpret_cast<sockaddr*>(&recvAddr), &size);
		if (r > 0)
		{
			// TODO 03_09
			// 受信データの処理のコードを読み理解を行うこと。
			
			// 受信データからネットワークタグを取得
			short type = 0;
			memcpy_s(&type, sizeof(short), buffer, sizeof(short));
			switch (static_cast<NetworkTag>(type))
			{
				case NetworkTag::Sync:
				{
					// Syncタグの場合
					// 受信データをplInfoにコピーする
					PlayerInformation plInfo{};
					memcpy_s(&plInfo, sizeof(PlayerInformation), buffer, sizeof(PlayerInformation));

					// 受信データをログ出力
					Logger::Print("recv data id:%d\n", plInfo.id);
					Logger::Print("position x:%d", plInfo.position.x);
					Logger::Print("y:%d", plInfo.position.y);
					Logger::Print("z:%d\n", plInfo.position.z);
					Logger::Print("angle x:%d", plInfo.angle.x);
					Logger::Print("y:%d", plInfo.angle.y);
					Logger::Print("z:%d", plInfo.angle.z);
					Logger::Print("state:%d\n", static_cast<int>(plInfo.state));

					bool inserted = false;
					// idが登録されているか確認
					for (Player* player : playerManager->GetPlayers())
					{
						if (player->GetPlayerID() == plInfo.id)
						{
							// 登録済フラグを立てる
							inserted = true;
						}
					}
					// 登録されていない場合、クライアントより先にログインしている
					// プレイヤーのデータのため新規登録を行う。
					if (!inserted)
					{
						// 新規登録
						Player* player = debug_new Player();
						player->SetPlayerID(plInfo.id);
						player->SetPosition(plInfo.position);
						player->SetAngle(plInfo.angle);
						player->SetScale(DirectX::XMFLOAT3(0.02f, 0.02f, 0.02f));
						player->SetModel();
						player->GetJobClass()->ChangeState(plInfo.state);
						player->SetReady(true);

						playerManager->AddPlayer(player);
					}
					else
					{
						// すでに登録されている場合、データ更新
						Player* player = playerManager->GetPlayer(plInfo.id);
						player->SetPosition(plInfo.position);
						player->SetAngle(plInfo.angle);
						player->SetScale(DirectX::XMFLOAT3(0.02f, 0.02f, 0.02f));
						player->GetJobClass()->ChangeState(plInfo.state);
					}
					Logger::Print("Synchronize Data id:%d\n", std::to_string(plInfo.id));

					break;
				}
				case NetworkTag::Login:
				{
					// Loginタグの場合
					// 受信データをLoginにコピーする
					PlayerLogin login{};
					memcpy_s(&login, sizeof(PlayerLogin), buffer, sizeof(PlayerLogin));

					// プレイヤー新規登録
					Player* player = debug_new Player();
					player->SetPlayerID(login.id);
					player->SetPosition(DirectX::XMFLOAT3(0, 0, 0));
					player->SetScale(DirectX::XMFLOAT3(0.02f, 0.02f, 0.02f));
					player->SetAngle(DirectX::XMFLOAT3(0, 0, 0));
					player->SetModel();
					player->GetJobClass()->ChangeState(Player::State::Idle);
					player->SetReady(true);
					playerManager->AddPlayer(player);

					// 受信したIDを初回だけ設定
					if (playerManager->GetMyPlayerID() < 0)
					{
						playerManager->SetMyPlayerID(login.id);
						Logger::Print("My Player ID:%d\n", login.id);
					}
					break;
				}
				case NetworkTag::Logout:
				{
					// Logoutタグの場合
					// 受信データをLogoutにコピーする
					PlayerLogout logout{};
					memcpy_s(&logout, sizeof(PlayerLogout), buffer, sizeof(PlayerLogout));
					// ログアウトしたプレイヤーを削除用配列に保存
					// 削除自体はUpdate関数の最後に実行している
					playerManager->ErasePlayer(logout.id);
					Logger::Print("Delete Player ID:%d\n", logout.id);
					break;
				}
			}
		}
	} while (loop);
}

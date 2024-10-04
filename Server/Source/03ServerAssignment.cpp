#include "03ServerAssignment.h"



// サーバ側コマンド入力スレッド
void ServerAssignment03::Exit()
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

void ServerAssignment03::Execute()
{
	// WinsockAPIを初期化
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		// 初期化失敗
		std::cout << "WSA Initialize Failed." << std::endl;
		return;
	}

	// サーバの受付設定
	struct sockaddr_in addr {};
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
	if (ioctlsocket(sock, FIONBIO, &mode) != 0)
	{
		std::cout << "Nonblocking Mode Failed." << std::endl;
		return;
	}

	// ソケットと受付情報を紐づける
	if (bind(sock, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) != 0)
	{
		std::cout << "Bind Failed." << std::endl;
		return;
	}

	std::cout << "Server Initialize OK." << std::endl;

	// クライアントからの受付処理
	int size = sizeof(struct sockaddr_in);

	// サーバ側からコマンド入力で終了されるまでループする。
	// キーボードでexitを入力するとループを抜けるための別スレッドを用意
	std::thread th(&ServerAssignment03::Exit, this);

	do {
		sockaddr_in temp{};
		char buffer[2048]{};
		int len = sizeof(sockaddr);
		int size = 0;
		// TODO 03_01
		// 受信データをbufferに保存
		// 送信者の情報はtempに保存


		if (size > 0)
		{
			// TODO 03_02
			// if(1)はダミーコードのため、HasSameData関数を使用して判断するコードに修正しなさい
			// HasSameDataの仕様は関数を読み込むこと
			if(1)
			{
				// 新規クライアントの場合
				// 受信データからネットワークタグを取得
				NetworkTag type;
				memcpy_s(&type, sizeof(short), buffer, sizeof(short));

				// 取得したタグをもとに処理を分岐
				switch (type)
				{
					case NetworkTag::Login:
					{
						// Loginタグの場合

						// loginに受信データをコピーする
						PlayerLogin login{};
						memcpy_s(&login, sizeof(PlayerLogin), buffer, sizeof(PlayerLogin));

						// IDを割り振り次の接続者のためにIDを加算しておく
						login.id = giveID;
						++giveID;

						// プレイヤー作成
						Player player{};
						player.id = login.id;
						player.position = DirectX::XMFLOAT3(0, 0, 0);
						player.angle = DirectX::XMFLOAT3(0, 0, 0);
						player.state = Player::State::Idle;

						// 新規クライアントを追加
						Client* newClient = new Client;
						newClient->addr = temp;
						newClient->player = player;
						clients.emplace_back(newClient);
						std::cout << "new client insert." << std::endl;

						for (Client* client : clients)
						{
							// TODO 03_03
							// 全クライアントにlogin通知


							PlayerInformation plInfo{};
							plInfo.cmd = NetworkTag::Sync;
							plInfo.id = client->player.id;
							plInfo.position = client->player.position;
							plInfo.angle = client->player.angle;
							plInfo.state = client->player.state;
							// TODO 03_04
							// plInfoを送信者に送信


						}

						break;
					}
					default:
					{
						break;
					}
				}
			}
			else
			{
				// 既存のクライアントの場合
				// 受信データからネットワークタグを取得
				NetworkTag type;
				memcpy_s(&type, sizeof(short), buffer, sizeof(short));

				switch (type)
				{
					case NetworkTag::Sync:
					{
						// Syncタグの場合
						// 受信データをplInfoにコピーする
						PlayerInformation plInfo{};
						memcpy_s(&plInfo, sizeof(PlayerInformation), buffer, sizeof(PlayerInformation));

						// 受信データを表示
						std::cout << "position :(" << plInfo.position.x << "," << plInfo.position.y << "," << plInfo.position.z << ")" << std::endl;
						std::cout << "angle :(" << plInfo.angle.x << "," << plInfo.angle.y << "," << plInfo.angle.z << ")" << std::endl;
						std::cout << "state : " << static_cast<int>(plInfo.state) << std::endl;

						// 既存クライアント全員に送信
						for (Client* client : clients)
						{
							// プレイヤーのサーバ情報更新
							if (client->player.id == plInfo.id)
							{
								client->player.position = plInfo.position;
								client->player.angle = plInfo.angle;
								client->player.state = plInfo.state;
							}
							else
							{
								// TODO 03_05
								// プレイヤー情報を送信する


								if (size < 0)
								{
									std::cout << "sendto failed. error code : " << WSAGetLastError() << std::endl;
								}
							}
						}
						break;
					}
					case NetworkTag::Logout:
					{
						// ログアウトデータの場合
						// 受信データをlogoputにコピーする
						PlayerLogout logout{};
						memcpy_s(&logout, sizeof(PlayerLogout), buffer, sizeof(PlayerLogout));

						for (auto it = clients.begin(); it != clients.end();)
						{
							// 送信者とidが同じとき
							if ((*it)->player.id == logout.id) {
								// データを削除
								it = clients.erase(it);
							}
							else {
								// TODO 03_06
								// logoutデータを送信者以外に送信


								++it; // 次の要素に進む
							}
						}
						break;
					}
				}
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


	// クライアント終了処理
	for (Client* client : clients)
	{
		if (client != nullptr)
		{
			delete client;
			client = nullptr;
		}
	}

	// サーバソケットの切断
	if (closesocket(sock) != 0) {
		int err = WSAGetLastError();
		std::cout << "Close Socket Failed.error_code:" << err << "." << std::endl;
	}

	// WSA終了
	if (WSACleanup() != 0)
	{
		std::cout << "Cleanup WinsockAPI Failed." << std::endl;

	}
	std::cout << "Cleanup WinsockAPI Success." << std::endl;
}

bool ServerAssignment03::HasSameData(const std::vector<Client*>& vec, const sockaddr_in& target)
{
	for (const Client* client : vec)
	{
		if (client->addr.sin_addr.S_un.S_addr == target.sin_addr.S_un.S_addr &&
			client->addr.sin_port == target.sin_port)
		{
			return true;
		}
	}
	return false;
}
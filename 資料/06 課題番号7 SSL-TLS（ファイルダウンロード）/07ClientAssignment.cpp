#include <string>
#include <fstream>
#include <WinSock2.h>		// WinSock version 2を使用するためのヘッダ
#include <WS2tcpip.h>		// WinSock version 2で使用する様々なオプションのヘッダ
// OpenSSL読み込み
#include <openssl/crypto.h>		// opensslの暗号化ヘッダ
#include <openssl/ssl.h>		// opensslのssl通信ヘッダ
#include <openssl/err.h>		// opensslのエラーヘッダ

#include "07ClientAssignment.h"
#include <algorithm>

//----------定数----------------------------------------
#define PL_POS DirectX::XMFLOAT3(0.0f,0.0f,0.0f)
#define PL_SIZE DirectX::XMFLOAT3(0.2f,0.2f,0.2f)
#define WIND_POS ImVec2(screenWidth/2-150,200)
#define WIND_SIZE ImVec2(300,100)
#define DOWNLOAD_PORT "443"

#define DATA_KEEP_SIZE 4096  	//初期サイズを確保
#define BUFFER_SIZE	1024
#define REQUEST_BUF_SIZE 1024
//---------------------------------------------------------------------------

//------------------------------------------------------------//
//コンストラクタ
ClientAssignment07::ClientAssignment07()
{

}

//------------------------------------------------------------//
//デストラクタ
ClientAssignment07::~ClientAssignment07()
{

}

//------------------------------------------------------------//
//初期化
void ClientAssignment07::Initialize()
{
	SceneBase::Initialize();

	// Player設定
	playerManager = new PlayerManager();
	playerManager->SetMyPlayerID(0);

	Player* player = new Player();


	// 各プレイヤーのキャラクターに合わせた設定を行う
	// JobClassはステートマシン
	player->SetPlayerID(0);
	player->SetPosition( PL_POS );
	player->SetScale( PL_SIZE );
	player->SetModel();

	playerManager->AddPlayer(player);
	playerManager->GetMyPlayer()->SetReady(true);
	SSL_load_error_strings();
	OpenSSL_add_ssl_algorithms();
}

//------------------------------------------------------------//
//終了処理
void ClientAssignment07::Finalize()
{
	if (recvTh.joinable())
	{
		recvTh.join();
	}
	OPENSSL_cleanup();
	CRYPTO_cleanup_all_ex_data();
	ERR_free_strings();
}

//------------------------------------------------------------//
//レンダー処理
void ClientAssignment07::Render()
{
	// 基底クラスのレンダー呼び出し
	SceneBase::Render();
	// imgui
	Graphics& graphics = Graphics::Instance();
	float screenWidth = static_cast<float>(graphics.GetScreenWidth());
	ImGui::SetNextWindowPos(WIND_POS, ImGuiCond_Once);
	ImGui::SetNextWindowSize(WIND_SIZE, ImGuiCond_Once);
	if (ImGui::Begin("Download", nullptr, ImGuiWindowFlags_None))
	{
		if (ImGui::Button("Start"))
		{
			// TODO 07_01
			// ダウンロードファイルの指定
			// ホスト名(ドメイン部分)
			std::string hostname = "";
			// パス
			std::string path = "";
			// ファイル名
			std::string filename = "";

			if (recvTh.joinable())recvTh.join();
			recvTh = std::thread(&ClientAssignment07::FileDownload, this, hostname, path, filename);
		}
		if (ImGui::Button(u8"タイトルへ"))
		{
			SceneManager::Instance().ChangeScene(new SceneTitle());
		}

	}
	ImGui::End();
}

//------------------------------------------------------------//
//ファイルのダウンロード　SSL/TLS　メイン部
void ClientAssignment07::FileDownload(const std::string& hostname, const std::string& path, const std::string& filename)
{
	// TODO 07_02
	// OpenSSLの初期化

	WSADATA wsaData;
	SOCKET sock = INVALID_SOCKET;
	addrinfo hints = {}, * addrInfo = nullptr;

	try {
		// WSA初期化
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
			throw std::runtime_error("WSA初期化失敗");
		}

		// 通信方式設定
		hints.ai_family = AF_INET;			  // IPv4設定
		hints.ai_socktype = SOCK_STREAM;		// TCP設定
		hints.ai_protocol = IPPROTO_TCP;

		// ドメインとポート番号からアドレス情報を取得する
		const std::string port = DOWNLOAD_PORT;
		if (getaddrinfo(hostname.c_str(), port.c_str(), &hints, &addrInfo) != 0) {
			throw std::runtime_error("ドメインからアドレス取得に失敗しました");
		}

		// ソケット作成
		sock = socket(addrInfo->ai_family, addrInfo->ai_socktype, addrInfo->ai_protocol);
		if (sock == INVALID_SOCKET) {
			throw std::runtime_error("ソケットの生成に失敗しました");
		}

		// サーバに接続
		if (connect(sock, addrInfo->ai_addr, static_cast<int>(addrInfo->ai_addrlen)) == SOCKET_ERROR) {
			throw std::runtime_error("connectに失敗しました");
		}

		// TODO 07_03
		// SSLコンテキスト作成
		SSL_CTX* ctx;

		// TODO 07_04
		// SSLオブジェクト作成
		SSL* ssl;

		// TODO 07_05
		// SSLオブジェクトにソケットを関連付ける


		// TODO 07_06
		// サーバに接続
		

		// TODO 07_07
		// HTTPリクエストを作成
		char request[REQUEST_BUF_SIZE];
		
		
		// TODO 07_08
		// リクエスト送信


		Logger::Print("サーバからのレスポンス\n");
		std::vector<char> data;
		data.reserve(DATA_KEEP_SIZE); // 初期サイズを確保
		char buf[BUFFER_SIZE];
		int size = 0;

		// TODO 07_09
		// データ受信ループ



		if (size < 0) {
			throw std::runtime_error("受信エラー");
		}

		// HTTPヘッダーの削除
		auto headerEnd = std::search(data.begin(), data.end(), "\r\n\r\n", "\r\n\r\n" + 4);
		if (headerEnd != data.end()) {
			data.erase(data.begin(), headerEnd + 4);
		}

		// ファイル書き出し
		std::ofstream writingFile(filename, std::ios::binary);
		if (!writingFile) {
			throw std::runtime_error("ファイルのオープンに失敗しました");
		}
		writingFile.write(data.data(), data.size());

		// TODO 07_10
		// リソース解放


		closesocket(sock);
		WSACleanup();
	}
	catch (const std::exception& ex) {
		Logger::Print("エラー:%s\n",ex.what());
		if (sock != INVALID_SOCKET) closesocket(sock);
		WSACleanup();
	}
}
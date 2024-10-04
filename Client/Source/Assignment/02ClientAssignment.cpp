#include "02ClientAssignment.h"



ClientAssignment02::ClientAssignment02()
{
	input[0] = '\0';
}

ClientAssignment02::~ClientAssignment02()
{
	
}

void ClientAssignment02::Initialize()
{
	SceneBase::Initialize();
	// TODO 02_10
	// WinsockAPIを初期化


	addrinfo hints;				// DNSへ問い合わせに必要な情報を設定
	addrinfo* addrInfo = NULL;	// 取得したアドレスがここに保存される
	// ゼロクリア
	ZeroMemory(&hints, sizeof(addrinfo));
	// TODO 01_11
	// DNS問い合わせに関する情報を設定

	const char hostname[] = "";	// ドメイン指定
	const char port[] = "";		// ポート番号指定
	// DNSへ問い合わせ


	// TODO 02_12
	// WinsockAPIを初期化
	// 取得したIPアドレスをsockaddr_inに変換しsin_addrを代入


	// TODO 02_13
	// ソケット作成

	// TODO 02_14
	// ノンブロッキング設定
	u_long mode = 1;


	// TODO 02_15
	// クライアント情報をサーバへ知らせるためにsendto関数を最初に実行する。


	Logger::Print("Connect Success\n");

	// Player設定
	playerManager = new PlayerManager();
	playerManager->SetMyPlayerID(0);

	Player* player = new Player();

	// 各プレイヤーのキャラクターに合わせた設定を行う
	// JobClassはステートマシン
	player->SetPlayerID(0);
	player->SetPosition(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	player->SetScale(DirectX::XMFLOAT3(0.02f, 0.02f, 0.02f));
	player->SetState(Player::State::Idle);
	player->SetModel();

	playerManager->AddPlayer(player);
	playerManager->GetMyPlayer()->SetReady(true);

	// 受信スレッド実装
	th = std::thread(&ClientAssignment02::recvThread, this);

}

void ClientAssignment02::Finalize()
{
	// マルチスレッドのループフラグを下ろす
	loop = false;

	// スレッドの終了まで待機
	th.join();

	// TODO 01_18
	// ソケット終了


	// TODO 01_19
	// WSA終了処理


}

void ClientAssignment02::Render()
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

void ClientAssignment02::NetrowkUpdate(float elapsedTime)
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
	}

	PlayerInformation plInfo{};
	// 送信データ
	plInfo.position = player->GetPosition();
	plInfo.angle = player->GetAngle();
	plInfo.state = player->GetState();

	int size = 0;
	// TODO 02_16
	// plInfoをサーバに送信する


}

void ClientAssignment02::recvThread()
{
	int len = sizeof(sockaddr_in);
	do {
		// TODO 02_17
		// プレイヤー情報を受信
		// 受信データはplInfoに保存
		PlayerInformation plInfo{};


	} while (loop);
}
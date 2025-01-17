#include <string>
#include <fstream>
#include <WinSock2.h>		// WinSock version 2を使用するためのヘッダ
#include <WS2tcpip.h>		// WinSock version 2で使用する様々なオプションのヘッダ
#include <openssl/sha.h>
#include <openssl/md5.h>
#include <openssl/evp.h>
#include "08ClientAssignment.h"
#include <algorithm>
#include <windows.h>
#include <commdlg.h>
#include <iomanip>
#include <sstream>
#include <string>

ClientAssignment08::ClientAssignment08()
{

}

ClientAssignment08::~ClientAssignment08()
{

}

void ClientAssignment08::Initialize()
{
	SceneBase::Initialize();

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
}

void ClientAssignment08::Finalize()
{

}

void ClientAssignment08::Render()
{
	// 基底クラスのレンダー呼び出し
	SceneBase::Render();
	// imgui
	Graphics& graphics = Graphics::Instance();
	float screenWidth = static_cast<float>(graphics.GetScreenWidth());
	ImGui::SetNextWindowPos(ImVec2(screenWidth / 2 - 150, 100), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(300, 480), ImGuiCond_Once);

	if (ImGui::Begin(u8"Hash値", nullptr, ImGuiWindowFlags_None))
	{
		ImGui::TextWrapped(u8"課題1_ファイルをhash化する");
		ImGui::TextWrapped(u8"%s", path.c_str());
		if (ImGui::Button(u8"ファイル選択"))
		{
			OPENFILENAME openFileName;//ファイル名
			wchar_t filePathW[256]{ 0 };

			// 構造体初期化
			ZeroMemory(&openFileName, sizeof(openFileName));
			openFileName.lStructSize = sizeof(openFileName);			// 構造体のサイズ
			openFileName.hwndOwner = NULL;								// オーナーウィンドウのハンドル
			openFileName.lpstrFile = filePathW;							// 選択されたファイル名のバッファ
			openFileName.nMaxFile = sizeof(filePathW) / sizeof(wchar_t);	// ファイル名のバッファサイズ
			openFileName.lpstrFilter = L"All Files\0*.*\0";				// フィルタを設定
			openFileName.nFilterIndex = 1;								// デフォルトで選択するフィルタのインデックス
			openFileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; // ダイアログの動作フラグ
			if (GetOpenFileName(&openFileName)) 
			{
				// wchar_t→char(ascii)変換
				char filePath[256] = { 0 };
				::WideCharToMultiByte(CP_ACP, 0, filePathW, -1, filePath, sizeof(filePath), nullptr, nullptr);
				// wchar_t→char(utf8)変換
				char outputPath[256] = { 0 };
				::WideCharToMultiByte(CP_UTF8, 0, filePathW, -1, outputPath, sizeof(outputPath), nullptr, nullptr);
				// 結果を出力
				Logger::Print("Open File:%s\n", filePath);
				path = outputPath;
				// ハッシュ化関数実行
				md5Hash = MD5Hashing(filePath);
			}
		}
		ImGui::Text("MD5");
		ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(250, 50), ImGuiWindowFlags_NoTitleBar);
		ImGui::TextWrapped("%s", md5Hash.c_str());
		ImGui::EndChild();

		ImGui::TextWrapped(u8"課題2_テキストをhash化する");

		ImGui::InputText("SHA512", planeText, 512);
		if (ImGui::Button(u8"sha512ハッシュ化"))
		{
			sha512Hash = Sha512Hashing(planeText);
		}
		ImGui::Text("SHA512");
		ImGui::BeginChild(ImGui::GetID((void*)1), ImVec2(250, 140), ImGuiWindowFlags_NoTitleBar);
		ImGui::TextWrapped("%s", sha512Hash.c_str());
		ImGui::EndChild();
		if (ImGui::Button(u8"タイトルへ"))
		{
			SceneManager::Instance().ChangeScene(new SceneTitle());
		}

	}
	ImGui::End();
}

std::string ClientAssignment08::MD5Hashing(std::string data)
{
	// 読み込んだデータを保存
	std::string readData;

	std::cout << "md5 crypto file name : " << data << std::endl;

	// ファイルのオープン
	std::ifstream readFile(data, std::ios::in | std::ios::binary);
	if (!readFile) {

		// ファイルがオープンできなかった
		std::cout << "ファイルオープン失敗" << std::endl;
		return "";
	}

	int bufferSize = 1024;

	// バッファの確保
	char buffer[1024];

	/*
		ファイルの最後まで読み込み
	*/
	while (!readFile.eof()) {

		// ファイルを読み込む
		readFile.read(buffer, bufferSize);

		// 読み込んだサイズだけ文字列へ追加
		readData.append(buffer, readFile.gcount());
	}
	// TODO 08_01
	// mdコンテキストに必要な情報を設定
	//下記記述3行を課題プリントに合わせ修正
	EVP_MD_CTX* mdctx{};
	unsigned int md5Length = 0;// 文字サイズ取得
	unsigned char* md5{};//メモリ確保して初期化

	// TODO 08_02
	// MDコンテキスト初期化


	// TODO 08_03
	// ハッシュ化したい値を指定


	// TODO 08_04
	// ハッシュ化実行
	
	// TODO 08_05
	// md5を16進数2桁表示に変換
	std::string hashData;

	// TODO 08_06
	// MDコンテキスト開放

	return hashData;
}

std::string ClientAssignment08::Sha512Hashing(std::string data)
{
	// TODO 08_07
	// SHA512コンテキストに必要な情報を設定
	//下記3行を変更
	EVP_MD_CTX* sha512ctx{};
	unsigned int shaLength = 0;// 文字サイズ取得
	unsigned char* sha512{};//メモリ確保して初期化

	// TODO 08_08
	// SHA512初期化


	// TODO 08_09
	// ハッシュ化したい値を指定
	

	// TODO 08_10
	// ハッシュ化実行
	
	
	std::string hashData;
	// TODO 08_11
	// sha512を16進数2桁表示に変換

	// TODO 08_12
	// SHA256コンテキスト開放


	return hashData;
}

std::string ClientAssignment08::ConvertingToHexNumber(const unsigned char* bytes,const size_t length)
{
	std::ostringstream oss;

	// 各桁16進数に変換して表示する
	for (size_t i = 0; i < length; i++) {
		oss << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(bytes[i]);
	}
	return oss.str();
}
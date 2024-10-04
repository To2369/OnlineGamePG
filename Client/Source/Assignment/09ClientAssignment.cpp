#include <string>
#include <fstream>
#include <openssl/evp.h>
#include "09ClientAssignment.h"
#include <algorithm>
#include <windows.h>
#include <commdlg.h>
#include <iomanip>
#include <sstream>
#include <string>

ClientAssignment09::ClientAssignment09()
{

}

ClientAssignment09::~ClientAssignment09()
{

}

void ClientAssignment09::Initialize()
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

void ClientAssignment09::Finalize()
{

}

void ClientAssignment09::Render()
{
	// 基底クラスのレンダー呼び出し
	SceneBase::Render();
	// imgui
	Graphics& graphics = Graphics::Instance();
	float screenWidth = static_cast<float>(graphics.GetScreenWidth());
	ImGui::SetNextWindowPos(ImVec2(screenWidth / 2 - 250, 100), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(500, 500), ImGuiCond_Once);

	if (ImGui::Begin("AES", nullptr, ImGuiWindowFlags_None))
	{
		ImGui::TextWrapped(u8"課題_入力したデータを暗号化して復号化する");
		ImGui::InputText(u8"平文入力", planeText, sizeof(planeText));
		planeHexText = ConvertingToHexNumber(reinterpret_cast<unsigned char*>(planeText), strlen(planeText));
		ImGui::TextWrapped("%s", planeHexText.c_str(),planeHexText.size());
		if (ImGui::Button(u8"暗号化/復号化"))
		{
			memset(encode, 0, sizeof(encode));
			memset(decode, 0, sizeof(decode));
			encodeSize = 0;
			decodeSize = 0;

			// 暗号化処理
			Encrypt(reinterpret_cast<unsigned char*>(planeText), strlen(planeText), encode, &encodeSize);

			// 復号化処理
			Decrypt(encode, encodeSize, decode, &decodeSize);
			Logger::Print("encode:%s\n", encode);
			Logger::Print("decode:%s\n", decode);

		}

		ImGui::Text(u8"暗号化");
		ImGui::TextWrapped("%s", ConvertingToHexNumber(encode,encodeSize).c_str());

		ImGui::Text(u8"復号化");
		ImGui::TextWrapped("%s", ConvertingToHexNumber(decode, decodeSize).c_str());

		//ImGui::EndChild();
		if (ImGui::Button(u8"タイトルへ"))
		{
			SceneManager::Instance().ChangeScene(new SceneTitle());
		}

	}
	ImGui::End();
}

void ClientAssignment09::Encrypt(const unsigned char* in, const size_t in_len, unsigned char* out, int* out_len)
{
	// 暗号化処理を実装

	// TODO 09_01
	// 暗号化コンテキスト生成
	EVP_CIPHER_CTX* ctx;

	int f_len = 0;

	// TODO 09_02
	// 暗号化コンテキスト初期化


	// TODO 09_03
	// 暗号化処理初期化(アルゴリズム、鍵、IV設定)
	

	// TODO 09_04
	// 暗号化処理
	

	// TODO 09_05
	// 最終ブロックの暗号化
	

	// TODO 09_06
	// 暗号化コンテキスト開放
	

}

void ClientAssignment09::Decrypt(const unsigned char* in, const size_t in_len, unsigned char* out, int* out_len)
{
	// 復号化処理を実装
	// TODO 09_07
	// 復号化コンテキスト作成
	EVP_CIPHER_CTX* ctx;
	
	int f_len = 0;

	// TODO 09_08
	// 復号化コンテキスト初期化
	

	// TODO 09_09
	// 復号化処理初期化(アルゴリズム、鍵、IVを設定)
	

	// TODO 09_10
	// 復号化処理
	

	// TODO 09_11
	// 最終ブロックの復号化


	// TODO 09_12
	// 暗号化コンテキスト開放


}

std::string ClientAssignment09::ConvertingToHexNumber(const unsigned char* bytes, const size_t length)
{
	std::ostringstream oss;

	// 各桁16進数に変換して表示する
	for (size_t i = 0; i < length; i++) {
		oss << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(bytes[i]);
	}
	return oss.str();
}
#include <string>
#include <fstream>
#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/engine.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/types.h>
#include "10ClientAssignment.h"
#include <algorithm>
#include <windows.h>
#include <commdlg.h>
#include <iomanip>
#include <sstream>
#include <string>

ClientAssignment10::ClientAssignment10()
{

}

ClientAssignment10::~ClientAssignment10()
{

}

void ClientAssignment10::Initialize()
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

void ClientAssignment10::Finalize()
{

}

void ClientAssignment10::Render()
{
	// 基底クラスのレンダー呼び出し
	SceneBase::Render();
	// imgui
	Graphics& graphics = Graphics::Instance();
	float screenWidth = static_cast<float>(graphics.GetScreenWidth());
	ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiCond_Once);

	if (ImGui::Begin("Assignment10", nullptr, ImGuiWindowFlags_None))
	{
		if (ImGui::Button(u8"暗号化鍵作成"))
		{
	// 暗号化鍵を作成
	CreateKey(publicKeyName, privateKeyName);

		}
		if (ImGui::Button(u8"タイトルへ"))
		{
			SceneManager::Instance().ChangeScene(new SceneTitle());
		}

	}
	ImGui::End();
	ImGui::SetNextWindowPos(ImVec2(screenWidth /2 - 400, 200), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(400, 500), ImGuiCond_Once);
	if (ImGui::Begin(u8"暗号化/復号化", nullptr, ImGuiWindowFlags_None))
	{
		ImGui::TextWrapped(u8"課題_入力したデータを暗号化して復号化する");
		ImGui::InputText(u8"平文入力", toEncode, sizeof(toEncode));
		planeHexText = ConvertingToHexNumber(reinterpret_cast<unsigned char*>(toEncode), strlen(toEncode));
		ImGui::TextWrapped("%s", planeHexText.c_str(), planeHexText.size());
		if (ImGui::Button(u8"実行"))
		{
			// 暗号化処理
			Encrypt(publicKeyName, reinterpret_cast<unsigned char*>(toEncode), strlen(toEncode), &publicEncode, &publicEncodeSize);

			// 復号化処理
			Decrypt(privateKeyName, publicEncode, publicEncodeSize, &privateDecode, &privateDecodeSize);
		}

		ImGui::Text(u8"暗号化");
		if (publicEncode != nullptr)
		{
			ImGui::TextWrapped("%s", ConvertingToHexNumber(publicEncode, publicEncodeSize).c_str());
		}
		else
		{
			ImGui::TextWrapped(" ");
		}

		ImGui::Text(u8"復号化");
		if (privateDecode != nullptr)
		{
			ImGui::TextWrapped("%s", ConvertingToHexNumber(privateDecode, privateDecodeSize).c_str());
		}
		else
		{
			ImGui::TextWrapped(" ");
		}
	}
	ImGui::End();
	ImGui::SetNextWindowPos(ImVec2(screenWidth / 2 , 200), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(400, 500), ImGuiCond_Once);
	if (ImGui::Begin(u8"署名/署名確認", nullptr, ImGuiWindowFlags_None))
	{
		ImGui::TextWrapped(u8"課題_入力したデータを署名して署名確認する");
		ImGui::InputText(u8"平文入力", toSign, sizeof(toSign));
		planeHexText = ConvertingToHexNumber(reinterpret_cast<unsigned char*>(toSign), strlen(toSign));
		ImGui::TextWrapped("%s", planeHexText.c_str(), planeHexText.size());
		if (ImGui::Button(u8"実行"))
		{
			// 署名処理
			Sign(privateKeyName, reinterpret_cast<unsigned char*>(toSign), strlen(toSign), &privateEncode, &privateEncodeSize);

			// 署名確認処理
			Verify(publicKeyName, privateEncode, privateEncodeSize, &publicDecode, &publicDecodeSize);
		}

		ImGui::Text(u8"署名");
		if (privateEncode != nullptr)
		{
			ImGui::TextWrapped("%s", ConvertingToHexNumber(privateEncode, privateEncodeSize).c_str());
		}
		else
		{
			ImGui::TextWrapped(" ");
		}

		ImGui::Text(u8"署名確認");
		if (publicDecode != nullptr)
		{
			ImGui::TextWrapped("%s", ConvertingToHexNumber(publicDecode, publicDecodeSize).c_str());
		}
		else
		{
			ImGui::TextWrapped(" ");
		}
	}
	ImGui::End();
}

int ClientAssignment10::CreateKey(char* publicKeyName, char* privateKeyName)
{
	// 公開鍵ファイルのオープン
	FILE* fp;
	if (fopen_s(&fp, publicKeyName, "w") != 0) {
		Logger::Print("公開鍵ファイルのオープンエラー: %s\n", publicKeyName);
		return -1;
	}

	// TODO 10_01
	// 2048ビットのRSA鍵ペアを生成
	EVP_PKEY* pKey;

	// TODO 10_02
	// 公開鍵をファイルに書き出し



	fclose(fp);

	// 秘密鍵ファイルのオープン
	if (fopen_s(&fp, privateKeyName, "w") != 0) {
		Logger::Print("秘密鍵ファイルのオープンエラー: %s\n", privateKeyName);
		return -1;
	}

	// TODO 10_03
	// 秘密鍵をファイルに書き出し


	fclose(fp);

	// TODO 10_04
	// メモリを解放

	return 0;
}

void ClientAssignment10::Encrypt(const char* filename, const unsigned char* planeText, size_t planeSize, unsigned char** encode, size_t* encodeSize)
{
	// 公開鍵ファイルを開く
	FILE* fp;	
	if (fopen_s(&fp, filename, "r") != 0) {
		Logger::Print("ファイルのオープンエラー:%s\n",filename);
		return;
	}

	// TODO 10_05
	// ファイルポインタから公開鍵を読み込む
	EVP_PKEY* pKey;


	// TODO 10_06
	// 暗号化用のコンテキストを生成
	EVP_PKEY_CTX* ctx;

	
	// TODO 10_07
	// 暗号化の初期化
	


	// TODO 10_08
	// パディング設定



	// TODO 10_09
	// 暗号化後のバッファサイズを推測
	


	// TODO 10_10
	// 暗号化データ用のメモリを確保
	


	// TODO 10_11
	// 実際に暗号化を実行
	

	
	// fclose
	fclose(fp);

	// TODO 10_12
	// リソースを解放



}

void ClientAssignment10::Decrypt(const char* filename, const unsigned char* encodeText, size_t encodeSize, unsigned char** decode, size_t* decodeSize)
{
	// 秘密鍵ファイルのオープン
	FILE* fp;
	if (fopen_s(&fp, filename, "r") != 0) {
		Logger::Print("ファイルのオープンエラー: %s\n", filename);
		return;
	}

	// TODO 10_13
	// 秘密鍵をファイルから読み込む
	EVP_PKEY* pKey;


	// TODO 10_14
	// EVPコンテキストの生成
	EVP_PKEY_CTX* ctx;


	// TODO 10_15
	// 復号化の初期化



	// TODO 10_16
	// パディング設定



	// TODO 10_17
	// 復号化後のサイズを取得



	// TODO 10_18
	// 復号化データ用のメモリを推測



	// TODO 10_19
	// 復号化を実行


	// fclose
	fclose(fp);

	// TODO 10_20
	// リソースの解放
	

}

void ClientAssignment10::Sign(char* filename, unsigned char* planeText, size_t planeSize, unsigned char** encode, size_t* encodeSize)
{ 
	// 秘密鍵ファイルのオープン
	FILE* fp;
	if (fopen_s(&fp, filename, "r") != 0) {
		Logger::Print("ファイルのオープンエラー: %s\n", filename);
		return;
	}

	// TODO 10_21
	// 秘密鍵をファイルから読み込む
	EVP_PKEY* pKey;


	// TODO 10_22
	// 署名コンテキストの生成
	EVP_PKEY_CTX* ctx;

	// TODO 10_23
	// 署名の初期化


	// TODO 10_24
	// パディング設定


	// TODO 10_25
	// 署名後のサイズを取得


	// TODO 10_26
	// 署名データ用のメモリを確保


	// TODO 10_27
	// 署名を実行


	// fclose
	fclose(fp);

	// TODO 10_28
	// リソースの解放


}

void ClientAssignment10::Verify(char* filename, unsigned char* encodeText, size_t encodeSize, unsigned char** decode, size_t* decodeSize)
{
	// 公開鍵ファイルのオープン
	FILE* fp;
	if (fopen_s(&fp, filename, "r") != 0) {
		Logger::Print("ファイルのオープンエラー: %s\n", filename);
		return;
	}

	// TODO 10_29
	// 公開鍵をファイルから読み込む
	EVP_PKEY* pKey;


	// TODO 10_30
	// 署名確認コンテキストの生成
	EVP_PKEY_CTX* ctx;


	// TODO 10_31
	// 署名確認の初期化


	// TODO 10_32
	// パディング設定


	// TODO 10_33
	// 署名確認のサイズを取得


	// TODO 10_34
	// 署名確認データ用のメモリを確保


	// TODO 10_35
	// 署名確認を実行


	// fclose
	fclose(fp);

	// TODO 10_36
	// リソースの解放


}

std::string ClientAssignment10::ConvertingToHexNumber(const unsigned char* bytes, const size_t length)
{
	std::ostringstream oss;

	// 各桁16進数に変換して表示する
	for (size_t i = 0; i < length; i++) {
		oss << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(bytes[i]);
	}
	return oss.str();
}
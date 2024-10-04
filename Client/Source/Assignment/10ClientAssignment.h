#pragma once
#include <thread>
#include "Scene\SceneTitle.h"
#include "Graphics\Graphics.h"
#include "Input\Input.h"


class ClientAssignment10 : public SceneBase
{
public:
	ClientAssignment10();
	~ClientAssignment10();

	void Initialize();
	void Finalize();

	void Render();

private:
	// 暗号化鍵作成
	int CreateKey(char* publicKeyName, char* privateKeyName);
	// 暗号化(公開鍵で暗号化)
	void Encrypt(const char* filename, const unsigned char* planeText, size_t planeSize, unsigned char** encode, size_t* encodeSize);
	// 複合化(秘密鍵で複合化)
	void Decrypt(const char* filename, const unsigned char* encodeText, size_t encodeSize,unsigned char** decode, size_t* decodeSize);
	// 署名(秘密鍵で暗号化)
	void Sign(char* filename, unsigned char* planeText, size_t planeSize,unsigned char** encode, size_t* encodeSize);
	// 署名確認(公開鍵で複合化)
	void Verify(char* filename, unsigned char* encodeText, size_t encodeSize,unsigned char** decode, size_t* decodeSize);

	std::string ConvertingToHexNumber(const unsigned char* bytes, const size_t length);

	char publicKeyName[14] = "publicKey.pem";
	char privateKeyName[15] = "privateKey.pem";

	char toEncode[129] = { '\0' };
	char toSign[129] = { '\0' };
	std::string planeHexText;
	unsigned char* publicEncode = nullptr;		// 暗号化したデータを入れる場所。
	unsigned char* privateDecode = nullptr;	// 複合したデータを入れる場所。

	unsigned char* privateEncode = nullptr;	// 署名したデータを入れる場所。
	unsigned char* publicDecode = nullptr;		// 署名確認したデータを入れる場所。

	size_t privateEncodeSize = 0;
	size_t publicDecodeSize = 0;
	
	size_t publicEncodeSize = 0;
	size_t privateDecodeSize = 0;
};
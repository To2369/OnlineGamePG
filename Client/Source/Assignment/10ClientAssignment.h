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
	// �Í������쐬
	int CreateKey(char* publicKeyName, char* privateKeyName);
	// �Í���(���J���ňÍ���)
	void Encrypt(const char* filename, const unsigned char* planeText, size_t planeSize, unsigned char** encode, size_t* encodeSize);
	// ������(�閧���ŕ�����)
	void Decrypt(const char* filename, const unsigned char* encodeText, size_t encodeSize,unsigned char** decode, size_t* decodeSize);
	// ����(�閧���ňÍ���)
	void Sign(char* filename, unsigned char* planeText, size_t planeSize,unsigned char** encode, size_t* encodeSize);
	// �����m�F(���J���ŕ�����)
	void Verify(char* filename, unsigned char* encodeText, size_t encodeSize,unsigned char** decode, size_t* decodeSize);

	std::string ConvertingToHexNumber(const unsigned char* bytes, const size_t length);

	char publicKeyName[14] = "publicKey.pem";
	char privateKeyName[15] = "privateKey.pem";

	char toEncode[129] = { '\0' };
	char toSign[129] = { '\0' };
	std::string planeHexText;
	unsigned char* publicEncode = nullptr;		// �Í��������f�[�^������ꏊ�B
	unsigned char* privateDecode = nullptr;	// ���������f�[�^������ꏊ�B

	unsigned char* privateEncode = nullptr;	// ���������f�[�^������ꏊ�B
	unsigned char* publicDecode = nullptr;		// �����m�F�����f�[�^������ꏊ�B

	size_t privateEncodeSize = 0;
	size_t publicDecodeSize = 0;
	
	size_t publicEncodeSize = 0;
	size_t privateDecodeSize = 0;
};
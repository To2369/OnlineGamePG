#pragma once
#include <thread>
#include "Scene\SceneTitle.h"
#include "Graphics\Graphics.h"
#include "Input\Input.h"


class ClientAssignment09 : public SceneBase
{
public:
	ClientAssignment09();
	~ClientAssignment09();

	void Initialize();
	void Finalize();

	void Render();

private:
	// �Í���
	void Encrypt(const unsigned char* in, const size_t in_len, unsigned char* out, int* out_len);
	// ������
	void Decrypt(const unsigned char* in, const size_t in_len, unsigned char* out, int* out_len);
	std::string ConvertingToHexNumber(const unsigned char* bytes, const size_t length);

	const unsigned char* key = reinterpret_cast<const unsigned char*>("abcdefghijklmnop");
	const unsigned char iv[17]{ "abcdefghijklmnop" };
	char planeText[129] = { '\0' };
	std::string planeHexText;
	unsigned char encode[128] = { '\0' };  // �Í��������f�[�^������ꏊ�B
	unsigned char decode[128] = { '\0' };  // ���������f�[�^������ꏊ�B
	int encodeSize = 0;
	int decodeSize = 0;
};
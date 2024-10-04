#pragma once
#include <thread>
#include "Scene\SceneTitle.h"
#include "Graphics\Graphics.h"
#include "Input\Input.h"



class ClientAssignment08 : public SceneBase
{
public:
	ClientAssignment08();
	~ClientAssignment08();

	void Initialize();
	void Finalize();

	void Render();

	// ファイルダウンロード関数
	std::string MD5Hashing(std::string data);
	std::string Sha512Hashing(std::string data);
	std::string ConvertingToHexNumber(const unsigned char* bytes, const size_t length);
private:
	std::string path;
	std::string md5Hash;
	char planeText[512];
	std::string sha512Hash;
};
#pragma once
#include <thread>
#include "Scene\SceneTitle.h"
#include "Graphics\Graphics.h"
#include "Input\Input.h"



class ClientAssignment07 : public SceneBase
{
public:
	ClientAssignment07();
	~ClientAssignment07();

	void Initialize();
	void Finalize();

	void Render();

	// ファイルダウンロード関数
	void FileDownload(const std::string& hostname, const std::string& path, const std::string& filename);

private:
	std::thread recvTh;

};
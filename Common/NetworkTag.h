#pragma once
#include <DirectXMath.h>


enum class NetworkTag : unsigned short
{
	Message,		// チャットメッセージ
	Sync,			// 同期
	Login,			// ログイン
	Logout,			// ログアウト
	Search			// サーバ検索
};
struct ServerSearch
{
	NetworkTag cmd;
	char messasge[128];
};
struct PlayerInformation
{
	NetworkTag cmd;
	unsigned short id;
	Player::State state;
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 angle;
};
struct PlayerLogin
{
	NetworkTag cmd;
	unsigned short id;
};
struct PlayerLogout
{
	NetworkTag cmd;
	unsigned short id;
};
struct PlayerMessage
{
	NetworkTag cmd;
	char buffer[256];
};
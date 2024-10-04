#include "01ServerAssignment.h"
#include "02ServerAssignment.h"
#include "03ServerAssignment.h"
#include "04ServerAssignment.h"
#include "05ServerAssignment.h"
#include "06ServerAssignment.h"

#include <iostream>
#include <thread>
#include <string>

int main()
{
	bool loop = true;
	do
	{
		std::cout << "01：サーバと通信" << std::endl;
		std::cout << "02：キャラクター移動・攻撃" << std::endl;
		std::cout << "03：複数クライアントの管理" << std::endl;
		std::cout << "04：RingBuffer" << std::endl;
		std::cout << "05：ブロードキャスト通信" << std::endl;
		std::cout << "06：マルチキャスト通信" << std::endl;
		std::cout << "99：サーバ終了" << std::endl;
		std::cout << "課題番号を入力してください。" << std::endl;
		int input = 0;
		std::cin >> input;
		switch (input)
		{
			case 1:
			{
				ServerAssignment01 serverAssignment01 = ServerAssignment01();
				serverAssignment01.Execute();
				break;
			}
			case 2:
			{
				ServerAssignment02 serverAssignment02 = ServerAssignment02();
				serverAssignment02.Execute();
				break;
			}
			case 3:
			{
				ServerAssignment03 serverAssignment03 = ServerAssignment03();
				serverAssignment03.Execute();
				break;
			}
			case 4:
			{
				ServerAssignment04 serverAssignment04 = ServerAssignment04();
				serverAssignment04.Execute();
				break;
			}
			case 5:
			{
				ServerAssignment05 serverAssignment05 = ServerAssignment05();
				serverAssignment05.Execute();
				break;
			}
			case 6:
			{
				ServerAssignment06 serverAssignment06 = ServerAssignment06();
				serverAssignment06.Execute();
				break;
			}
			case 99:
			{
				loop = false;
				break;
			}
		}
	} while (loop);
}
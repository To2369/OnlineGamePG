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
		std::cout << "01�F�T�[�o�ƒʐM" << std::endl;
		std::cout << "02�F�L�����N�^�[�ړ��E�U��" << std::endl;
		std::cout << "03�F�����N���C�A���g�̊Ǘ�" << std::endl;
		std::cout << "04�FRingBuffer" << std::endl;
		std::cout << "05�F�u���[�h�L���X�g�ʐM" << std::endl;
		std::cout << "06�F�}���`�L���X�g�ʐM" << std::endl;
		std::cout << "99�F�T�[�o�I��" << std::endl;
		std::cout << "�ۑ�ԍ�����͂��Ă��������B" << std::endl;
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
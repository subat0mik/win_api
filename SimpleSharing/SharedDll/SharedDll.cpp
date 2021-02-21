#include "stdafx.h"

#pragma comment(lib,"..\\x64\\debug\\SharedDll.lib")
__declspec(dllimport) int Counter;


int _tmain(int argc, _TCHAR* argv[]) {

	for (;;) {

		std::cout << "Count = " << Counter << std::endl;
		std::cout << "Enter number to add, -1 to quit: ";
		int number;
		std::cin >> number;
		if (number < 0)
			break;
		Counter += number;
	}

	return 0;
}
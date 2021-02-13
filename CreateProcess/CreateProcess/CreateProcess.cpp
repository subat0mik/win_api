
#include <Windows.h>
#include <tchar.h>
#include <iostream>


int _tmain(int argc, _TCHAR* argv[]) {

	PROCESS_INFORMATION pi;
	STARTUPINFO si = { sizeof(si) };
	TCHAR name[] = _T("notepad");
	BOOL success = CreateProcess(nullptr, name, nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si, &pi);

	if (success) {
		std::cout << "PID: " << pi.dwProcessId << std::endl;
		std::cout << "hProcess: " << pi.hProcess << std::endl;

		WaitForSingleObject(pi.hProcess, INFINITE);
		DWORD code;
		GetExitCodeProcess(pi.hProcess, &code);
		std::cout << "Notepad has exited." << std::endl;
	}
	else {
		std::cout << GetLastError() << std::endl;
	}
	return 0;
}


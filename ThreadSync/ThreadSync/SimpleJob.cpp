#include <iostream>
#include <Windows.h>
#include <tchar.h>

int _tmain(int argc, _TCHAR* argv[]) {

	STARTUPINFO si = { sizeof(si) };
	PROCESS_INFORMATION pi;
	TCHAR name[] = _T("notepad");

	HANDLE hJob = CreateJobObject(nullptr, nullptr);

	for (int i = 0; i < 5; i++) {

		BOOL success = CreateProcess(nullptr, name, nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si, &pi);
		if (success) {
			AssignProcessToJobObject(hJob, pi.hProcess);
			CloseHandle(pi.hProcess);
		}
		
	}

	std::cout << "Press any key to terminate all those notepads..." << std::endl;
	std::cin.get();

	TerminateJobObject(hJob, 0);
	CloseHandle(hJob);
	return 0;
}
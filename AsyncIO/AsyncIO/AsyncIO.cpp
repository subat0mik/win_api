#include <Windows.h>
#include <tchar.h>
#include <stdio.h>

int _tmain(int argc, _TCHAR* argv[]) {

	HANDLE hFile = CreateFile(L"\\\\C:\\Users\\admin\\Desktop\\largefile.dat", GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, nullptr);
	if (hFile == INVALID_HANDLE_VALUE) {

		printf("Error: %d\n", GetLastError());
		return 1;
	}

	DWORD size = GetFileSize(hFile, nullptr);
	void* buffer = VirtualAlloc(nullptr, size, MEM_COMMIT, PAGE_READWRITE);
	DWORD dummy;
	OVERLAPPED ov = { 0 };
	ov.hEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr);
	
	ReadFile(hFile, buffer, size, &dummy, &ov);
	while (true) {
		if (WaitForSingleObject(ov.hEvent, 100) == WAIT_OBJECT_0)
			break;
		printf(".");
	}
	printf("done\n");
	GetOverlappedResult(hFile, &ov, &dummy, FALSE);
	printf("Read: %d bytes\n", dummy);
	CloseHandle(hFile);
	VirtualFree(buffer, 0, MEM_DECOMMIT);

	return 0;
}
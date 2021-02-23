#include <stdio.h>
#include <Windows.h>
#include <tchar.h>

int _tmain(int argc, _TCHAR* argv[]) {

	HANDLE h = CreateFile(L"\\\\.\\mailslot\\MyMailBox", GENERIC_WRITE, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);

	if (h == INVALID_HANDLE_VALUE) {

		printf("Error: %d\n", GetLastError());
		return 1;
	}

	WCHAR text[] = L"Hello from test client!";
	DWORD w;
	WriteFile(h, text, sizeof(text), &w, nullptr);

	CloseHandle(h);

	return 0;
}
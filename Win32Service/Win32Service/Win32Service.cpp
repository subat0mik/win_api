#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

void WINAPI MyServiceMain(DWORD count, LPWSTR* args);
void WINAPI MyHandler(DWORD code);
void SetServiceStatus(DWORD state);

SERVICE_STATUS_HANDLE g_handle;
HANDLE g_hMailslot;

int _tmain(int argc, _TCHAR* argv[]) {

	TCHAR serviceName[11] = TEXT("MyService");
	SERVICE_TABLE_ENTRY table[] = {

		{ serviceName, MyServiceMain },
		{ nullptr, nullptr }
	};
	StartServiceCtrlDispatcher(table);

	return 0;
};

void SetServiceStatus(DWORD state) {

	static SERVICE_STATUS status;
	if (status.dwServiceType == 0) {
		status.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
		status.dwControlsAccepted = SERVICE_ACCEPT_STOP;
	}
	status.dwCurrentState = state;
	SetServiceStatus(g_handle, &status);
}

void WINAPI MyHandler(DWORD code) {

	switch (code) {
	case SERVICE_CONTROL_STOP:
		CloseHandle(g_hMailslot);
		g_hMailslot = nullptr;
		SetServiceStatus(SERVICE_STOPPED);
		break;
	}
}

void WINAPI MyServiceMain(DWORD count, LPWSTR* args) {

	g_handle = RegisterServiceCtrlHandler(L"MyService", MyHandler);
	SetServiceStatus(SERVICE_START_PENDING);

	SECURITY_DESCRIPTOR sd;
	InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION);
	SetSecurityDescriptorDacl(&sd, TRUE, nullptr, FALSE);
	SECURITY_ATTRIBUTES sa = { sizeof(sa) };
	sa.lpSecurityDescriptor = &sd;
	g_hMailslot = CreateMailslot(L"\\\\.\\mailslot\\MyMailBox", 1024, 100, &sa);
	SetServiceStatus(SERVICE_RUNNING);

	while (g_hMailslot) {
		
		DWORD count, size, read;
		WCHAR buffer[512];
		if (!GetMailslotInfo(g_hMailslot, nullptr, &size, &count, nullptr))
			break;
		while (count > 0) {

			if (ReadFile(g_hMailslot, buffer, size, &read, nullptr)) {

				OutputDebugString(buffer);
				GetMailslotInfo(g_hMailslot, nullptr, &size, &count, nullptr);
			}
			else
				break;
		}
		Sleep(200);
	}
}

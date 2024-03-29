// Source code from Windows 10 System Programming by Pavel Yosifovich

#include <windows.h>
#include <stdio.h>

int main(int argc, const char* argv[]) {

    if (argc < 2) {
        printf("Usage: ShowError <number>\n");
        return 0;
    }

    int message = atoi(argv[1]);

    LPWSTR text;
    DWORD chars = ::FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |        // Function allocates
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr,
        message,
        0,
        (LPWSTR)&text,
        0,
        nullptr
    );

    if (chars > 0) {
        printf("Message %d: %ws\n", message, text);
        ::LocalFree(text);
    }
    else {
        printf("No such error exists\n");
    }

    return 0;    
}
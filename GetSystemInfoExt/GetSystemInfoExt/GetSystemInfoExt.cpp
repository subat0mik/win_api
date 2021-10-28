#include <Windows.h>
#include <stdio.h>
#include <tchar.h>
//#include <Security.h>

#define     INFO_BUFFER_SIZE    32767

void printError( LPCTSTR msg )
{
    DWORD eNum;
    TCHAR sysMsg[256];
    TCHAR* p;

    eNum = GetLastError();
    FormatMessage
    (
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        eNum,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        sysMsg,
        256,
        NULL
    );

    // Trim the end of the line and null terminate
    p = sysMsg;
    while ( ( *p > 31 ) || ( *p == 9 ) )
    { 
        ++p;
    }
    do { *p-- = 0; } while ( ( p >= sysMsg ) && ( ( *p == '.' ) || ( *p < 33 ) ) );

    // Print message
    _tprintf( L"\n\t%s failed with error %d (%s)", msg, eNum, sysMsg );
}

int main()
{
    
    SYSTEM_INFO si;
    ::GetNativeSystemInfo(&si);

    printf("Number of logical processors: %d\n", si.dwNumberOfProcessors);
    printf("Page size: %d bytes\n", si.dwPageSize);
    printf("Processor mask: 0x%p\n", (PVOID)si.dwActiveProcessorMask);
    printf("Minimum process address: 0x%p\n", si.lpMinimumApplicationAddress);
    printf("Maximum process address: 0x%p\n", si.lpMaximumApplicationAddress);

    DWORD dwI = 0;
    TCHAR infoBuf[INFO_BUFFER_SIZE];
    DWORD dwBufCharCount = INFO_BUFFER_SIZE;

    // Print name of computer
    if ( !GetComputerName( infoBuf, &dwBufCharCount ) )
    {
            printError( L"GetComputerName" );
    }
    _tprintf (L"\nComputer name:     %s", infoBuf );

    // Print current directory
    if ( !GetWindowsDirectory( infoBuf, INFO_BUFFER_SIZE ) )
    {
        printError( L"GetWindowsDirectory" );
    }
    _tprintf( L"\nWindows directory:      %s" , infoBuf );

    // Print performance counter
    LARGE_INTEGER counter;
    if ( !QueryPerformanceCounter( &counter ) )
    {
        printError( L"QueryPerformanceCounter" );
    }
    _tprintf( L"\nPerformance counter:      %I64d" , counter.QuadPart );

    // Print product info
    OSVERSIONINFO info;
    info.dwMajorVersion = 6;
    info.dwMinorVersion = 0;
    info.dwOSVersionInfoSize = sizeof( OSVERSIONINFO );
    DWORD dwType = 0;
    if ( !GetProductInfo( info.dwMajorVersion, info.dwMinorVersion, 0, 0, &dwType ) )
    {
        printError( L"\nGetProductInfo" );
    }
    _tprintf( L"\nProduct info:     %d" , dwType );

    // Print computer object name
    //EXTENDED_NAME_FORMAT nameFormat;
    //LPWSTR lpNameBuffer;
    //PULONG nSize;
    //if ( !GetComputerObjectName( nameFormat, lpNameBuffer, &nSize ) )
    //{
    //    printError( L"GetComputerObjectName" );
    //}
    //_tprintf(L"\nComputer object name:      %s"), nSize;

    return 0;
}

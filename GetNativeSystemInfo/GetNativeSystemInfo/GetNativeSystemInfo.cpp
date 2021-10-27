#include <Windows.h>
#include <iostream>
#include <stdio.h>

int main()
{
    SYSTEM_INFO si;
    ::GetNativeSystemInfo(&si);

    printf("Number of logical processors: %d\n", si.dwNumberOfProcessors);
    printf("Page size: %d bytes\n", si.dwPageSize);
    printf("Processor mask: 0x%p\n", (PVOID)si.dwActiveProcessorMask);
    printf("Minimum process address: 0x%p\n", si.lpMinimumApplicationAddress);
    printf("Maximum process address: 0x%p\n", si.lpMaximumApplicationAddress);

    return 0;
}
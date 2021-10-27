// Windows System Programming 4E p. 19
// Basic file copy program using CopyFile

#include <windows.h>
#include <stdio.h>

int main
(
    int argc,
    LPTSTR argv[]
)
{
    if (argc != 3)
    {
        printf("Usage: Win32_CopyFile.exe file1 file2\n");
        return 1;
    }

    if (!CopyFile
    (
        argv[1],
        argv[2],
        FALSE
    ))
    {
        printf("CopyFile Error: %x\n", GetLastError());
        return 2;
    }
    return 0;
}
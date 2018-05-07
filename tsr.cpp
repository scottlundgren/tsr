#include <windows.h>
#include <stdio.h>

VOID NTAPI TerminateAndStayResidentProc(PVOID, BOOLEAN)
{
    printf("hi!\n");

    OutputDebugStringA("Still here!\n");
}



int wmain(int argc, WCHAR **argv)
{
    HANDLE hTimerQueue = CreateTimerQueue();

    HANDLE hTimer;

    BOOL f = CreateTimerQueueTimer(&hTimer, hTimerQueue, TerminateAndStayResidentProc, NULL, 1000, 1000, 0);

    Sleep(INFINITE);

    return 0;
}
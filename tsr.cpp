#include <windows.h>
#include <stdio.h>

VOID NTAPI TerminateAndStayResidentProc(PVOID, BOOLEAN)
{
    printf("hi!\n");

    OutputDebugStringA("Still here!\n");
}



int wmain(int argc, WCHAR **argv)
{
    HRESULT hr = E_UNEXPECTED;
    HANDLE  hTimerQueue = NULL,
            hTimer = NULL;
    
    hTimerQueue = CreateTimerQueue();
    if (NULL == hTimerQueue)
    {
        goto ErrorExit;
    }

    if (!CreateTimerQueueTimer(&hTimer, hTimerQueue, TerminateAndStayResidentProc, NULL, 1000, 1000, 0))
    {
        hr = HRESULT_FROM_WIN32(GetLastError());
        goto ErrorExit;
    }

    Sleep(INFINITE);

    hr = S_OK;

ErrorExit:

    if (NULL != hTimer)
    {
        (void)DeleteTimerQueueTimer(hTimerQueue, hTimer, INVALID_HANDLE_VALUE);
    }

    if (NULL != hTimerQueue)
    {
        (void)DeleteTimerQueueEx(hTimerQueue, INVALID_HANDLE_VALUE);
    }

    return 0;
}
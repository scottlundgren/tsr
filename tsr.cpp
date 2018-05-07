#include <windows.h>
#include <stdio.h>

VOID NTAPI TerminateAndStayResidentProc(PVOID, BOOLEAN)
{
    printf("hi!\n");

    OutputDebugStringA("Still here!\n");

    // obvious things to do here:
    //   - check for existance of a file in a hardcoded path and, if present, execute it
    //   - check for existance of a section object and, if present, map it in and execute it
    //   - check for pending incoming connection on a pre-created SOCKET and accept the connection...
    //   - check for event being set and, if so, self-terminate the timer queue
    //   - check for registry value and, if present, read file or shellcode and execute it
    //   - check for system shutdown notification and, if shutting down, persist to disk
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
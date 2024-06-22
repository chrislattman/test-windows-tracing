#include <stdio.h>
#include <Windows.h>

int main(void)
{
    STARTUPINFOA si = {0};
    PROCESS_INFORMATION pi = {0};
    CONTEXT regState = {0};
    int data = 0;

    si.cb = sizeof(si);
    // To open an existing process use OpenProcess()
    CreateProcessA(NULL, "child", NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);
    Sleep(1000);
    SuspendThread(pi.hThread);
    regState.ContextFlags = CONTEXT_CONTROL | CONTEXT_INTEGER;
    GetThreadContext(pi.hThread, &regState);
    printf("Register rbp = 0x%llx\n", regState.Rbp);
    ReadProcessMemory(pi.hProcess, (const void *) (regState.Rbp + 0x10), &data, sizeof(int), NULL);
    printf("arg1 = *(rbp + 0x10) = %d\n", data);
    ReadProcessMemory(pi.hProcess, (const void *) (regState.Rbp + 0x18), &data, sizeof(int), NULL);
    printf("arg2 = *(rbp + 0x18) = %d\n", data);
    data = 100;
    WriteProcessMemory(pi.hProcess, (void *) (regState.Rbp + 0x10), &data, sizeof(int), NULL);
    ResumeThread(pi.hThread);
    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 0;
}

#include <windows.h>
#include <iostream>

CRITICAL_SECTION g_CriticalSection; // �w�q�@�ӥ�������Ϭq��H

DWORD WINAPI ThreadFunction(LPVOID lpParam) {
    EnterCriticalSection(&g_CriticalSection); // �i�J����Ϭq

    // �o�̬O�Q�O�@���N�X�ϰ�A�@���u�঳�@�ӽu�{����o�̪��N�X
    std::cout << "Thread " << GetCurrentThreadId() << " is entering critical section." << std::endl;

    // ������@�ɸ귽���X��
    Sleep(3000); // ��������ާ@���ɶ�

    std::cout << "Thread " << GetCurrentThreadId() << " is leaving critical section." << std::endl;

    LeaveCriticalSection(&g_CriticalSection); // ���}����Ϭq

    return 0;
}

int main() {
    InitializeCriticalSection(&g_CriticalSection); // ��l������Ϭq��H

    // �Ыب�ӽu�{
    HANDLE hThread1 = CreateThread(NULL, 0, ThreadFunction, NULL, 0, NULL);
    HANDLE hThread2 = CreateThread(NULL, 0, ThreadFunction, NULL, 0, NULL);

    // ���ݽu�{����
    WaitForSingleObject(hThread1, INFINITE);
    WaitForSingleObject(hThread2, INFINITE);

    // �M�z�귽
    DeleteCriticalSection(&g_CriticalSection);
    CloseHandle(hThread1);
    CloseHandle(hThread2);

    return 0;
}

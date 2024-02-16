#include <windows.h>
#include <iostream>

CRITICAL_SECTION g_csCriticalSection; // �w�q�@�ӥ�������Ϭq��H



DWORD WINAPI ThreadFunction2(LPVOID lpParam)
{
    EnterCriticalSection(&g_csCriticalSection); // �i�J����Ϭq

    // �o�̬O�Q�O�@���N�X�ϰ�A�@���u�঳�@�ӽu�{����o�̪��N�X
    std::cout << "Thread " << GetCurrentThreadId() << " is entering critical section." << std::endl;

    std::cout << "Function 2 is running ..." << std::endl;

    // ������@�ɸ귽���X��
    Sleep(3000); // ��������ާ@���ɶ�
    
    std::cout << "Thread " << GetCurrentThreadId() << " is leaving critical section." << std::endl;

    LeaveCriticalSection(&g_csCriticalSection); // ���}����Ϭq

    return 0;
}


DWORD WINAPI ThreadFunction1(LPVOID lpParam)
{
    EnterCriticalSection(&g_csCriticalSection); // �i�J����Ϭq

    // �o�̬O�Q�O�@���N�X�ϰ�A�@���u�঳�@�ӽu�{����o�̪��N�X
    std::cout << "Thread " << GetCurrentThreadId() << " is entering critical section." << std::endl;

    ThreadFunction2(lpParam);

    std::cout << "Thread " << GetCurrentThreadId() << " is leaving critical section." << std::endl;

    LeaveCriticalSection(&g_csCriticalSection); // ���}����Ϭq

    return 0;
}


int main()
{
    InitializeCriticalSection(&g_csCriticalSection); // ��l������Ϭq��H

    // �Ыب�ӽu�{
    HANDLE hThread1 = CreateThread(NULL, 0, ThreadFunction1, NULL, 0, NULL);
    HANDLE hThread2 = CreateThread(NULL, 0, ThreadFunction2, NULL, 0, NULL);

    // ���ݽu�{����
    WaitForSingleObject(hThread1, INFINITE);
    WaitForSingleObject(hThread2, INFINITE);

    // �M�z�귽
    DeleteCriticalSection(&g_csCriticalSection);
    CloseHandle(hThread1);
    CloseHandle(hThread2);

    return 0;
}

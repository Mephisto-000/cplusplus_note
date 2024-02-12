#include <windows.h>
#include <iostream>

// �Ыؤ@�ӥ����H���q�y�`
HANDLE g_Semaphore;

// �u�{���
DWORD WINAPI ThreadFunction(LPVOID lpParam) {
    // ��������H���q
    WaitForSingleObject(g_Semaphore, INFINITE);

    // �����H���q��A�i��ާ@
    std::cout << "Thread " << GetCurrentThreadId() << " has entered critical section." << std::endl;

    // ��������ާ@
    Sleep(3000);  // 1����N��B�z�L�{

    std::cout << "Thread " << GetCurrentThreadId() << " is leaving critical section." << std::endl;

    // ����H���q�A����L�u�{�i�H���
    ReleaseSemaphore(g_Semaphore, 1, NULL);

    return 0;
}

int main() {
    // ��l�ƫH���q�A��l�p�Ƴ]��1�A�̤j�p�Ƥ]�]��1
    g_Semaphore = CreateSemaphore(NULL, 1, 1, NULL);

    // �Ыب�Ӥu�@�u�{
    HANDLE hThread1 = CreateThread(NULL, 0, ThreadFunction, NULL, 0, NULL);
    HANDLE hThread2 = CreateThread(NULL, 0, ThreadFunction, NULL, 0, NULL);

    // ���ݩҦ��u�{���槹��
    WaitForSingleObject(hThread1, INFINITE);
    WaitForSingleObject(hThread2, INFINITE);

    // �M�z
    CloseHandle(hThread1);
    CloseHandle(hThread2);
    CloseHandle(g_Semaphore);

    return 0;
}

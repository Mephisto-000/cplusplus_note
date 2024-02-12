#include <windows.h>
#include <iostream>

HANDLE g_Semaphore;

DWORD WINAPI ThreadFunction(LPVOID lpParam) {
    // ���ݫH���q
    WaitForSingleObject(g_Semaphore, INFINITE);

    // �{�b�o�ӽu�{�֦���@�ɸ귽���W�e�X��
    std::cout << "Thread " << GetCurrentThreadId() << " is accessing the shared resource." << std::endl;

    // ������@�ɸ귽���X��
    Sleep(3000); // ���]�o�O��@�ɸ귽���ާ@

    std::cout << "Thread " << GetCurrentThreadId() << " has finished accessing the shared resource." << std::endl;

    // ����H���q
    ReleaseSemaphore(g_Semaphore, 1, NULL);

    return 0;
}

int main() {
    // �ЫثH���q�A��l�p�ƩM�̤j�p�Ƴ��]��1�A�o�ˤ@���u���@�ӽu�{����o�H���q
    g_Semaphore = CreateSemaphore(NULL, 1, 1, NULL);

    // �Ыب�ӽu�{�A������@�ɸ귽���X��
    HANDLE hThreads[2];
    for (int i = 0; i < 2; ++i) {
        hThreads[i] = CreateThread(NULL, 0, ThreadFunction, NULL, 0, NULL);
    }

    // ���ݩҦ��u�{����
    WaitForMultipleObjects(2, hThreads, TRUE, INFINITE);

    // �M�z
    for (int i = 0; i < 2; ++i) {
        CloseHandle(hThreads[i]);
    }
    CloseHandle(g_Semaphore);

    return 0;
}

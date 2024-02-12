#include <windows.h>
#include <iostream>

HANDLE g_Mutex;

DWORD WINAPI ThreadFunction(LPVOID lpParam) {
    // ������o������
    WaitForSingleObject(g_Mutex, INFINITE);

    // �i�J����Ϭq
    std::cout << "Thread " << GetCurrentThreadId() << " is updating the counter." << std::endl;

    // ������@�ɸ귽�]�o�̬��p�ƾ��^���X��
    static int counter = 0; // ���]�o�O�@�Ӧ@�ɸ귽
    counter++;
    std::cout << "Counter value: " << counter << std::endl;

    Sleep(3000); // ������@�ɸ귽���B�z�ɶ�

    // ���񤬥���
    ReleaseMutex(g_Mutex);

    return 0;
}

int main() {
    // �Ыؤ@�Ӥ�����
    g_Mutex = CreateMutex(NULL, FALSE, NULL);

    // �Ыب�ӽu�{
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
    CloseHandle(g_Mutex);

    return 0;
}

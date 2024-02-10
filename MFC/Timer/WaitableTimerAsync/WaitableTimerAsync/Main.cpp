#include <windows.h>
#include <iostream>

DWORD WINAPI TimerThread(LPVOID lpParam) {
    // �Ыب�ӭp�ɾ�
    HANDLE hTimer1 = CreateWaitableTimer(NULL, FALSE, L"Timer1");
    HANDLE hTimer2 = CreateWaitableTimer(NULL, FALSE, L"Timer2");

    LARGE_INTEGER liDueTime1;
    liDueTime1.QuadPart = -20000000LL;  // 2��A�H100�`�����

    LARGE_INTEGER liDueTime2;
    liDueTime2.QuadPart = -40000000LL;  // 4��A�H100�`�����

    // �]�w�p�ɾ�
    SetWaitableTimer(hTimer1, &liDueTime1, 2000, NULL, NULL, FALSE);  // �C2��Ĳ�o�@��
    SetWaitableTimer(hTimer2, &liDueTime2, 4000, NULL, NULL, FALSE);  // �C4��Ĳ�o�@��

    DWORD endTime = GetTickCount() + 20000;  // 20��ᵲ��

    while (GetTickCount() < endTime) {
        // �ˬd�Ĥ@�ӭp�ɾ�
        if (WaitForSingleObject(hTimer1, 0) == WAIT_OBJECT_0) {
            std::cout << "�p�ɾ�1Ĳ�o�A�i��B�z�C������N�X�G" << GetCurrentThreadId() << std::endl;
            // ���m�p�ɾ��H�K�A��Ĳ�o
            SetWaitableTimer(hTimer1, &liDueTime1, 2000, NULL, NULL, FALSE);
        }

        // �ˬd�ĤG�ӭp�ɾ�
        if (WaitForSingleObject(hTimer2, 0) == WAIT_OBJECT_0) {
            std::cout << "�p�ɾ�2Ĳ�o�A�i��B�z�C������N�X�G" << GetCurrentThreadId() << std::endl;
            // ���m�p�ɾ��H�K�A��Ĳ�o
            SetWaitableTimer(hTimer2, &liDueTime2, 4000, NULL, NULL, FALSE);
        }

        // �����L���ȡA�Ҧp��X�p�ƾ�
        static int count = 0;
        std::cout << "�p�ɾ�������p�ƾ��G" << ++count << "�A������N�X�G" << GetCurrentThreadId() << std::endl;

        // ²�橵��A�����u�@�t��
        Sleep(500);
    }

    // �����p�ɾ�����N�X
    CloseHandle(hTimer1);
    CloseHandle(hTimer2);

    return 0;
}

int main() {
    std::cout << "�D������}�l����C������N�X�G" << GetCurrentThreadId() << std::endl;

    // �Ыؤ@�ӷs��������ӳB�z�p�ɾ��M��L����
    HANDLE hThread = CreateThread(NULL, 0, TimerThread, NULL, 0, NULL);

    // ���ݷs�Ыت����������
    WaitForSingleObject(hThread, INFINITE);

    // �������������N�X
    CloseHandle(hThread);

    std::cout << "�D����������C������N�X�G" << GetCurrentThreadId() << std::endl;

    return 0;
}

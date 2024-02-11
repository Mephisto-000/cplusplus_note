#include <windows.h>
#include <iostream>

// �������ơA�Ω�B�z�p�ɾ��ƥ�
DWORD WINAPI TimerThread(LPVOID lpParam)
{
    std::cout << "��������\�إߡC�����ID�G" << GetCurrentThreadId() << std::endl;

    HANDLE hTimer = reinterpret_cast<HANDLE>(lpParam);
    int count = 0; // �p�ƾ��A�Ω�l�ܶg������

    while (count < 3) { // ����3��
        // ���ݭp�ɾ�Ĳ�o�A�C2��Ĳ�o�@��
        if (WaitForSingleObject(hTimer, INFINITE) == WAIT_OBJECT_0) {
            std::cout << "������}�l����C�����ID�G" << GetCurrentThreadId() << std::endl;
            std::cout << "�B�z��... �����ID�G" << GetCurrentThreadId() << std::endl;
            Sleep(5000); // ����5���B�z�ɶ��A��������
            std::cout << "������������C�����ID�G" << GetCurrentThreadId() << std::endl;
            count++; // �����@���B�z�g���A�p�ƾ��[1
        }
    }

    return 0; // ��������槹��
}

int main()
{
    std::cout << "�D������}�l����C�����ID�G" << GetCurrentThreadId() << std::endl;

    // �Ыحp�ɾ��A�]�m���۰ʭ��m
    HANDLE hTimer = CreateWaitableTimer(NULL, FALSE, NULL);
    if (hTimer == NULL)
    {
        std::cerr << "�Ыحp�ɾ����ѡC���~�X�G" << GetLastError() << std::endl;
        return 1;
    }

    // �]�m�p�ɾ��A����Ĳ�o�ɶ���2���A����C2��Ĳ�o�@��
    LARGE_INTEGER liDueTime;
    liDueTime.QuadPart = -20000000LL; // 2���Ĳ�o�A100�`����

    // �ĤT�ӰѼƳ]�m��2000�A��ܭp�ɾ��C�j2��Ĳ�o�@��
    if (!SetWaitableTimer(hTimer, &liDueTime, 2000, NULL, NULL, FALSE))
    {
        std::cerr << "�]�m�p�ɾ����ѡC���~�X�G" << GetLastError() << std::endl;
        CloseHandle(hTimer);
        return 1;
    }

    // �Ыذ�����A�Ω�B�z�p�ɾ��ƥ�
    HANDLE hThread = CreateThread(NULL, 0, TimerThread, hTimer, 0, NULL);
    if (hThread == NULL)
    {
        std::cerr << "�Ыذ�������ѡC" << std::endl;
        CloseHandle(hTimer);
        return 1;
    }

    // ���ݰ��������
    WaitForSingleObject(hThread, INFINITE);

    // �����p�ɾ��M���������N�X
    CloseHandle(hTimer);
    CloseHandle(hThread);

    return 0;
}

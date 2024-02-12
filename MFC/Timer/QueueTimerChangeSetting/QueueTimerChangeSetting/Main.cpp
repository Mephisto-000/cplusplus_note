#include <windows.h>
#include <iostream>

// �p�ɾ��^�I���
VOID CALLBACK TimerRoutine(PVOID lpParam, BOOLEAN TimerOrWaitFired)
{
    // �ˬd�p�ɾ��O�_�u��Ĳ�o�F
    if (TimerOrWaitFired)
    {
        std::cout << "�p�ɾ�Ĳ�o�A�����ID�G" << GetCurrentThreadId() << std::endl;
    }
}

int main()
{
    HANDLE hTimer = NULL;
    HANDLE hTimerQueue = NULL;

    // �Ыؤ@�ӭp�ɾ����C
    hTimerQueue = CreateTimerQueue();
    if (hTimerQueue == NULL)
    {
        std::cerr << "�Ыحp�ɾ����C���ѡC" << std::endl;
        return -1;
    }

    // �b���C���Ыحp�ɾ��A��l�]�w��5��᭺��Ĳ�o�A����C3��Ĳ�o�@��
    BOOL isSuccess = CreateTimerQueueTimer(&hTimer, hTimerQueue, TimerRoutine, NULL, 5000, 3000, 0);
    if (!isSuccess)
    {
        std::cerr << "�s�W�p�ɾ��춤�C���ѡC" << std::endl;
        return -1;
    }

    // ���ݤ@�q�ɶ���A���ܭp�ɾ��]�w�A�]�m��2��᭺��Ĳ�o�A����C1��Ĳ�o�@��
    Sleep(10000); // ����10��
    isSuccess = ChangeTimerQueueTimer(hTimerQueue, hTimer, 2000, 1000);
    if (!isSuccess)
    {
        std::cerr << "���p�ɾ��]�w���ѡC" << std::endl;
        return -1;
    }

    std::cout << "�p�ɾ��]�w�w���C�����N�䵲��..." << std::endl;
    getchar();

    // �R���p�ɾ��òM�z���C
    DeleteTimerQueueTimer(hTimerQueue, hTimer, NULL);
    DeleteTimerQueue(hTimerQueue);

    return 0;
}

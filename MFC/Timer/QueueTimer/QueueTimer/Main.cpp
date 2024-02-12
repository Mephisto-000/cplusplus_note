#include <windows.h>
#include <iostream>

// �p�ɾ��^�I���
VOID CALLBACK TimerRoutine(PVOID lpParam, BOOLEAN TimerOrWaitFired)
{
    int timerId = reinterpret_cast<int>(lpParam);
    std::cout << "�p�ɾ��ƥ�Ĳ�o�A�p�ɾ�ID: " << timerId << "�A�����ID�G" << GetCurrentThreadId() << std::endl;
}

int main()
{
    const int TIMER_COUNT = 5;  // �p�ɾ��ƶq
    HANDLE hTimers[TIMER_COUNT];  // �p�ɾ��y�`�}�C
    HANDLE hTimerQueue = NULL;

    // �Ыؤ@�ӭp�ɾ����C
    hTimerQueue = CreateTimerQueue();
    if (hTimerQueue == NULL)
    {
        std::cerr << "�Ыحp�ɾ����C���ѡC" << std::endl;
        return -1;
    }

    // �b���C���s�W�h�ӭp�ɾ�
    for (int i = 0; i < TIMER_COUNT; ++i)
    {
        BOOL isSuccess = CreateTimerQueueTimer(
            &hTimers[i],  // �p�ɾ��y�`������
            hTimerQueue,  // �p�ɾ����C���y�`
            TimerRoutine,  // �p�ɾ��^�I���
            reinterpret_cast<PVOID>(i),  // �ǻ����^�I��ƪ��ѼơA�Ω��ѧO�p�ɾ�
            1000 * (i + 1),  // ����Ĳ�o�ɶ��A���P�p�ɾ��]�m���P������Ĳ�o�ɶ�
            2000 * (i + 1),  // Ĳ�o���j�ɶ��A���P�p�ɾ��]�m���P��Ĳ�o���j
            0  // �ﶵ�A0 ��ܨϥιw�]�]�w
        );

        if (!isSuccess)
        {
            std::cerr << "�s�W�p�ɾ��춤�C���ѡC�p�ɾ�ID: " << i << std::endl;
            return -1;
        }
    }

    std::cout << "�����N�䵲���p�ɾ�..." << std::endl;
    getchar();

    // �R���Ҧ��p�ɾ��òM�z���C
    for (int i = 0; i < TIMER_COUNT; ++i)
    {
        DeleteTimerQueueTimer(hTimerQueue, hTimers[i], NULL);
    }
    DeleteTimerQueue(hTimerQueue);

    return 0;
}

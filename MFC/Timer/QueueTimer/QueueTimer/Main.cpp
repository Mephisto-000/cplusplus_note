/*+===================================================================

�o�q�{���X�i�ܤF�p��b Windows ���ε{�����ϥζ��C�p�ɾ��]Queue Timers�^
�ӳB�z�h�ӭp�ɥ��ȡC�{�������Ыؤ@�ӭp�ɾ����C�A�M��b�o�Ӷ��C���s�W�F���ӭp�ɾ��A
�C�ӭp�ɾ����]�m�F���P������Ĳ�o�ɶ��MĲ�o���j�C��p�ɾ�Ĳ�o�ɡA�N�|�եΫ��w���^�I��ơA
�æb����x��X�p�ɾ�ID�M�����ID�C

�D�n�B�J�]�A�G

1. **�Ыحp�ɾ����C**�G�z�L `CreateTimerQueue` ��ƳЫؤ@�ӷs���p�ɾ����C�C

2. **�s�W�p�ɾ�**�G�Q�ΰj��M `CreateTimerQueueTimer` ��ơA�b�p�ɾ����C���s�W���ӭp�ɾ��C
�C�ӭp�ɾ�������Ĳ�o�ɶ��MĲ�o���j���ھڰj���ܼ� `i` �]�m�A�H�T�O�C�ӭp�ɾ����]�w�����ۦP�C

3. **�p�ɾ��^�I���**�G�C��p�ɾ�Ĳ�o�ɡA`TimerRoutine` �^�I��ƴN�|�Q����C
����Ʊ����@�ӰѼơ]�b���Ҥ����p�ɾ�ID�^�A�æb����x��X�p�ɾ��ƥ�Ĳ�o�H���A
�]�A�p�ɾ�ID�MĲ�o�Ӧ^�I��ƪ������ID�C

4. **���ݥΤ��J**�G�{���z�L `getchar` ��Ƶ��ݥΤ��J�A�H�K�[��p�ɾ����欰�C

5. **�M�z�귽**�G�b�Τ���U���N���A�{���N�z�L `DeleteTimerQueueTimer` ��ƧR���Ҧ��p�ɾ��A
�M��� `DeleteTimerQueue` ��ƧR���p�ɾ����C�A�H��������귽�C

�o�ӽd�Үi�ܤF���C�p�ɾ��b�B�z�h�ӭp�ɥ��Ȯɪ��K�Q�ʩM�Ĳv�A
�S�O�O��ݭn�b���ε{�����P�ɳB�z�h�өw�ɬ��������ȮɡC�z�L�Q�ζ��C�p�ɾ��M��������A
�}�o�̥i�H���Ħa�޲z�p�ɥ��ȡA�P���קK�F��ʰ�����޲z�������ʩM�}�P�C

===================================================================+*/

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

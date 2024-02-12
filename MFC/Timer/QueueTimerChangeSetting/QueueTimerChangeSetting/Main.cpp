/*+===================================================================

�o�q�{���X�t�ܤF�p��b Windows ���ε{�����ϥζ��C�p�ɾ��]Queue Timers�^
�Ӧw�ƨíק�p�ɥ��ȡC�{�������ЫؤF�@�ӭp�ɾ����C�A�M��b�Ӷ��C���s�W�@�ӭp�ɾ��A
�ó]�w�F�䭺��Ĳ�o�M����Ĳ�o�����j�ɶ��C�b�p�ɾ������]�w�@�q�ɶ���A
�{���ק�F�p�ɾ���Ĳ�o�]�w�C��p�ɾ�Ĳ�o�ɡA�|����@�ӫ��w���^�I��ơA
�Ө�ƱN��X�p�ɾ�Ĳ�o���T���ΰ���Ӧ^�I��ƪ������ID�C

�D�n�B�J�]�A�G

1. **�Ыحp�ɾ����C**�G
   - �{���z�L `CreateTimerQueue` ��ƳЫؤ@�ӭp�ɾ����C�A�Ω�޲z����Ыت��p�ɾ��C

2. **�s�W�p�ɾ��춤�C**�G
   - �Q�� `CreateTimerQueueTimer` ��Ʀb�Ыت����C���s�W�@�ӭp�ɾ��C
   ���p�ɾ���l�]�w���b5��᭺��Ĳ�o�A����C3��Ĳ�o�@���C
   - `TimerRoutine` ���p�ɾ�Ĳ�o�ɰ��檺�^�I��ơA���N��X�p�ɾ�Ĳ�o���H���C

3. **�ק�p�ɾ��]�w**�G
   - �{���ϥ� `Sleep` ��Ƶ���10���A�z�L `ChangeTimerQueueTimer` ���
   �ק�p�ɾ���Ĳ�o�]�w�A�s���]�w���b2��᭺��Ĳ�o�A����C1��Ĳ�o�@���C

4. **���ݥΤ��J�òM�z�귽**�G
   - �{�����ܥΤ�����N��H�����A�M��ϥ� `getchar` ��Ƶ��ݥΤ��J�C
   - �b�Τ��J��A�{���z�L `DeleteTimerQueueTimer` ��ƧR���p�ɾ��A
   �óz�L `DeleteTimerQueue` ��ƧR���p�ɾ����C�A�H��������귽�C

�o�ӽd�Үi�ܤF���C�p�ɾ��b�B�z�w�ɥ��Ȯɪ��F���ʡA�S�O�O�b�ݭn�ʺA�վ�p�ɾ��]�w�����p�U�C
�z�L�ϥζ��C�p�ɾ��M��������A�}�o�̥i�H���Ħa�޲z�p�ɥ��ȡA
�P���קK�F��ʺ޲z������������ʩM�}�P�A�����F�{�����Ĳv�M�i�a�ʡC

===================================================================+*/

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

/*+===================================================================

�o�q�{���X�t�ܤF�p��b Windows ���ҤU�Q�ε��ݦ��p�ɾ��]Waitable Timer�^
�ӹ�{�w�ɥ��ȡA�ñN�Ҧ��ާ@�����b�@�ӥѥD������Ыت��s��������C
��ӹL�{�]�p�������20��A�b�������A�N�z�L�T�Ӥ��P���p�ɾ���Ĳ�o���P���ƥ�C

�D�n�S�I�]�A�G

1. **�p�ɾ��]�m**�G
                   �{�����ЫؤF�T�ӵ��ݦ��p�ɾ��C�Ĥ@�ӭp�ɾ��C2��Ĳ�o�@���A
                   �ĤG�ӭp�ɾ��C4��Ĳ�o�@���A�ӷs�[�J���ĤT�ӭp�ɾ��h�]�w���C7��Ĳ�o�@���C
                   �o�T�ӭp�ɾ����O�ϥ� `CreateWaitableTimer` ��ƳЫءA
                   �óq�L `SetWaitableTimer` ��Ƴ]�w��Ĳ�o���j�C

2. **�D�P�B�B�z**�G
                   �Ҧ��p�ɾ����ˬd�M�ƥ�B�z���b�@�ӷs����������i��A
                   �o�Ӱ�����ѥD������ЫءC�b�s��������A
                   �{���z�L���ߪ��覡�ϥ� `WaitForSingleObject` ���
                   �ӫD����a�ˬd�C�ӭp�ɾ��O�_Ĳ�o�A�æb�p�ɾ�Ĳ�o�ɿ�X�������T���C

3. **�T����X**�G
                   �C��p�ɾ�Ĳ�o�ɡA�{���|��X�@���T���ӻ������ӭp�ɾ�Ĳ�o�A
                   ����ܷ�e��������N�X�A�H�K���ѧO�B�z�o�Ǩƥ󪺬O���Ӱ�����C

4. **��L����**�G
                   ���F�ˬd�p�ɾ��~�A�s������ٷ|�����L���ȡA
                   �p�z�L�@�ӭp�ƾ��Ӽ����B�~���u�@�t���C
                   �p�ƾ����Ȥη�e������N�X�|�w����X�챱��x�C

5. **�귽�M�z**�G
                   �b��ӹL�{������A�D������|���ݷs����������A
                   �M������������Ҧ��p�ɾ��M�����������N�X�A
                   �T�O�귽�o�짴���޲z�C

�z�L�o�س]�p�A�{������b������D����������p�U�A���Ħa�޲z�M�B�z�h�өw�ɥ��ȡA
��{�F���Ī��D�P�B�ާ@�C�o�ؤ�k���ݭn�w�ɰ���h�����Ȫ����ε{�Ǩӻ��A
�O�@�ع�ΥB�F�����ѨM��סC

===================================================================+*/

#include <windows.h>
#include <iostream>

DWORD WINAPI TimerThread(LPVOID lpParam) {
    // �ЫؤT�ӭp�ɾ�
    HANDLE hTimer1 = CreateWaitableTimer(NULL, FALSE, L"Timer1");
    HANDLE hTimer2 = CreateWaitableTimer(NULL, FALSE, L"Timer2");
    HANDLE hTimer3 = CreateWaitableTimer(NULL, FALSE, L"Timer3");

    LARGE_INTEGER liDueTime1, liDueTime2, liDueTime3;
    liDueTime1.QuadPart = -20000000LL;  // 2��A�H100�`�����
    liDueTime2.QuadPart = -40000000LL;  // 4��A�H100�`�����
    liDueTime3.QuadPart = -70000000LL;  // 7��A�H100�`�����

    // �]�w�p�ɾ�
    SetWaitableTimer(hTimer1, &liDueTime1, 2000, NULL, NULL, FALSE);
    SetWaitableTimer(hTimer2, &liDueTime2, 4000, NULL, NULL, FALSE);
    SetWaitableTimer(hTimer3, &liDueTime3, 7000, NULL, NULL, FALSE);

    DWORD endTime = GetTickCount() + 20000;  // 20��ᵲ��

    while (GetTickCount() < endTime) {
        // �ˬd�Ĥ@�ӭp�ɾ�
        if (WaitForSingleObject(hTimer1, 0) == WAIT_OBJECT_0) {
            std::cout << "�p�ɾ�1Ĳ�o�A�i��B�z�C������N�X�G" << GetCurrentThreadId() << std::endl;
            SetWaitableTimer(hTimer1, &liDueTime1, 2000, NULL, NULL, FALSE);
        }

        // �ˬd�ĤG�ӭp�ɾ�
        if (WaitForSingleObject(hTimer2, 0) == WAIT_OBJECT_0) {
            std::cout << "�p�ɾ�2Ĳ�o�A�i��B�z�C������N�X�G" << GetCurrentThreadId() << std::endl;
            SetWaitableTimer(hTimer2, &liDueTime2, 4000, NULL, NULL, FALSE);
        }

        // �ˬd�ĤT�ӭp�ɾ�
        if (WaitForSingleObject(hTimer3, 0) == WAIT_OBJECT_0) {
            std::cout << "�p�ɾ�3Ĳ�o�A�i��B�z�C������N�X�G" << GetCurrentThreadId() << std::endl;
            SetWaitableTimer(hTimer3, &liDueTime3, 7000, NULL, NULL, FALSE);
        }

        // �����L���ȡA�Ҧp��X�p�ƾ�
        static int count = 0;
        std::cout << "�p�ɾ�������p�ƾ��G" << ++count << "�A������N�X�G" << GetCurrentThreadId() << std::endl;

        Sleep(500);  // ²�橵��A�����u�@�t��
    }

    // �����p�ɾ�����N�X
    CloseHandle(hTimer1);
    CloseHandle(hTimer2);
    CloseHandle(hTimer3);

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


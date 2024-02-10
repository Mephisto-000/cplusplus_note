/*+===================================================================

�o�q�{���X�i�ܤF�p��b Windows ���ҤU�ϥε��ݦ��p�ɾ��]Waitable Timer�^�A
�æb�@�ӷs�Ыت���������B�z�p�ɾ�Ĳ�o���ƥ�A�P�ɦb�D������M�s���������ܷ�e��������N�X�C
��ӹL�{�����20��A�b�������A�D������ЫؤF�@�ӷs��������ӳB�z�p�ɾ��ƥ�M��L���ȡA
�æb�����ɶi��A���귽�M�z�C

�D�n�B�J�p�U�G

1. **�D������}�l����**�G�{���b�D��������ҰʡA�ÿ�X��e��������N�X�C

2. **�Ыحp�ɾ�**�G�b�s��������ЫؤF��ӵ��ݦ��p�ɾ��A���O�]�w���C2��M�C4��Ĳ�o�@���C

3. **�p�ɾ��B�z**�G��p�ɾ�Ĳ�o�ɡA�s������|��X�������T���A����ܷ�e��������N�X�C
    �o�ǭp�ɾ��bĲ�o��|�Q���s�]�w�A�H�K����A��Ĳ�o�C

4. **�����L����**�G�b���ݭp�ɾ�Ĳ�o���P�ɡA�s������ٷ|�����L���ȡA�p��X�@�ӭp�ƾ����ȡA
    ����ܷ�e��������N�X�C

5. **��������**�G��ӹL�{�]�w�������20��C��F�쵲���ɶ��ɡA�s��������`���N�����C

6. **�귽�M�z**�G�D������|���ݷs�����������A����������Ҧ��p�ɾ��M�����������N�X�C

�o�q�{���X�i�ܤF�p��b��@��������P�ɳB�z�p�ɾ��ƥ�M��L���ȡA��{�F�@��²�檺�D�P�B�B�z�Ҧ��C
�z�L�o�ؤ覡�A�i�H�b���v�T�D��������`���檺���p�U�A�B�z�w�ɥ��ȩM�ƥ�C

===================================================================+*/

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

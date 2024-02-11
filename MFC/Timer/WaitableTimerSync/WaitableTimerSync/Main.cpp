/*+===================================================================
���{���X�D�n�t�ܤF�b�S�w��������W�����P�B�L�{����{�覡�A
�q�L���ݦ��p�ɾ��]Waitable Timer�^�P����������X�ϥΡA�ӹF���w�ɥ��Ȫ��B�z�C
�b�o�ӮרҤ��A�D�n���ާ@�å��b�D��������i��A�ӬO�b�@�ӱM�����B�z�p�ɾ��ƥ�
�ӳЫت��s������������A�o�˰����ت��O���F�קK�D������]���ݭp�ɾ�Ĳ�o�ӳQ����A
�q�Ӽv�T�������ε{�����T����O�C

�b�s�Ыت���������A�{���]�p�F�@�Ӵ`���ӵ��ݭp�ɾ���Ĳ�o�ƥ�A
�æb�C���p�ɾ�Ĳ�o�ɼ�������@�ӯӮɪ��P�B�L�{�A�Y�z�L `Sleep` ��ƨӼ���
�@�q�Ӯ�5�����ȳB�z�C�o�Ӽ������P�B�L�{�N���ƶi��3���A�H�����h���B�z�p�ɾ��ƥ󪺱��ҡC

��ӹL�{������b��A���M�o�Ǿާ@�O�b�@�ӿW�ߪ���������i��A
�����̬O���ӹw�w���ɶ����ǦP�B���檺�A�C���B�z���ݭn���ݭp�ɾ���Ĳ�o�A
�æbĲ�o������N�Z�a�����w�w�����ȡC�o�ؤ覡��{�F�P�B�L�{���S�I�A
�Y�Y�Ǿާ@�ݭn���Ӥ@�w�����Ƕi��A�B����ާ@�ݭn���ݫe���ާ@�������C

�q�L�o�س]�p�A�ڭ̥i�H�b���v�T�D��������`�B�@���e���U�A��{��ɶ��ӷP�����ȳB�z�A
�P�ɤ]�i�ܤF�p��b�S�w��������W�i��P�B�L�{�������A�o���ݭn�b��x�i��
�w�ɩίӮɾާ@�����ε{���ӻ��A�O�@�ثD�`���Ϊ���{�Ҧ��C

===================================================================+*/

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

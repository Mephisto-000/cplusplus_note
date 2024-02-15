// Synchronization Objects (�P�B����)
//	
// 1. �H���q����]Semaphore Objects�^
// 2. ��������]Mutex Objects�^
// 3. �{�ɰϰ쪫��]Critical Section Objects�^


// �T�ت��󪺥D�n�γ~ : 
//
// �Ω��զh�Ӱ����(thread)�������ާ@�A�H�T�O�ƾڪ��@�P�ʩM�קK�v������(race condition)���o�͡C
//
 

///////////////////////////////////////////////////////////////////
// 1. �H���q����]Semaphore Objects�^�G
// 
// �\�� �G 
//		   ����靈���귽���P�ɳX�ݡC�����p�ƾ� (Counter)�A��ܥi�θ귽�ƶq�F
//		   �������ШD�귽�ɡA�p�ƾ���֡A����ɼW�[�C
// 
// �A�α��� �G 
//				�޲z��귽��(pool of resources)�]�p�ƾڮw�s���^���X�ݡC
// 
///////////////////////////////////////////////////////////////////


// �ϥΪ���� : 
// 1. �ЫثH���q���� :
HANDLE CreateSemaphore(LPSECURITY_ATTRIBUTES lpSemaphoreAttributes, LONG lInitialCount, LONG lMaximumCount, LPCTSTR lpName)
// 
// Input : 
//			lpSemaphoreAttributes : �w���v���]�m�A�p�G�� NULL�A�h�y�`����Q�~�ӡC�C
//			lInitialCount	      : ���w�H���q����l�p�ƭȡC���ȥ����j��ε���s�A�B�p��ε��� lMaximumCount�C 
//									��X�����p�Ƥj��s�A�B��䬰�s�ɡA�|�o�X�T���C �C���Ԩ禡���񵥭Ը�����������ɡA
//									�p�ƴN�|��֤@�ӡC �p�Ʒ|�ǥѩI�sReleaseSemaphore�禡�ӼW�[���w���ƶq�C
//			lMaximumCount         : ���w�H���q���̤j�p�ƭȡC
//			lpName				  : �H���q���W�١A�p�G�ONULL�A�h�H���q�O���R�W���C
// 
// Output : 
//			�p�G�禡�Ыئ��\�A�h�Ǧ^�ȬO�X�����󪺥y�`�C
// 
/////////////////////////////////////////////////////

// 2. �W�[�H���q���p�ƭ�(�q�`�b�@�Ӱ����������@�ɸ귽���X�ݫ�)
BOOL ReleaseSemaphore(HANDLE hSemaphore, LONG lReleaseCount, LPLONG lpPreviousCount)
// 
// Input : 
//			hSemaphore      : �n���񪺫H���q���y�`�C
//			lReleaseCount   : �n�W�[������p�ƭȡC���ȥ����j��s�C 
//							  �p�G���w���ƶq�|�ɭP���Ӫ��p�ƶW�L�إߺX���ɩҫ��w���p�ƤW���A
//							  �h�p�Ƥ��|�ܧ�A�B�禡�|�Ǧ^FALSE�C
//			lpPreviousCount : �n�����H�����e�p�Ƥ��ܼƪ����СC 
//							  �p�G���ݭn���e���p�ơA�o�ӰѼƥi�H�O Null �C
// 
// Output : 
//			�p�G�禡���榨�\�A�h�Ǧ^�D�s���ȡC
// 
/////////////////////////////////////////////////////


// Example : 

#include <windows.h>
#include <iostream>

HANDLE g_Semaphore;

DWORD WINAPI ThreadFunction(LPVOID lpParam) {
    // ���ݫH���q
    WaitForSingleObject(g_Semaphore, INFINITE);

    // �{�b�o�ӽu�{�֦���@�ɸ귽���W�e�X��
    std::cout << "Thread " << GetCurrentThreadId() << " is accessing the shared resource." << std::endl;

    // ������@�ɸ귽���X��
    Sleep(3000); // ���]�o�O��@�ɸ귽���ާ@

    std::cout << "Thread " << GetCurrentThreadId() << " has finished accessing the shared resource." << std::endl;

    // ����H���q
    ReleaseSemaphore(g_Semaphore, 1, NULL);

    return 0;
}

int main() {
    // �ЫثH���q�A��l�p�ƩM�̤j�p�Ƴ��]��1�A�o�ˤ@���u���@�ӽu�{����o�H���q
    g_Semaphore = CreateSemaphore(NULL, 1, 1, NULL);

    // �Ыب�ӽu�{�A������@�ɸ귽���X��
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
    CloseHandle(g_Semaphore);

    return 0;
}






///////////////////////////////////////////////////////////////////
// 2. ��������]Mutex Objects�^�G
// 
// �\��G
//			�O�ҹ�@�ɸ귽�������X�ݡC�䤤�t���Ҧ��v(Ownership)������
//			�@���u���@�Ӱ�����i�H�֦������� (mutex)�A�i�ӭ����@
//			��������ϥΡC
// 
// �A�α��ҡG
//				�O�@�ݭn�����X�ݪ��@�ɸ귽�A�p�����ܶq�θ�Ƶ��c�C
// 
///////////////////////////////////////////////////////////////////


// �ϥΪ���� : 
// 1. �Ыؤ�����
HANDLE CreateMutex(LPSECURITY_ATTRIBUTES lpMutexAttributes, BOOL bInitialOwner, LPCTSTR lpName)
// 
// Input : 
//          lpMutexAttributes : ���VSECURITY_ATTRIBUTES���c�����w�A
//                              �o�ӵ��c�M�w�F��^���y�`�O�_�i�H�Q�l�i�{�~�ӡC�p�G��NULL�A�h�y�`����Q�~�ӡC
//          bInitialOwner     : �p�G��TRUE�A�h�Ыؤ����ꪺ�u�{�ߧY���������ꪺ�֦��̡C
//          lpName            : �����ꪺ�W�١C�p�G��NULL�A�h�Ыؤ@�ӥ��R�W��������C
// 
// Output : 
//			�p�G�禡���\�A�h�Ǧ^�s�إ� Mutex ���󪺥y�`�C
// 
/////////////////////////////////////////////////////

// 2. ���񤬥���
BOOL ReleaseMutex(HANDLE hMutex)
// 
// Input : 
//          hMutex : �n���񪺤����ꪺ�y�`�C
// 
// Output : 
//			�p�G�禡���\�A�h�Ǧ^�D�s���ȡC
// 
/////////////////////////////////////////////////////


// Example : 

#include <windows.h>
#include <iostream>

HANDLE g_Mutex;

DWORD WINAPI ThreadFunction(LPVOID lpParam) {
    // ������o������
    WaitForSingleObject(g_Mutex, INFINITE);

    // �i�J����ϰ�
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






///////////////////////////////////////////////////////////////////
// 3. �{�ɰϰ쪫��]Critical Section Objects�^�G
// 
// �\��G
//			�b�P�@��{(process)�������(thread)�����Ѥ����X�ݡC
//			���q�šA�L�ݤ��ּҦ�(kernel - mode)�}�P�A����t�ק֩󤬥�����C
// 
// �A�α��ҡG
//				�A�X��u�ݦb�P�@��{���P�B�������C
// 
///////////////////////////////////////////////////////////////////


// �ϥΪ���� : 
// 1. ��l���{�ɰϰ쪫��
void InitializeCriticalSection(LPCRITICAL_SECTION lpCriticalSection)
// 
// Input : 
//          lpCriticalSection : ���V�n��l�ƪ��{�ɰϰ쪫�󪺫��w�C
// 
// Output : 
//			None�C
// 
/////////////////////////////////////////////////////

// 2. �i�J�w��l�ƪ��{�ɰϰ�
void EnterCriticalSection(LPCRITICAL_SECTION lpCriticalSection)
// 
// Input : 
//          lpCriticalSection : ���V�n�i�J���{�ɰϰ쪫�󪺫��w�C
// 
// Output : 
//			None�C
// 
// Remark : 
//          �p�G�{�ɰϰ�w�Q��L������֦��A
//          �h�եΪ�������N�Q���_�A�����{�ɰϰ��ܬ��i�ΡC
// 
/////////////////////////////////////////////////////

// 3. ���նi�J�w��l�ƪ��{�ɰϰ�
BOOL TryEnterCriticalSection(LPCRITICAL_SECTION lpCriticalSection)
// 
// Input : 
//          lpCriticalSection : ���V�n���նi�J���{�ɰϰ쪫�󪺫��w�C
// 
// Output : 
//			�p�G���\��J����ϰ�A�Υثe���u�{�w�g�֦�����ϰ�A�h�Ǧ^�Ȭ��D�s�C
//          �p�G�t�@�ӽu�{�w�g�֦�����ϰ�A�h�Ǧ^�Ȭ��s�C
// 
// 
/////////////////////////////////////////////////////

// 4. ���}�{�ɰϰ�
void LeaveCriticalSection(LPCRITICAL_SECTION lpCriticalSection)
// 
// Input : 
//          lpCriticalSection : ���V�n���}���{�ɰϰ쪫�󪺫��w�C
// 
// Output : 
//          None.
// 
/////////////////////////////////////////////////////

// 5. �R���{�ɰϰ쪫��
void DeleteCriticalSection(LPCRITICAL_SECTION lpCriticalSection)
// 
// Input : 
//          lpCriticalSection : ���V�n�R�����{�ɰϰ쪫�󪺫��w�C
// 
// Output : 
//          None.
// 
/////////////////////////////////////////////////////


// Example : 

#include <windows.h>
#include <iostream>

CRITICAL_SECTION g_CriticalSection; // �w�q�@�ӥ�������Ϭq��H

DWORD WINAPI ThreadFunction(LPVOID lpParam) {
    EnterCriticalSection(&g_CriticalSection); // �i�J����Ϭq

    // �o�̬O�Q�O�@���N�X�ϰ�A�@���u�঳�@�ӽu�{����o�̪��N�X
    std::cout << "Thread " << GetCurrentThreadId() << " is entering critical section." << std::endl;

    // ������@�ɸ귽���X��
    Sleep(3000); // ��������ާ@���ɶ�

    std::cout << "Thread " << GetCurrentThreadId() << " is leaving critical section." << std::endl;

    LeaveCriticalSection(&g_CriticalSection); // ���}����Ϭq

    return 0;
}

int main() {
    InitializeCriticalSection(&g_CriticalSection); // ��l������Ϭq��H

    // �Ыب�ӽu�{
    HANDLE hThread1 = CreateThread(NULL, 0, ThreadFunction, NULL, 0, NULL);
    HANDLE hThread2 = CreateThread(NULL, 0, ThreadFunction, NULL, 0, NULL);

    // ���ݽu�{����
    WaitForSingleObject(hThread1, INFINITE);
    WaitForSingleObject(hThread2, INFINITE);

    // �M�z�귽
    DeleteCriticalSection(&g_CriticalSection);
    CloseHandle(hThread1);
    CloseHandle(hThread2);

    return 0;
}

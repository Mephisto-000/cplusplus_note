// Timers(1. Standard Timers, 2. Waitable Timers, 3. Queue Timers)



/////////////////////////////////////////////////////
// 1. Standard Timers
// 
// �D�n���� :
//				�D�n�Ω�w�ɧ�s UI �A��{²��ʵe�B�i�ױ��B
//				�����έp�ƾ��������C
// 
/////////////////////////////////////////////////////


// �ϥΪ���� :
// 1. �Ыحp�ɾ�
UINT SetTimer(UINT_PTR nIDEvent, UINT nElapse, void (CALLBACK EXPORT * lpfnTimer)(HWND, UINT, UINT_PTR, DWORD))
// 
// Input : 
//			nIDEvent													: �p�ɾ����s���C
//			nElapse														: �p�ɾ��h�[Ĳ�o�@���A��쬰�@��C
//			(CALLBACK EXPORT * lpfnTimer)(HWND, UINT, UINT_PTR, DWORD)  : �^�I���(callback function)�]�w�C
// 
// Output : 
//			�p�ɾ��s���C�p�G hWnd ���O NULL ���ܡA�h�|�^�ǥ��� nIDEvent�C�p�G�X�{���~�A�h�Ǧ^ 0�C
// 
/////////////////////////////////////////////////////

// 2. �P���p�ɾ�
BOOL KillTimer(UINT_PTR nIDEvent)
//
// Input : 
//			nIDEvent : ���P�����p�ɾ��s��
//
// Output : 
//			�p�G���\�P���p�ɾ��A�Ǧ^TRUE�A�p�G���ѡA�Ǧ^FALSE�C
// 
/////////////////////////////////////////////////////

// 3. �^�I(callback function)
void OnTimer()
//
// Input :
//			None.
//
// Output : 
//			None.
// 
// Remark : 
//			�Ω�b SetTimer() �ҳ]�w���g���ɶ��A����ҳ]�p���{���X�C
// 
/////////////////////////////////////////////////////


// Example :
// �b CTimersDlg �������إ� OnButtonBegin ���s
// �Ω�إ߭p�ɾ�
void CTimersDlg::OnButtonBegin()
{
		.
		.
		.
		// �إ߭p�ɾ��A�C uElapse �@����Ĳ�o�p�ɾ�
		SetTimer(m_nTimerID, uElapse, NULL);
}


// �b CTimersDlg �������إ� OnButtonStop ���s
// �Ω�P���p�ɾ�
void CTimersDlg::OnButtonStop()
{
	// �P���p�ɾ�
	KillTimer(m_nTimerID);
}

// Callback function
void CTimersDlg::OnTimer(UINT nIDEvent)
{
		// do something, but quickly
		.
		.
		.

		CDialog::OnTimer(nIDEvent);
}






/////////////////////////////////////////////////////
// 2. Waitable Timers
// 
// �D�n���� :
//				���Φb�ݭn����Ǳ���ɶ����j�����ȤW�A
//				�Ҧp�t�Ϊ��w�ɳƥ��B�ƾڸ�Ƨ�s�άO�t��
//				���ɶ����ݫ�A����A�åB�i�b���ݮɹ�S�w
//				�����(thread) �i����_(blocked) �A
//				�i�b���ݪ��ɭ����� CPU ���B��귽�C
// 
/////////////////////////////////////////////////////



// �ϥΪ���� :
// 1. �Ыحp�ɾ�
HANDLE CreateWaitableTimer(LPSECURITY_ATTRIBUTES lpTimerAttributes, BOOL bManualReset, LPCTSTR lpTimerName)
//
// Input : 
//			lpTimerAttributes : �w���ݩʪ��]�w�A�i�� NULL�C 
//			bManualReset      : ���w�i���ݭp�ɾ��O��ʭ��]�٬O�۰ʭ��]�C�n�O�]����ʡA�ݭn�ե� SetWaitableTimer�~��N�w�ɾ��ܬ����H�����A
//								�p�G�O�۰ʪ��A�h�ե� WaitForSingleObject ��{�w�ɾ��H�����m�C
//			lpTimerName       : �w�ɾ��W�١A�i�]��NULL�C
//			
//
// Output : 
//			�p�G���\�ЫءA�Ǧ^�i���ݭp�ɾ���H���y�`(handle)�C
// 
/////////////////////////////////////////////////////

// 2. �p�ɾ��]�w
BOOL SetWaitableTimer(HANDLE hTimer, const LARGE_INTEGER * pDueTime, LONG lPeriod, PTIMERAPCROUTINE pfnCompletionRoutine, LPVOID lpArgToCompletionRoutine, BOOL fResume)
//
// Input : 
//			hTimer						: ��Ĳ�o���p�ɾ��y�`�C
//			pDueTime					: ���w�p�ɾ����A�]�m���wĲ�o���ɶ�(�Ĥ@��Ĳ�o���ɶ�)�C
//			lPeriod						: �p�ɾ���Ĳ�o�g���A�H�@�����C
//			pfnCompletionRoutine		: ���V�^�I��� (callback function) �����СA�i�� NULL�C
//			lpArgToCompletionRoutine    : �^�I��Ƥ����ѼơA�i�� NULL�C
//			fResume						: ��p�ɾ����A�w�]�m���wĲ�o�ɡA�O�_�~�����C
//
// Output : 
//			���\����ɡA�Ǧ^�D 0 �ȡC
// 
/////////////////////////////////////////////////////

// 3. �P���p�ɾ�
BOOL CancelWaitableTimer(HANDLE hTimer)
//
// Input : 
//			hTimer : ���P�����p�ɾ��y�` (handle)�C
//
// Output : 
//			���\����ɡA�Ǧ^�D 0 �ȡC
// 
/////////////////////////////////////////////////////


// Example :

#include <windows.h>
#include <iostream>

// �p�ɾ�����ɪ��^�I��� (callback function)
VOID CALLBACK TimerRoutine(LPVOID lpParam, DWORD dwTimerLowValue, DWORD dwTimerHighValue)
{
	std::cout << "Timer expired" << std::endl;
}

int main()
{
	HANDLE hTimer = NULL;

	// �Ыؤ@�ӥi���ݪ��p�ɾ�
	hTimer = CreateWaitableTimer(NULL, TRUE, L"WaitableTimer");
	if (hTimer == NULL)
	{
		std::cerr << "CreateWaitableTimer failed: " << GetLastError() << std::endl;
		return 1;
	}

	// �]�m�p�ɾ��A2�����
	LARGE_INTEGER liDueTime;
	liDueTime.QuadPart = -20000000LL;  // �p�ɾ��b2������A�t�Ȫ�ܬ۹�ɶ�

	if (!SetWaitableTimer(hTimer, &liDueTime, 0, TimerRoutine, NULL, FALSE))
	{
		std::cerr << "SetWaitableTimer failed: " << GetLastError() << std::endl;
		CloseHandle(hTimer);
		return 2;
	}

	// �i�J�L�����ݪ��A�A����^�I��ƳQ��I�M����
	// �����q�D������|�Q���_�A������槹�^�I��� (callback function)
	SleepEx(INFINITE, TRUE);

	// �����p�ɾ�
	if (!CancelWaitableTimer(hTimer))
	{
		std::cerr << "CancelWaitableTimer failed: " << GetLastError() << std::endl;
	}

	CloseHandle(hTimer);
	return 0;
}






/////////////////////////////////////////////////////
// 3. Queue Timers
// 
// �D�n���� :
//				�D�n�Ω�B�z�j�q�p�ɾ��B�z�����ȤW�A
//				�Ҧp�B�z�j�q�D�P�B�L�{(asynchronous procedure)���I�s�A
//				�ڭ̤��ݭn�@�ӭӳ]�w�p�ɴ��b�ӧO��������W�A�p�ɾ���C�ϥ�
//				�������(thread pool) �Ӧ۰����ڭ̤��t��������ϥΡA
//				�ۤ� Waitable Timers �A���Φۦ�޲z������ε��ݾ���A
//				�����C���귽���ӡC
// 
/////////////////////////////////////////////////////



// �ϥΪ���� :
// 1. �إߤ@�ӭp�ɾ���C
HANDLE CreateTimerQueue()
//
// Input : 
//			None�C
//
// Output : 
//			�p�G�Ыئ��\�A��^�p�ɾ����C���y�`�F�p�G���ѡA��^ NULL�C
// 
/////////////////////////////////////////////////////

// 2. �b���w���p�ɾ���C���إߤ@�ӭp�ɾ��A�ó]�w�^�I���(callback function) �M�w�ɪ�Ĳ�o�ɶ�
BOOL CreateTimerQueueTimer(PHANDLE phNewTimer, HANDLE TimerQueue, WAITORTIMERCALLBACK Callback, PVOID Parameter, DWORD DueTime, DWORD Period, ULONG Flags)
//
// Input : 
//			phNewTimer   : ���V HANDLE �����w�A����ư����A�� HANDLE �N�����s�p�ɾ����y�`�C
// 			TimerQueue   : �p�ɾ���C���y�`�A�ӭp�ɾ��N�Q�[�J��o�Ӧ�C���C
// 			Callback     : �p�ɾ���w�ɭn�եΪ��^�I��ƪ����w�C
// 			Parameter    : �^�I��ƪ��ѼơC
// 			DueTime      : �b�p�ɾ��Ĥ@���]�m��Ĳ�o���A�e���ɶ��A��쬰�@��C
// 			Period		 : �p�ɾ��g���A��쬰�@��C�p�G���s�A�p�ɾ��u�|Ĳ�o�@���C
// 			Flags		 :  0. WT_EXECUTEDEFAULT             : ���w�]�ȡA�^�I��ƱN�b�p�ɾ���C�������������C
//							1. WT_EXECUTEINTIMERTHREAD       : ���ܦ^�I��ƱN�b�p�ɾ���C���@���M���p�ɾ������������C
//							2. WT_EXECUTEINIOTHREAD			 : ���ܦ^�I��ƱN�b�@�ӱM���Ω�����J / ��X(I / O) �ާ@�������������C
// 							3. WT_EXECUTEINPERSISTENTTHREAD  : ��ܦ^�I��ƱN�b�@�ӫ���s�b�������������C
//							4. WT_EXECUTELONGFUNCTION		 : ��ܦ^�I��ƥi��ݭn���ɶ�����C
//			 				5. WT_EXECUTEONLYONCE            : ���ܭp�ɾ��u��Ĳ�o�@���A�Ӥ��O���ӳ]�w���g������Ĳ�o�C
// 
// Output : 
//			�p�G���\�A��^ TRUE�F�p�G���ѡA��^ FALSE�C
// 
/////////////////////////////////////////////////////

// 3. ���w�s�b���w�ɾ��]�w
BOOL ChangeTimerQueueTimer(HANDLE TimerQueue, HANDLE Timer, ULONG DueTime, ULONG Period)
//
// Input : 
//			TimerQueue : �p�ɾ���C���y�`�A�p�ɾ������w�b�o�Ӧ�C���C
//			Timer      : �n��諸�p�ɾ����y�`�C
//          DueTime    : �p�ɾ�����Ĳ�o���e���s���ݮɶ��A��쬰�@��C
//			Period     : �p�ɾ����sĲ�o���j�ɶ��A��쬰�@��C
//
// Output : 
//			�p�G���\�A��^ TRUE�F�p�G���ѡA��^ FALSE�C
// 
/////////////////////////////////////////////////////

// 4. �R���@�өw�ɾ�
DeleteTimerQueueTimer(HANDLE TimerQueue, HANDLE Timer, HANDLE CompletionEvent)
//
// Input : 
//			TimerQueue      : �p�ɾ���C���y�`�A�p�ɾ������w�b�o�Ӷ��C���C
//			Timer	        : �n�R�����p�ɾ����y�`�C
//			CompletionEvent : �@�ӥi�諸�ƥ�y�`�C�p�G���޼Ƥ��� NULL�A��ƱN���ݭp�ɾ��Q�R���A
//							  �æb���w���ƥ�W�]�m���H�����A�C�p�G�� INVALID_HANDLE_VALUE�A
//							  ��Ʒ|���ݭp�ɾ��Q�R���A�M��ߧY��^�C
// 
// Output : 
//			�p�G���\�A��^ TRUE�F�p�G���ѡA��^ FALSE�C
// 
/////////////////////////////////////////////////////

// 5. �R���w�ɾ���C�è����Ҧ���C�����w�ɾ����Ƶ{
DeleteTimerQueue(HANDLE TimerQueue, HANDLE CompletionEvent)
//
// Input : 
//			TimerQueue		 : �n�R�����p�ɾ���C���y�`�C
// 			CompletionEvent	 : �@�ӥi�諸�ƥ�y�`�C�P DeleteTimerQueueTimer ���� 
//							   CompletionEvent �ۦ��C
//
// Output : 
//			�p�G���\�A��^ TRUE�F�p�G���ѡA��^ FALSE�C
// 
/////////////////////////////////////////////////////


// Example :

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

	// �Ыؤ@�ӭp�ɾ���C
	hTimerQueue = CreateTimerQueue();
	if (hTimerQueue == NULL)
	{
		std::cerr << "�Ыحp�ɾ���C���ѡC" << std::endl;
		return -1;
	}

	// �b��C���s�W�h�ӭp�ɾ�
	for (int i = 0; i < TIMER_COUNT; ++i)
	{
		BOOL isSuccess = CreateTimerQueueTimer(
			&hTimers[i],						// �p�ɾ��y�`������
			hTimerQueue,						// �p�ɾ���C���y�`
			TimerRoutine,						// �p�ɾ��^�I���
			reinterpret_cast<PVOID>(i),			// �ǻ����^�I��ƪ��ѼơA�Ω��ѧO�p�ɾ�
			1000 * (i + 1),						// ����Ĳ�o�ɶ��A���P�p�ɾ��]�m���P������Ĳ�o�ɶ�
			2000 * (i + 1),						// Ĳ�o���j�ɶ��A���P�p�ɾ��]�m���P��Ĳ�o���j
			0									// �ﶵ�A0 ��ܨϥιw�]�]�w
		);

		if (!isSuccess)
		{
			std::cerr << "�s�W�p�ɾ����C���ѡC�p�ɾ�ID: " << i << std::endl;
			return -1;
		}

	}

	std::cout << "�����N�䵲���p�ɾ�..." << std::endl;
	getchar();

	// �R���Ҧ��p�ɾ��òM�z��C
	for (int i = 0; i < TIMER_COUNT; ++i)
	{
		DeleteTimerQueueTimer(hTimerQueue, hTimers[i], NULL);
	}
	DeleteTimerQueue(hTimerQueue);

	return 0;
}






/////////////////////////////////////////////////////
// Standard Timers �P Waitable Timers, Queue Timers ���t�� :
//
// �b�ϥ� Standard Timers �ɡA���O�V������
// Message Queues �o�e�B�z�������A�ҥH�� Message Queues
// �����j�q����L�����ݳB�z�ɡAĲ�o Standard Timers ��
// �ɶ��|�]�����ǡA�άO�b OnTimer ���B�z����O�j�q�ɶ����ʧ@
// �ɡA�]�|�y�� Standard Timers Ĳ�o�ɶ����ǽT�C
// 
/////////////////////////////////////////////////////


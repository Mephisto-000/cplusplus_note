// Timers(1. Standard Timers, 2. Waitable Timers, 3. Queue Timers)



/////////////////////////////////////////////////////
// 1. Standard Timers
// 
// 主要應用 :
//				主要用於定時更新 UI ，實現簡單動畫、進度條、
//				時鐘或計數器等元素。
// 
/////////////////////////////////////////////////////


// 使用的函數 :
// 1. 創建計時器
UINT SetTimer(UINT_PTR nIDEvent, UINT nElapse, void (CALLBACK EXPORT * lpfnTimer)(HWND, UINT, UINT_PTR, DWORD))
// 
// Input : 
//			nIDEvent													: 計時器的編號。
//			nElapse														: 計時器多久觸發一次，單位為毫秒。
//			(CALLBACK EXPORT * lpfnTimer)(HWND, UINT, UINT_PTR, DWORD)  : 回呼函數(callback function)設定。
// 
// Output : 
//			計時器編號。如果 hWnd 不是 NULL 的話，則會回傳它的 nIDEvent。如果出現錯誤，則傳回 0。
// 
/////////////////////////////////////////////////////

// 2. 銷毀計時器
BOOL KillTimer(UINT_PTR nIDEvent)
//
// Input : 
//			nIDEvent : 欲銷毀的計時器編號
//
// Output : 
//			如果成功銷毀計時器，傳回TRUE，如果失敗，傳回FALSE。
// 
/////////////////////////////////////////////////////

// 3. 回呼(callback function)
void OnTimer()
//
// Input :
//			None.
//
// Output : 
//			None.
// 
// Remark : 
//			用於在 SetTimer() 所設定的週期時間，執行所設計的程式碼。
// 
/////////////////////////////////////////////////////


// Example :
// 在 CTimersDlg 視窗內建立 OnButtonBegin 按鈕
// 用於建立計時器
void CTimersDlg::OnButtonBegin()
{
		.
		.
		.
		// 建立計時器，每 uElapse 毫秒單位觸發計時器
		SetTimer(m_nTimerID, uElapse, NULL);
}


// 在 CTimersDlg 視窗內建立 OnButtonStop 按鈕
// 用於銷毀計時器
void CTimersDlg::OnButtonStop()
{
	// 銷毀計時器
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
// 主要應用 :
//				應用在需要高精準控制時間間隔的任務上，
//				例如系統的定時備份、數據資料更新或是系統
//				長時間等待後再喚醒，並且可在等待時對特定
//				執行緒(thread) 進行阻斷(blocked) ，
//				可在等待的時候釋放 CPU 的運算資源。
// 
/////////////////////////////////////////////////////



// 使用的函數 :
// 1. 創建計時器
HANDLE CreateWaitableTimer(LPSECURITY_ATTRIBUTES lpTimerAttributes, BOOL bManualReset, LPCTSTR lpTimerName)
//
// Input : 
//			lpTimerAttributes : 安全屬性的設定，可為 NULL。 
//			bManualReset      : 指定可等待計時器是手動重設還是自動重設。要是設為手動，需要調用 SetWaitableTimer才能將定時器變為有信號的，
//								如果是自動的，則調用 WaitForSingleObject 實現定時器信號重置。
//			lpTimerName       : 定時器名稱，可設為NULL。
//			
//
// Output : 
//			如果成功創建，傳回可等待計時器對象的句柄(handle)。
// 
/////////////////////////////////////////////////////

// 2. 計時器設定
BOOL SetWaitableTimer(HANDLE hTimer, const LARGE_INTEGER * pDueTime, LONG lPeriod, PTIMERAPCROUTINE pfnCompletionRoutine, LPVOID lpArgToCompletionRoutine, BOOL fResume)
//
// Input : 
//			hTimer						: 欲觸發的計時器句柄。
//			pDueTime					: 指定計時器狀態設置為已觸發的時間(第一次觸發的時間)。
//			lPeriod						: 計時器的觸發週期，以毫秒為單位。
//			pfnCompletionRoutine		: 指向回呼函數 (callback function) 的指標，可為 NULL。
//			lpArgToCompletionRoutine    : 回呼函數內的參數，可為 NULL。
//			fResume						: 當計時器狀態已設置為已觸發時，是否繼續執行。
//
// Output : 
//			當成功執行時，傳回非 0 值。
// 
/////////////////////////////////////////////////////

// 3. 銷毀計時器
BOOL CancelWaitableTimer(HANDLE hTimer)
//
// Input : 
//			hTimer : 欲銷毀的計時器句柄 (handle)。
//
// Output : 
//			當成功執行時，傳回非 0 值。
// 
/////////////////////////////////////////////////////


// Example :

#include <windows.h>
#include <iostream>

// 計時器到期時的回呼函數 (callback function)
VOID CALLBACK TimerRoutine(LPVOID lpParam, DWORD dwTimerLowValue, DWORD dwTimerHighValue)
{
	std::cout << "Timer expired" << std::endl;
}

int main()
{
	HANDLE hTimer = NULL;

	// 創建一個可等待的計時器
	hTimer = CreateWaitableTimer(NULL, TRUE, L"WaitableTimer");
	if (hTimer == NULL)
	{
		std::cerr << "CreateWaitableTimer failed: " << GetLastError() << std::endl;
		return 1;
	}

	// 設置計時器，2秒後到期
	LARGE_INTEGER liDueTime;
	liDueTime.QuadPart = -20000000LL;  // 計時器在2秒後到期，負值表示相對時間

	if (!SetWaitableTimer(hTimer, &liDueTime, 0, TimerRoutine, NULL, FALSE))
	{
		std::cerr << "SetWaitableTimer failed: " << GetLastError() << std::endl;
		CloseHandle(hTimer);
		return 2;
	}

	// 進入無限等待狀態，直到回呼函數被交付和執行
	// 此階段主執行緒會被阻斷，直到執行完回呼函數 (callback function)
	SleepEx(INFINITE, TRUE);

	// 取消計時器
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
// 主要應用 :
//				主要用於處理大量計時器處理的任務上，
//				例如處理大量非同步過程(asynchronous procedure)的呼叫，
//				我們不需要一個個設定計時期在個別的執行緒上，計時器佇列使用
//				執行緒池(thread pool) 來自動幫我們分配執行緒的使用，
//				相比 Waitable Timers ，不用自行管理執行緒或等待機制，
//				有較低的資源消耗。
// 
/////////////////////////////////////////////////////



// 使用的函數 :
// 1. 建立一個計時器佇列
HANDLE CreateTimerQueue()
//
// Input : 
//			None。
//
// Output : 
//			如果創建成功，返回計時器隊列的句柄；如果失敗，返回 NULL。
// 
/////////////////////////////////////////////////////

// 2. 在指定的計時器佇列內建立一個計時器，並設定回呼函數(callback function) 和定時的觸發時間
BOOL CreateTimerQueueTimer(PHANDLE phNewTimer, HANDLE TimerQueue, WAITORTIMERCALLBACK Callback, PVOID Parameter, DWORD DueTime, DWORD Period, ULONG Flags)
//
// Input : 
//			phNewTimer   : 指向 HANDLE 的指針，此函數執行後，該 HANDLE 將接收新計時器的句柄。
// 			TimerQueue   : 計時器佇列的句柄，該計時器將被加入到這個佇列中。
// 			Callback     : 計時器到定時要調用的回呼函數的指針。
// 			Parameter    : 回呼函數的參數。
// 			DueTime      : 在計時器第一次設置為觸發狀態前的時間，單位為毫秒。
// 			Period		 : 計時器週期，單位為毫秒。如果為零，計時器只會觸發一次。
// 			Flags		 :  0. WT_EXECUTEDEFAULT             : 為預設值，回呼函數將在計時器佇列的執行緒中執行。
//							1. WT_EXECUTEINTIMERTHREAD       : 指示回呼函數將在計時器佇列維護的專門計時器執行緒中執行。
//							2. WT_EXECUTEINIOTHREAD			 : 指示回呼函數將在一個專門用於執行輸入 / 輸出(I / O) 操作的執行緒中執行。
// 							3. WT_EXECUTEINPERSISTENTTHREAD  : 表示回呼函數將在一個持續存在的執行緒中執行。
//							4. WT_EXECUTELONGFUNCTION		 : 表示回呼函數可能需要長時間執行。
//			 				5. WT_EXECUTEONLYONCE            : 指示計時器只應觸發一次，而不是按照設定的週期重複觸發。
// 
// Output : 
//			如果成功，返回 TRUE；如果失敗，返回 FALSE。
// 
/////////////////////////////////////////////////////

// 3. 更改已存在的定時器設定
BOOL ChangeTimerQueueTimer(HANDLE TimerQueue, HANDLE Timer, ULONG DueTime, ULONG Period)
//
// Input : 
//			TimerQueue : 計時器佇列的句柄，計時器必須已在這個佇列中。
//			Timer      : 要更改的計時器的句柄。
//          DueTime    : 計時器首次觸發之前的新等待時間，單位為毫秒。
//			Period     : 計時器的新觸發間隔時間，單位為毫秒。
//
// Output : 
//			如果成功，返回 TRUE；如果失敗，返回 FALSE。
// 
/////////////////////////////////////////////////////

// 4. 刪除一個定時器
DeleteTimerQueueTimer(HANDLE TimerQueue, HANDLE Timer, HANDLE CompletionEvent)
//
// Input : 
//			TimerQueue      : 計時器佇列的句柄，計時器必須已在這個隊列中。
//			Timer	        : 要刪除的計時器的句柄。
//			CompletionEvent : 一個可選的事件句柄。如果此引數不為 NULL，函數將等待計時器被刪除，
//							  並在指定的事件上設置有信號狀態。如果為 INVALID_HANDLE_VALUE，
//							  函數會等待計時器被刪除，然後立即返回。
// 
// Output : 
//			如果成功，返回 TRUE；如果失敗，返回 FALSE。
// 
/////////////////////////////////////////////////////

// 5. 刪除定時器佇列並取消所有佇列中的定時器的排程
DeleteTimerQueue(HANDLE TimerQueue, HANDLE CompletionEvent)
//
// Input : 
//			TimerQueue		 : 要刪除的計時器佇列的句柄。
// 			CompletionEvent	 : 一個可選的事件句柄。與 DeleteTimerQueueTimer 中的 
//							   CompletionEvent 相似。
//
// Output : 
//			如果成功，返回 TRUE；如果失敗，返回 FALSE。
// 
/////////////////////////////////////////////////////


// Example :

#include <windows.h>
#include <iostream>

// 計時器回呼函數
VOID CALLBACK TimerRoutine(PVOID lpParam, BOOLEAN TimerOrWaitFired)
{
	int timerId = reinterpret_cast<int>(lpParam);
	std::cout << "計時器事件觸發，計時器ID: " << timerId << "，執行緒ID：" << GetCurrentThreadId() << std::endl;
}

int main()
{
	const int TIMER_COUNT = 5;  // 計時器數量
	HANDLE hTimers[TIMER_COUNT];  // 計時器句柄陣列
	HANDLE hTimerQueue = NULL;

	// 創建一個計時器佇列
	hTimerQueue = CreateTimerQueue();
	if (hTimerQueue == NULL)
	{
		std::cerr << "創建計時器佇列失敗。" << std::endl;
		return -1;
	}

	// 在佇列中新增多個計時器
	for (int i = 0; i < TIMER_COUNT; ++i)
	{
		BOOL isSuccess = CreateTimerQueueTimer(
			&hTimers[i],						// 計時器句柄的指標
			hTimerQueue,						// 計時器佇列的句柄
			TimerRoutine,						// 計時器回呼函數
			reinterpret_cast<PVOID>(i),			// 傳遞給回呼函數的參數，用於識別計時器
			1000 * (i + 1),						// 首次觸發時間，不同計時器設置不同的首次觸發時間
			2000 * (i + 1),						// 觸發間隔時間，不同計時器設置不同的觸發間隔
			0									// 選項，0 表示使用預設設定
		);

		if (!isSuccess)
		{
			std::cerr << "新增計時器到佇列失敗。計時器ID: " << i << std::endl;
			return -1;
		}

	}

	std::cout << "按任意鍵結束計時器..." << std::endl;
	getchar();

	// 刪除所有計時器並清理佇列
	for (int i = 0; i < TIMER_COUNT; ++i)
	{
		DeleteTimerQueueTimer(hTimerQueue, hTimers[i], NULL);
	}
	DeleteTimerQueue(hTimerQueue);

	return 0;
}






/////////////////////////////////////////////////////
// Standard Timers 與 Waitable Timers, Queue Timers 的差異 :
//
// 在使用 Standard Timers 時，它是向視窗的
// Message Queues 發送處理的消息，所以當 Message Queues
// 中有大量的其他消息待處理時，觸發 Standard Timers 的
// 時間會因此不準，或是在 OnTimer 內處理須花費大量時間的動作
// 時，也會造成 Standard Timers 觸發時間不準確。
// 
/////////////////////////////////////////////////////


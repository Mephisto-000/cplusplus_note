// Synchronization Objects (同步物件)
//	
// 1. 信號量物件（Semaphore Objects）
// 2. 互斥物件（Mutex Objects）
// 3. 臨界區域物件（Critical Section Objects）


// 三種物件的主要用途 : 
//
// 用於協調多個執行緒(thread)之間的操作，以確保數據的一致性和避免競爭條件(race condition)的發生。
//
 

///////////////////////////////////////////////////////////////////
// 1. 信號量物件（Semaphore Objects）：
// 
// 功能 ： 
//		   控制對有限資源的同時訪問。當中有計數器 (Counter)，表示可用資源數量；
//		   當執行緒請求資源時，計數器減少，釋放時增加。
// 
// 適用情境 ： 
//				管理對資源池(pool of resources)（如數據庫連接）的訪問。
// 
///////////////////////////////////////////////////////////////////


// 使用的函數 : 
// 1. 創建信號量物件 :
HANDLE CreateSemaphore(LPSECURITY_ATTRIBUTES lpSemaphoreAttributes, LONG lInitialCount, LONG lMaximumCount, LPCTSTR lpName)
// 
// Input : 
//			lpSemaphoreAttributes : 安全權限設置，如果為 NULL，則句柄不能被繼承。。
//			lInitialCount	      : 指定信號量的初始計數值。此值必須大於或等於零，且小於或等於 lMaximumCount。 
//									當旗號的計數大於零，且當其為零時，會發出訊號。 每當等候函式釋放等候號號的執行緒時，
//									計數就會減少一個。 計數會藉由呼叫ReleaseSemaphore函式來增加指定的數量。
//			lMaximumCount         : 指定信號量的最大計數值。
//			lpName				  : 信號量的名稱，如果是NULL，則信號量是未命名的。
// 
// Output : 
//			如果函式創建成功，則傳回值是旗號物件的句柄。
// 
/////////////////////////////////////////////////////

// 2. 增加信號量的計數值(通常在一個執行緒完成對共享資源的訪問後)
BOOL ReleaseSemaphore(HANDLE hSemaphore, LONG lReleaseCount, LPLONG lpPreviousCount)
// 
// Input : 
//			hSemaphore      : 要釋放的信號量的句柄。
//			lReleaseCount   : 要增加的釋放計數值。此值必須大於零。 
//							  如果指定的數量會導致號志的計數超過建立旗號時所指定的計數上限，
//							  則計數不會變更，且函式會傳回FALSE。
//			lpPreviousCount : 要接收信號先前計數之變數的指標。 
//							  如果不需要先前的計數，這個參數可以是 Null 。
// 
// Output : 
//			如果函式執行成功，則傳回非零的值。
// 
/////////////////////////////////////////////////////


// Example : 

#include <windows.h>
#include <iostream>

HANDLE g_Semaphore;

DWORD WINAPI ThreadFunction(LPVOID lpParam) {
    // 等待信號量
    WaitForSingleObject(g_Semaphore, INFINITE);

    // 現在這個線程擁有對共享資源的獨占訪問
    std::cout << "Thread " << GetCurrentThreadId() << " is accessing the shared resource." << std::endl;

    // 模擬對共享資源的訪問
    Sleep(3000); // 假設這是對共享資源的操作

    std::cout << "Thread " << GetCurrentThreadId() << " has finished accessing the shared resource." << std::endl;

    // 釋放信號量
    ReleaseSemaphore(g_Semaphore, 1, NULL);

    return 0;
}

int main() {
    // 創建信號量，初始計數和最大計數都設為1，這樣一次只有一個線程能獲得信號量
    g_Semaphore = CreateSemaphore(NULL, 1, 1, NULL);

    // 創建兩個線程，模擬對共享資源的訪問
    HANDLE hThreads[2];
    for (int i = 0; i < 2; ++i) {
        hThreads[i] = CreateThread(NULL, 0, ThreadFunction, NULL, 0, NULL);
    }

    // 等待所有線程完成
    WaitForMultipleObjects(2, hThreads, TRUE, INFINITE);

    // 清理
    for (int i = 0; i < 2; ++i) {
        CloseHandle(hThreads[i]);
    }
    CloseHandle(g_Semaphore);

    return 0;
}






///////////////////////////////////////////////////////////////////
// 2. 互斥物件（Mutex Objects）：
// 
// 功能：
//			保證對共享資源的互斥訪問。其中含有所有權(Ownership)的概念
//			一次只有一個執行緒可以擁有互斥鎖 (mutex)，進而限制單一
//			執行緒的使用。
// 
// 適用情境：
//				保護需要互斥訪問的共享資源，如全局變量或資料結構。
// 
///////////////////////////////////////////////////////////////////


// 使用的函數 : 
// 1. 創建互斥鎖
HANDLE CreateMutex(LPSECURITY_ATTRIBUTES lpMutexAttributes, BOOL bInitialOwner, LPCTSTR lpName)
// 
// Input : 
//          lpMutexAttributes : 指向SECURITY_ATTRIBUTES結構的指針，
//                              這個結構決定了返回的句柄是否可以被子進程繼承。如果為NULL，則句柄不能被繼承。
//          bInitialOwner     : 如果為TRUE，則創建互斥鎖的線程立即成為互斥鎖的擁有者。
//          lpName            : 互斥鎖的名稱。如果為NULL，則創建一個未命名的互斥鎖。
// 
// Output : 
//			如果函式成功，則傳回新建立 Mutex 物件的句柄。
// 
/////////////////////////////////////////////////////

// 2. 釋放互斥鎖
BOOL ReleaseMutex(HANDLE hMutex)
// 
// Input : 
//          hMutex : 要釋放的互斥鎖的句柄。
// 
// Output : 
//			如果函式成功，則傳回非零的值。
// 
/////////////////////////////////////////////////////


// Example : 

#include <windows.h>
#include <iostream>

HANDLE g_Mutex;

DWORD WINAPI ThreadFunction(LPVOID lpParam) {
    // 等待獲得互斥鎖
    WaitForSingleObject(g_Mutex, INFINITE);

    // 進入關鍵區域
    std::cout << "Thread " << GetCurrentThreadId() << " is updating the counter." << std::endl;

    // 模擬對共享資源（這裡為計數器）的訪問
    static int counter = 0; // 假設這是一個共享資源
    counter++;
    std::cout << "Counter value: " << counter << std::endl;

    Sleep(3000); // 模擬對共享資源的處理時間

    // 釋放互斥鎖
    ReleaseMutex(g_Mutex);

    return 0;
}

int main() {
    // 創建一個互斥鎖
    g_Mutex = CreateMutex(NULL, FALSE, NULL);

    // 創建兩個線程
    HANDLE hThreads[2];
    for (int i = 0; i < 2; ++i) {
        hThreads[i] = CreateThread(NULL, 0, ThreadFunction, NULL, 0, NULL);
    }

    // 等待所有線程完成
    WaitForMultipleObjects(2, hThreads, TRUE, INFINITE);

    // 清理
    for (int i = 0; i < 2; ++i) {
        CloseHandle(hThreads[i]);
    }
    CloseHandle(g_Mutex);

    return 0;
}






///////////////////////////////////////////////////////////////////
// 3. 臨界區域物件（Critical Section Objects）：
// 
// 功能：
//			在同一行程(process)的執行緒(thread)間提供互斥訪問。
//			輕量級，無需內核模式(kernel - mode)開銷，執行速度快於互斥物件。
// 
// 適用情境：
//				適合於只需在同一行程內同步的場景。
// 
///////////////////////////////////////////////////////////////////


// 使用的函數 : 
// 1. 初始化臨界區域物件
void InitializeCriticalSection(LPCRITICAL_SECTION lpCriticalSection)
// 
// Input : 
//          lpCriticalSection : 指向要初始化的臨界區域物件的指針。
// 
// Output : 
//			None。
// 
/////////////////////////////////////////////////////

// 2. 進入已初始化的臨界區域
void EnterCriticalSection(LPCRITICAL_SECTION lpCriticalSection)
// 
// Input : 
//          lpCriticalSection : 指向要進入的臨界區域物件的指針。
// 
// Output : 
//			None。
// 
// Remark : 
//          如果臨界區域已被其他執行緒擁有，
//          則調用的執行緒將被阻斷，直到臨界區域變為可用。
// 
/////////////////////////////////////////////////////

// 3. 嘗試進入已初始化的臨界區域
BOOL TryEnterCriticalSection(LPCRITICAL_SECTION lpCriticalSection)
// 
// Input : 
//          lpCriticalSection : 指向要嘗試進入的臨界區域物件的指針。
// 
// Output : 
//			如果成功輸入關鍵區域，或目前的線程已經擁有關鍵區域，則傳回值為非零。
//          如果另一個線程已經擁有關鍵區域，則傳回值為零。
// 
// 
/////////////////////////////////////////////////////

// 4. 離開臨界區域
void LeaveCriticalSection(LPCRITICAL_SECTION lpCriticalSection)
// 
// Input : 
//          lpCriticalSection : 指向要離開的臨界區域物件的指針。
// 
// Output : 
//          None.
// 
/////////////////////////////////////////////////////

// 5. 刪除臨界區域物件
void DeleteCriticalSection(LPCRITICAL_SECTION lpCriticalSection)
// 
// Input : 
//          lpCriticalSection : 指向要刪除的臨界區域物件的指針。
// 
// Output : 
//          None.
// 
/////////////////////////////////////////////////////


// Example : 

#include <windows.h>
#include <iostream>

CRITICAL_SECTION g_CriticalSection; // 定義一個全局關鍵區段對象

DWORD WINAPI ThreadFunction(LPVOID lpParam) {
    EnterCriticalSection(&g_CriticalSection); // 進入關鍵區段

    // 這裡是被保護的代碼區域，一次只能有一個線程執行這裡的代碼
    std::cout << "Thread " << GetCurrentThreadId() << " is entering critical section." << std::endl;

    // 模擬對共享資源的訪問
    Sleep(3000); // 模擬執行操作的時間

    std::cout << "Thread " << GetCurrentThreadId() << " is leaving critical section." << std::endl;

    LeaveCriticalSection(&g_CriticalSection); // 離開關鍵區段

    return 0;
}

int main() {
    InitializeCriticalSection(&g_CriticalSection); // 初始化關鍵區段對象

    // 創建兩個線程
    HANDLE hThread1 = CreateThread(NULL, 0, ThreadFunction, NULL, 0, NULL);
    HANDLE hThread2 = CreateThread(NULL, 0, ThreadFunction, NULL, 0, NULL);

    // 等待線程結束
    WaitForSingleObject(hThread1, INFINITE);
    WaitForSingleObject(hThread2, INFINITE);

    // 清理資源
    DeleteCriticalSection(&g_CriticalSection);
    CloseHandle(hThread1);
    CloseHandle(hThread2);

    return 0;
}

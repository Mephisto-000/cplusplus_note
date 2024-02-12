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

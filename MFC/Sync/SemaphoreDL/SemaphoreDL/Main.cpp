#include <windows.h>
#include <iostream>

// 創建一個全局信號量句柄
HANDLE g_Semaphore;

// 線程函數
DWORD WINAPI ThreadFunction(LPVOID lpParam) {
    // 嘗試獲取信號量
    WaitForSingleObject(g_Semaphore, INFINITE);

    // 獲取到信號量後，進行操作
    std::cout << "Thread " << GetCurrentThreadId() << " has entered critical section." << std::endl;

    // 模擬執行操作
    Sleep(3000);  // 1秒延遲代表處理過程

    std::cout << "Thread " << GetCurrentThreadId() << " is leaving critical section." << std::endl;

    // 釋放信號量，讓其他線程可以獲取
    ReleaseSemaphore(g_Semaphore, 1, NULL);

    return 0;
}

int main() {
    // 初始化信號量，初始計數設為1，最大計數也設為1
    g_Semaphore = CreateSemaphore(NULL, 1, 1, NULL);

    // 創建兩個工作線程
    HANDLE hThread1 = CreateThread(NULL, 0, ThreadFunction, NULL, 0, NULL);
    HANDLE hThread2 = CreateThread(NULL, 0, ThreadFunction, NULL, 0, NULL);

    // 等待所有線程執行完畢
    WaitForSingleObject(hThread1, INFINITE);
    WaitForSingleObject(hThread2, INFINITE);

    // 清理
    CloseHandle(hThread1);
    CloseHandle(hThread2);
    CloseHandle(g_Semaphore);

    return 0;
}

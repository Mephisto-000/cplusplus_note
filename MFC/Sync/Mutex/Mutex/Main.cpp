#include <windows.h>
#include <iostream>

HANDLE g_Mutex;

DWORD WINAPI ThreadFunction(LPVOID lpParam) {
    // 等待獲得互斥鎖
    WaitForSingleObject(g_Mutex, INFINITE);

    // 進入關鍵區段
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

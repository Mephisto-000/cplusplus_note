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

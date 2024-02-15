#include <windows.h>
#include <iostream>

// 共享資源
int sharedData = 0;

// 線程函數
DWORD WINAPI ThreadFunction(LPVOID lpParam) {
    for (int i = 0; i < 100000; ++i) {
        // 這裡存在競爭條件，因為多個線程試圖同時修改sharedData
        sharedData++;
    }
    return 0;
}

int main() {
    // 創建兩個線程
    HANDLE hThread1 = CreateThread(NULL, 0, ThreadFunction, NULL, 0, NULL);
    HANDLE hThread2 = CreateThread(NULL, 0, ThreadFunction, NULL, 0, NULL);

    // 等待線程結束
    WaitForSingleObject(hThread1, INFINITE);
    WaitForSingleObject(hThread2, INFINITE);

    // 輸出最終的共享資源值
    std::cout << "Expected value: 200000" << std::endl;
    std::cout << "Actual value: " << sharedData << std::endl;

    // 關閉線程句柄
    CloseHandle(hThread1);
    CloseHandle(hThread2);

    return 0;
}
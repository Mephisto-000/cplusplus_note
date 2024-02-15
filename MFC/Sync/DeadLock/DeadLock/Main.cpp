#include <windows.h>
#include <iostream>

// 定義兩個全局互斥鎖
HANDLE Mutex1;
HANDLE Mutex2;

// 線程函數1
DWORD WINAPI ThreadFunc1(LPVOID lpParam) {
    WaitForSingleObject(Mutex1, INFINITE); // 首先獲取Mutex1
    Sleep(100); // 等待一段時間，增加死鎖發生的可能性
    WaitForSingleObject(Mutex2, INFINITE); // 然後試圖獲取Mutex2

    std::cout << "Thread 1 has both Mutex1 and Mutex2" << std::endl;

    ReleaseMutex(Mutex2);
    ReleaseMutex(Mutex1);

    return 0;
}

// 線程函數2
DWORD WINAPI ThreadFunc2(LPVOID lpParam) {
    WaitForSingleObject(Mutex2, INFINITE); // 首先獲取Mutex2
    Sleep(100); // 等待一段時間，增加死鎖發生的可能性
    WaitForSingleObject(Mutex1, INFINITE); // 然後試圖獲取Mutex1

    std::cout << "Thread 2 has both Mutex1 and Mutex2" << std::endl;

    ReleaseMutex(Mutex1);
    ReleaseMutex(Mutex2);

    return 0;
}

int main() {
    // 創建兩個互斥鎖
    Mutex1 = CreateMutex(NULL, FALSE, NULL);
    Mutex2 = CreateMutex(NULL, FALSE, NULL);

    // 創建兩個線程
    HANDLE Thread1 = CreateThread(NULL, 0, ThreadFunc1, NULL, 0, NULL);
    HANDLE Thread2 = CreateThread(NULL, 0, ThreadFunc2, NULL, 0, NULL);

    // 等待線程結束
    WaitForSingleObject(Thread1, INFINITE);
    WaitForSingleObject(Thread2, INFINITE);

    // 關閉線程和互斥鎖句柄
    CloseHandle(Thread1);
    CloseHandle(Thread2);
    CloseHandle(Mutex1);
    CloseHandle(Mutex2);

    return 0;
}

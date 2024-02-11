#include <windows.h>
#include <iostream>

// 執行緒函數，用於處理計時器事件
DWORD WINAPI TimerThread(LPVOID lpParam)
{
    std::cout << "執行緒成功建立。執行緒ID：" << GetCurrentThreadId() << std::endl;

    HANDLE hTimer = reinterpret_cast<HANDLE>(lpParam);
    int count = 0; // 計數器，用於追蹤週期次數

    while (count < 3) { // 重複3次
        // 等待計時器觸發，每2秒觸發一次
        if (WaitForSingleObject(hTimer, INFINITE) == WAIT_OBJECT_0) {
            std::cout << "執行緒開始阻塞。執行緒ID：" << GetCurrentThreadId() << std::endl;
            std::cout << "處理中... 執行緒ID：" << GetCurrentThreadId() << std::endl;
            Sleep(5000); // 模擬5秒的處理時間，阻塞執行緒
            std::cout << "執行緒停止阻塞。執行緒ID：" << GetCurrentThreadId() << std::endl;
            count++; // 完成一次處理週期，計數器加1
        }
    }

    return 0; // 執行緒執行完成
}

int main()
{
    std::cout << "主執行緒開始執行。執行緒ID：" << GetCurrentThreadId() << std::endl;

    // 創建計時器，設置為自動重置
    HANDLE hTimer = CreateWaitableTimer(NULL, FALSE, NULL);
    if (hTimer == NULL)
    {
        std::cerr << "創建計時器失敗。錯誤碼：" << GetLastError() << std::endl;
        return 1;
    }

    // 設置計時器，首次觸發時間為2秒後，之後每2秒觸發一次
    LARGE_INTEGER liDueTime;
    liDueTime.QuadPart = -20000000LL; // 2秒後觸發，100奈秒單位

    // 第三個參數設置為2000，表示計時器每隔2秒觸發一次
    if (!SetWaitableTimer(hTimer, &liDueTime, 2000, NULL, NULL, FALSE))
    {
        std::cerr << "設置計時器失敗。錯誤碼：" << GetLastError() << std::endl;
        CloseHandle(hTimer);
        return 1;
    }

    // 創建執行緒，用於處理計時器事件
    HANDLE hThread = CreateThread(NULL, 0, TimerThread, hTimer, 0, NULL);
    if (hThread == NULL)
    {
        std::cerr << "創建執行緒失敗。" << std::endl;
        CloseHandle(hTimer);
        return 1;
    }

    // 等待執行緒完成
    WaitForSingleObject(hThread, INFINITE);

    // 關閉計時器和執行緒控制代碼
    CloseHandle(hTimer);
    CloseHandle(hThread);

    return 0;
}

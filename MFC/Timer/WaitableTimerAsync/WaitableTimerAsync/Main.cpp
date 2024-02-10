#include <windows.h>
#include <iostream>

DWORD WINAPI TimerThread(LPVOID lpParam) {
    // 創建兩個計時器
    HANDLE hTimer1 = CreateWaitableTimer(NULL, FALSE, L"Timer1");
    HANDLE hTimer2 = CreateWaitableTimer(NULL, FALSE, L"Timer2");

    LARGE_INTEGER liDueTime1;
    liDueTime1.QuadPart = -20000000LL;  // 2秒，以100奈秒為單位

    LARGE_INTEGER liDueTime2;
    liDueTime2.QuadPart = -40000000LL;  // 4秒，以100奈秒為單位

    // 設定計時器
    SetWaitableTimer(hTimer1, &liDueTime1, 2000, NULL, NULL, FALSE);  // 每2秒觸發一次
    SetWaitableTimer(hTimer2, &liDueTime2, 4000, NULL, NULL, FALSE);  // 每4秒觸發一次

    DWORD endTime = GetTickCount() + 20000;  // 20秒後結束

    while (GetTickCount() < endTime) {
        // 檢查第一個計時器
        if (WaitForSingleObject(hTimer1, 0) == WAIT_OBJECT_0) {
            std::cout << "計時器1觸發，進行處理。執行緒代碼：" << GetCurrentThreadId() << std::endl;
            // 重置計時器以便再次觸發
            SetWaitableTimer(hTimer1, &liDueTime1, 2000, NULL, NULL, FALSE);
        }

        // 檢查第二個計時器
        if (WaitForSingleObject(hTimer2, 0) == WAIT_OBJECT_0) {
            std::cout << "計時器2觸發，進行處理。執行緒代碼：" << GetCurrentThreadId() << std::endl;
            // 重置計時器以便再次觸發
            SetWaitableTimer(hTimer2, &liDueTime2, 4000, NULL, NULL, FALSE);
        }

        // 執行其他任務，例如輸出計數器
        static int count = 0;
        std::cout << "計時器執行緒計數器：" << ++count << "，執行緒代碼：" << GetCurrentThreadId() << std::endl;

        // 簡單延遲，模擬工作負載
        Sleep(500);
    }

    // 關閉計時器控制代碼
    CloseHandle(hTimer1);
    CloseHandle(hTimer2);

    return 0;
}

int main() {
    std::cout << "主執行緒開始執行。執行緒代碼：" << GetCurrentThreadId() << std::endl;

    // 創建一個新的執行緒來處理計時器和其他任務
    HANDLE hThread = CreateThread(NULL, 0, TimerThread, NULL, 0, NULL);

    // 等待新創建的執行緒結束
    WaitForSingleObject(hThread, INFINITE);

    // 關閉執行緒控制代碼
    CloseHandle(hThread);

    std::cout << "主執行緒結束。執行緒代碼：" << GetCurrentThreadId() << std::endl;

    return 0;
}

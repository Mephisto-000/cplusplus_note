/*+===================================================================

這段程式碼展示了如何在 Windows 環境下使用等待式計時器（Waitable Timer），
並在一個新創建的執行緒中處理計時器觸發的事件，同時在主執行緒和新執行緒中顯示當前的執行緒代碼。
整個過程持續約20秒，在此期間，主執行緒創建了一個新的執行緒來處理計時器事件和其他任務，
並在結束時進行適當的資源清理。

主要步驟如下：

1. **主執行緒開始執行**：程式在主執行緒中啟動，並輸出當前的執行緒代碼。

2. **創建計時器**：在新執行緒中創建了兩個等待式計時器，分別設定為每2秒和每4秒觸發一次。

3. **計時器處理**：當計時器觸發時，新執行緒會輸出相應的訊息，並顯示當前執行緒的代碼。
    這些計時器在觸發後會被重新設定，以便能夠再次觸發。

4. **執行其他任務**：在等待計時器觸發的同時，新執行緒還會執行其他任務，如輸出一個計數器的值，
    並顯示當前執行緒的代碼。

5. **結束條件**：整個過程設定為持續約20秒。當達到結束時間時，新執行緒的循環將結束。

6. **資源清理**：主執行緒會等待新執行緒結束後，關閉並釋放所有計時器和執行緒的控制代碼。

這段程式碼展示了如何在單一執行緒中同時處理計時器事件和其他任務，實現了一種簡單的非同步處理模式。
透過這種方式，可以在不影響主執行緒正常執行的情況下，處理定時任務和事件。

===================================================================+*/

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

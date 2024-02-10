/*+===================================================================

這段程式碼演示了如何在 Windows 環境下利用等待式計時器（Waitable Timer）
來實現定時任務，並將所有操作集中在一個由主執行緒創建的新執行緒中。
整個過程設計為持續約20秒，在此期間，將透過三個不同的計時器來觸發不同的事件。

主要特點包括：

1. **計時器設置**：
                   程式中創建了三個等待式計時器。第一個計時器每2秒觸發一次，
                   第二個計時器每4秒觸發一次，而新加入的第三個計時器則設定為每7秒觸發一次。
                   這三個計時器分別使用 `CreateWaitableTimer` 函數創建，
                   並通過 `SetWaitableTimer` 函數設定其觸發間隔。

2. **非同步處理**：
                   所有計時器的檢查和事件處理都在一個新的執行緒中進行，
                   這個執行緒由主執行緒創建。在新執行緒中，
                   程式透過輪詢的方式使用 `WaitForSingleObject` 函數
                   來非阻塞地檢查每個計時器是否觸發，並在計時器觸發時輸出相應的訊息。

3. **訊息輸出**：
                   每當計時器觸發時，程式會輸出一條訊息來說明哪個計時器觸發，
                   並顯示當前執行緒的代碼，以便於識別處理這些事件的是哪個執行緒。

4. **其他任務**：
                   除了檢查計時器外，新執行緒還會執行其他任務，
                   如透過一個計數器來模擬額外的工作負載。
                   計數器的值及當前執行緒代碼會定期輸出到控制台。

5. **資源清理**：
                   在整個過程結束後，主執行緒會等待新執行緒結束，
                   然後關閉並釋放所有計時器和執行緒的控制代碼，
                   確保資源得到妥善管理。

透過這種設計，程式能夠在不阻塞主執行緒的情況下，有效地管理和處理多個定時任務，
實現了高效的非同步操作。這種方法對於需要定時執行多項任務的應用程序來說，
是一種實用且靈活的解決方案。

===================================================================+*/

#include <windows.h>
#include <iostream>

DWORD WINAPI TimerThread(LPVOID lpParam) {
    // 創建三個計時器
    HANDLE hTimer1 = CreateWaitableTimer(NULL, FALSE, L"Timer1");
    HANDLE hTimer2 = CreateWaitableTimer(NULL, FALSE, L"Timer2");
    HANDLE hTimer3 = CreateWaitableTimer(NULL, FALSE, L"Timer3");

    LARGE_INTEGER liDueTime1, liDueTime2, liDueTime3;
    liDueTime1.QuadPart = -20000000LL;  // 2秒，以100奈秒為單位
    liDueTime2.QuadPart = -40000000LL;  // 4秒，以100奈秒為單位
    liDueTime3.QuadPart = -70000000LL;  // 7秒，以100奈秒為單位

    // 設定計時器
    SetWaitableTimer(hTimer1, &liDueTime1, 2000, NULL, NULL, FALSE);
    SetWaitableTimer(hTimer2, &liDueTime2, 4000, NULL, NULL, FALSE);
    SetWaitableTimer(hTimer3, &liDueTime3, 7000, NULL, NULL, FALSE);

    DWORD endTime = GetTickCount() + 20000;  // 20秒後結束

    while (GetTickCount() < endTime) {
        // 檢查第一個計時器
        if (WaitForSingleObject(hTimer1, 0) == WAIT_OBJECT_0) {
            std::cout << "計時器1觸發，進行處理。執行緒代碼：" << GetCurrentThreadId() << std::endl;
            SetWaitableTimer(hTimer1, &liDueTime1, 2000, NULL, NULL, FALSE);
        }

        // 檢查第二個計時器
        if (WaitForSingleObject(hTimer2, 0) == WAIT_OBJECT_0) {
            std::cout << "計時器2觸發，進行處理。執行緒代碼：" << GetCurrentThreadId() << std::endl;
            SetWaitableTimer(hTimer2, &liDueTime2, 4000, NULL, NULL, FALSE);
        }

        // 檢查第三個計時器
        if (WaitForSingleObject(hTimer3, 0) == WAIT_OBJECT_0) {
            std::cout << "計時器3觸發，進行處理。執行緒代碼：" << GetCurrentThreadId() << std::endl;
            SetWaitableTimer(hTimer3, &liDueTime3, 7000, NULL, NULL, FALSE);
        }

        // 執行其他任務，例如輸出計數器
        static int count = 0;
        std::cout << "計時器執行緒計數器：" << ++count << "，執行緒代碼：" << GetCurrentThreadId() << std::endl;

        Sleep(500);  // 簡單延遲，模擬工作負載
    }

    // 關閉計時器控制代碼
    CloseHandle(hTimer1);
    CloseHandle(hTimer2);
    CloseHandle(hTimer3);

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


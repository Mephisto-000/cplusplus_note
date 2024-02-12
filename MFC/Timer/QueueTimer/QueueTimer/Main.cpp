/*+===================================================================

這段程式碼展示了如何在 Windows 應用程式中使用隊列計時器（Queue Timers）
來處理多個計時任務。程式首先創建一個計時器隊列，然後在這個隊列中新增了五個計時器，
每個計時器都設置了不同的首次觸發時間和觸發間隔。當計時器觸發時，將會調用指定的回呼函數，
並在控制台輸出計時器ID和執行緒ID。

主要步驟包括：

1. **創建計時器隊列**：透過 `CreateTimerQueue` 函數創建一個新的計時器隊列。

2. **新增計時器**：利用迴圈和 `CreateTimerQueueTimer` 函數，在計時器隊列中新增五個計時器。
每個計時器的首次觸發時間和觸發間隔都根據迴圈變數 `i` 設置，以確保每個計時器的設定都不相同。

3. **計時器回呼函數**：每當計時器觸發時，`TimerRoutine` 回呼函數就會被執行。
此函數接收一個參數（在此例中為計時器ID），並在控制台輸出計時器事件的觸發信息，
包括計時器ID和觸發該回呼函數的執行緒ID。

4. **等待用戶輸入**：程式透過 `getchar` 函數等待用戶輸入，以便觀察計時器的行為。

5. **清理資源**：在用戶按下任意鍵後，程式將透過 `DeleteTimerQueueTimer` 函數刪除所有計時器，
然後用 `DeleteTimerQueue` 函數刪除計時器隊列，以釋放相關資源。

這個範例展示了隊列計時器在處理多個計時任務時的便利性和效率，
特別是當需要在應用程式中同時處理多個定時相關的任務時。透過利用隊列計時器和執行緒池，
開發者可以有效地管理計時任務，同時避免了手動執行緒管理的複雜性和開銷。

===================================================================+*/

#include <windows.h>
#include <iostream>

// 計時器回呼函數
VOID CALLBACK TimerRoutine(PVOID lpParam, BOOLEAN TimerOrWaitFired)
{
    int timerId = reinterpret_cast<int>(lpParam);
    std::cout << "計時器事件觸發，計時器ID: " << timerId << "，執行緒ID：" << GetCurrentThreadId() << std::endl;
}

int main()
{
    const int TIMER_COUNT = 5;  // 計時器數量
    HANDLE hTimers[TIMER_COUNT];  // 計時器句柄陣列
    HANDLE hTimerQueue = NULL;

    // 創建一個計時器隊列
    hTimerQueue = CreateTimerQueue();
    if (hTimerQueue == NULL)
    {
        std::cerr << "創建計時器隊列失敗。" << std::endl;
        return -1;
    }

    // 在隊列中新增多個計時器
    for (int i = 0; i < TIMER_COUNT; ++i)
    {
        BOOL isSuccess = CreateTimerQueueTimer(
            &hTimers[i],  // 計時器句柄的指標
            hTimerQueue,  // 計時器隊列的句柄
            TimerRoutine,  // 計時器回呼函數
            reinterpret_cast<PVOID>(i),  // 傳遞給回呼函數的參數，用於識別計時器
            1000 * (i + 1),  // 首次觸發時間，不同計時器設置不同的首次觸發時間
            2000 * (i + 1),  // 觸發間隔時間，不同計時器設置不同的觸發間隔
            0  // 選項，0 表示使用預設設定
        );

        if (!isSuccess)
        {
            std::cerr << "新增計時器到隊列失敗。計時器ID: " << i << std::endl;
            return -1;
        }
    }

    std::cout << "按任意鍵結束計時器..." << std::endl;
    getchar();

    // 刪除所有計時器並清理隊列
    for (int i = 0; i < TIMER_COUNT; ++i)
    {
        DeleteTimerQueueTimer(hTimerQueue, hTimers[i], NULL);
    }
    DeleteTimerQueue(hTimerQueue);

    return 0;
}

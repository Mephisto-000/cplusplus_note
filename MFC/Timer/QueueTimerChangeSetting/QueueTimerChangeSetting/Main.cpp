/*+===================================================================

這段程式碼演示了如何在 Windows 應用程式中使用隊列計時器（Queue Timers）
來安排並修改計時任務。程式首先創建了一個計時器隊列，然後在該隊列中新增一個計時器，
並設定了其首次觸發和後續觸發的間隔時間。在計時器首次設定一段時間後，
程式修改了計時器的觸發設定。當計時器觸發時，會執行一個指定的回呼函數，
該函數將輸出計時器觸發的訊息及執行該回呼函數的執行緒ID。

主要步驟包括：

1. **創建計時器隊列**：
   - 程式透過 `CreateTimerQueue` 函數創建一個計時器隊列，用於管理後續創建的計時器。

2. **新增計時器到隊列**：
   - 利用 `CreateTimerQueueTimer` 函數在創建的隊列中新增一個計時器。
   此計時器初始設定為在5秒後首次觸發，之後每3秒觸發一次。
   - `TimerRoutine` 為計時器觸發時執行的回呼函數，它將輸出計時器觸發的信息。

3. **修改計時器設定**：
   - 程式使用 `Sleep` 函數等待10秒後，透過 `ChangeTimerQueueTimer` 函數
   修改計時器的觸發設定，新的設定為在2秒後首次觸發，之後每1秒觸發一次。

4. **等待用戶輸入並清理資源**：
   - 程式提示用戶按任意鍵以結束，然後使用 `getchar` 函數等待用戶輸入。
   - 在用戶輸入後，程式透過 `DeleteTimerQueueTimer` 函數刪除計時器，
   並透過 `DeleteTimerQueue` 函數刪除計時器隊列，以釋放相關資源。

這個範例展示了隊列計時器在處理定時任務時的靈活性，特別是在需要動態調整計時器設定的情況下。
透過使用隊列計時器和執行緒池，開發者可以有效地管理計時任務，
同時避免了手動管理執行緒的複雜性和開銷，提高了程式的效率和可靠性。

===================================================================+*/

#include <windows.h>
#include <iostream>

// 計時器回呼函數
VOID CALLBACK TimerRoutine(PVOID lpParam, BOOLEAN TimerOrWaitFired)
{
    // 檢查計時器是否真的觸發了
    if (TimerOrWaitFired)
    {
        std::cout << "計時器觸發，執行緒ID：" << GetCurrentThreadId() << std::endl;
    }
}

int main()
{
    HANDLE hTimer = NULL;
    HANDLE hTimerQueue = NULL;

    // 創建一個計時器隊列
    hTimerQueue = CreateTimerQueue();
    if (hTimerQueue == NULL)
    {
        std::cerr << "創建計時器隊列失敗。" << std::endl;
        return -1;
    }

    // 在隊列中創建計時器，初始設定為5秒後首次觸發，之後每3秒觸發一次
    BOOL isSuccess = CreateTimerQueueTimer(&hTimer, hTimerQueue, TimerRoutine, NULL, 5000, 3000, 0);
    if (!isSuccess)
    {
        std::cerr << "新增計時器到隊列失敗。" << std::endl;
        return -1;
    }

    // 等待一段時間後，改變計時器設定，設置為2秒後首次觸發，之後每1秒觸發一次
    Sleep(10000); // 等待10秒
    isSuccess = ChangeTimerQueueTimer(hTimerQueue, hTimer, 2000, 1000);
    if (!isSuccess)
    {
        std::cerr << "更改計時器設定失敗。" << std::endl;
        return -1;
    }

    std::cout << "計時器設定已更改。按任意鍵結束..." << std::endl;
    getchar();

    // 刪除計時器並清理隊列
    DeleteTimerQueueTimer(hTimerQueue, hTimer, NULL);
    DeleteTimerQueue(hTimerQueue);

    return 0;
}

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

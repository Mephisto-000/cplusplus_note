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

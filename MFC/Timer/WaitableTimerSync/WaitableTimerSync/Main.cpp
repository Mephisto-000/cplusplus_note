/*+===================================================================
本程式碼主要演示了在特定的執行緒上模擬同步過程的實現方式，
通過等待式計時器（Waitable Timer）與執行緒的結合使用，來達成定時任務的處理。
在這個案例中，主要的操作並未在主執行緒中進行，而是在一個專門為處理計時器事件
而創建的新執行緒中完成，這樣做的目的是為了避免主執行緒因等待計時器觸發而被阻塞，
從而影響到整個應用程式的響應能力。

在新創建的執行緒中，程式設計了一個循環來等待計時器的觸發事件，
並在每次計時器觸發時模擬執行一個耗時的同步過程，即透過 `Sleep` 函數來模擬
一段耗時5秒的任務處理。這個模擬的同步過程將重複進行3次，以模擬多次處理計時器事件的情境。

整個過程的關鍵在於，雖然這些操作是在一個獨立的執行緒中進行，
但它們是按照預定的時間順序同步執行的，每次處理都需要等待計時器的觸發，
並在觸發後按部就班地完成預定的任務。這種方式體現了同步過程的特點，
即某些操作需要按照一定的順序進行，且後續操作需要等待前面操作的完成。

通過這種設計，我們可以在不影響主執行緒正常運作的前提下，實現對時間敏感的任務處理，
同時也展示了如何在特定的執行緒上進行同步過程的模擬，這對於需要在後台進行
定時或耗時操作的應用程式來說，是一種非常有用的實現模式。

===================================================================+*/

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

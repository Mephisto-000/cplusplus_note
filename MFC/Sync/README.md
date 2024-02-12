# Synchronization Objects

參考網站 : [Microsoft Learn](https://learn.microsoft.com/en-us/windows/win32/sync/synchronization-objects)  
翻譯使用 : ChatGPT 3.5

## Introduction

同步物件（Synchronization Objects）是用於協調多個線程之間的操作，以確保數據的一致性和避免競爭條件的發生。以下是一些常見的同步物件及其簡介：

1. **信號量物件（Semaphore Objects）**：
   - **功能**：控制對有限資源的同時訪問。
   - **計數器 ( Counter )**：表示可用資源數量；當線程請求資源時，計數器減少，釋放時增加。
   - **適用情境**：管理對資源池(pool of resources)（如數據庫連接）的訪問。
2. **互斥物件（Mutex Objects）**：
   - **功能**：保證對共享資源的互斥訪問。
   - **所有權 ( Ownership )**：一次只有一個執行緒可以擁有互斥鎖。
   - **適用情境**：保護需要互斥訪問的共享資源，如全局變量或資料結構。
3. **臨界區段物件（Critical Section Objects）**：
   - **功能**：在同一行程(process)的執行緒(thread)間提供互斥訪問。
   - **性能**：輕量級，無需內核模式(kernel-mode)開銷，執行速度快於互斥物件。
   - **適用情境**：適合於只需在同一行程內同步的場景。
4. **可等待的定時器物件（Waitable Timer Objects）**：
   - **功能**：在設定的時間後發信號，用於喚醒等待的執行緒。
   - **設定方式**：可以設定為單次或重複計時。
   - **適用情境**：定時任務，如在特定時間後執行操作或定時檢查條件。



當中 **Semaphore Objects** 和 **Mutex Objects** 主要用於解決多線程程式設計中的兩類核心問題：資源的互斥訪問（Mutual Exclusion）和對有限資源的同步訪問（Synchronization of Access to Limited Resources）。

**Mutex Objects** 用於確保對共享資源的獨占訪問，防止數據競爭和破壞；而 **Semaphore Objects** 用於控制對一組共享資源的平行訪問，確保不會超過預定的數量，從而在資源有限的情況下實現有效的資源共享。

## 1. Semaphore Objects

創建信號量物件 : 

```c++
HANDLE CreateSemaphore(LPSECURITY_ATTRIBUTES lpSemaphoreAttributes,
                       LONG lInitialCount,
                       LONG lMaximumCount,
                       LPCTSTR lpName
);
```

#### Arguments : 

- **lpSemaphoreAttributes** : 指向一個`SECURITY_ATTRIBUTES`結構的指針，它決定返回的句柄是否可以被子進程繼承。如果為NULL，則句柄不能被繼承。
- **lInitialCount** : 指定信號量的初始計數值。此值必須大於或等於零，且小於或等於 *lMaximumCount*。 當旗號的計數大於零，且當其為零時，會發出訊號。 每當等候函式釋放等候號號的執行緒時，計數就會減少一個。 計數會藉由呼叫`ReleaseSemaphore`函式來增加指定的數量。
- **lMaximumCount** : 指定信號量的最大計數值。
- **lpName** : 信號量的名稱，如果是NULL，則信號量是未命名的。

#### Return Value :

- 如果函式成功，則傳回值是旗號物件的句柄。

要是想要增加信號量的計數值(通常在一個執行緒完成對共享資源的訪問後)則使用 : 

```c++
BOOL ReleaseSemaphore(HANDLE hSemaphore,
                      LONG lReleaseCount,
                      LPLONG lpPreviousCount
);
```

#### Arguments : 

- **hSemaphore** : 要釋放的信號量的句柄。`CreateSemaphore`會傳回這個句柄。
- **lReleaseCount** : 要增加的釋放計數值。此值必須大於零。 如果指定的數量會導致號志的計數超過建立旗號時所指定的計數上限，則計數不會變更，且函式會傳回`FALSE`。
- **lpPreviousCount** : 要接收信號先前計數之變數的指標。 如果不需要先前的計數，這個參數可以是 **Null** 。

#### Return Value :

- 如果函式成功，則傳回非零的值。

### Example 1(防止資源競爭, Race conditions)

$\star$ [什麼是資源競爭(race condition)和死鎖(deadlock)](https://cloudxlab.com/blog/race-condition-and-deadlock/)
$\star$ [執行緒Thread+死結Deadlock](https://ithelp.ithome.com.tw/articles/10309156?sc=rss.iron)
$\star$ [RaceCondition+臨界區間(Critical Section)](https://ithelp.ithome.com.tw/articles/10309334)

簡單來說 : 

|                        **Dead Lock**                         |                      **Race Condition**                      |
| :----------------------------------------------------------: | :----------------------------------------------------------: |
| 說明 : 兩個(含)以上的執行緒(或行程)因互相等待物件的鎖而導致程式無法繼續執行的現象。 | 說明 : 兩個(含)以上的執行緒(或行程)，在相同的運作下，因執行順序的不同，造成處理的結果不同。 |



造成 Dead Lock 的條件(缺一不可) : 

1. **不可搶奪 (No Preemptive)** : 不可互相搶奪彼此的資源。
2. **持有並等待 (Hold and Wait)** : 等待時可以持續持有資源。
3. **互斥 (Mutal Exclusion)** : 資源不能共用。
4. **循環等待 (Circular Waiting)** : 每個 thread(或 process) 都在等待下一個 thread(或 process)，最後一個 thread(或 process) 又剛好在等第一個 thread(或 process) 形成一個圈；健身房裡一人正在踩飛輪，一人正在舉啞鈴，踩飛輪的人想等舉啞鈴的人用完就去舉啞鈴，舉啞鈴的人想等踩飛輪的人用完就去踩飛輪。



```c++
#include <windows.h>
#include <iostream>

HANDLE g_Semaphore;

DWORD WINAPI ThreadFunction(LPVOID lpParam) {
    // 等待信號量
    WaitForSingleObject(g_Semaphore, INFINITE);

    // 現在這個線程擁有對共享資源的獨占訪問
    std::cout << "Thread " << GetCurrentThreadId() << " is accessing the shared resource." << std::endl;

    // 模擬對共享資源的訪問
    Sleep(3000); // 假設這是對共享資源的操作

    std::cout << "Thread " << GetCurrentThreadId() << " has finished accessing the shared resource." << std::endl;

    // 釋放信號量
    ReleaseSemaphore(g_Semaphore, 1, NULL);

    return 0;
}

int main() {
    // 創建信號量，初始計數和最大計數都設為1，這樣一次只有一個線程能獲得信號量
    g_Semaphore = CreateSemaphore(NULL, 1, 1, NULL);

    // 創建兩個線程，模擬對共享資源的訪問
    HANDLE hThreads[2];
    for (int i = 0; i < 2; ++i) {
        hThreads[i] = CreateThread(NULL, 0, ThreadFunction, NULL, 0, NULL);
    }

    // 等待所有線程完成
    WaitForMultipleObjects(2, hThreads, TRUE, INFINITE);

    // 清理
    for (int i = 0; i < 2; ++i) {
        CloseHandle(hThreads[i]);
    }
    CloseHandle(g_Semaphore);

    return 0;
}

```

說明 : 
這段程式碼創建了一個信號量和兩個執行緒，每個執行緒都試圖訪問一個共享資源。信號量確保了一次只有一個執行緒能訪問該資源，從而避免了資源競爭。

### Example 2(防止死鎖, DeadLock)

```c++
#include <windows.h>
#include <iostream>

// 創建一個全局信號量句柄
HANDLE g_Semaphore;

// 線程函數
DWORD WINAPI ThreadFunction(LPVOID lpParam) {
    // 嘗試獲取信號量
    WaitForSingleObject(g_Semaphore, INFINITE);

    // 獲取到信號量後，進行操作
    std::cout << "Thread " << GetCurrentThreadId() << " has entered critical section." << std::endl;

    // 模擬執行操作
    Sleep(3000);  // 1秒延遲代表處理過程

    std::cout << "Thread " << GetCurrentThreadId() << " is leaving critical section." << std::endl;

    // 釋放信號量，讓其他線程可以獲取
    ReleaseSemaphore(g_Semaphore, 1, NULL);

    return 0;
}

int main() {
    // 初始化信號量，初始計數設為1，最大計數也設為1
    g_Semaphore = CreateSemaphore(NULL, 1, 1, NULL);

    // 創建兩個工作線程
    HANDLE hThread1 = CreateThread(NULL, 0, ThreadFunction, NULL, 0, NULL);
    HANDLE hThread2 = CreateThread(NULL, 0, ThreadFunction, NULL, 0, NULL);

    // 等待所有線程執行完畢
    WaitForSingleObject(hThread1, INFINITE);
    WaitForSingleObject(hThread2, INFINITE);

    // 清理
    CloseHandle(hThread1);
    CloseHandle(hThread2);
    CloseHandle(g_Semaphore);

    return 0;
}

```

說明 : 
在這個範例中，我們定義了一個全局信號量`g_Semaphore`，並在`main`函數中用`CreateSemaphore`函數初始化它。接著創建了兩個線程，每個線程都執行`ThreadFunction`。在`ThreadFunction`中，線程首先使用`WaitForSingleObject`函數等待信號量，這代表線程想要進入"關鍵區段"。一旦獲得信號量，線程就模擬一個需要獨占訪問資源的操作（這裡用`Sleep`函數模擬）。操作完成後，線程使用`ReleaseSemaphore`函數釋放信號量，允許其他線程進入關鍵區段。

通過確保在任何情況下，獲得信號量的線程最終都會釋放它，我們可以避免因線程持續等待無法釋放的信號量而導致的死鎖情況。這個範例展示了如何通過合理使用信號量來同步線程，同時避免死鎖的基本策略。

## 2. Mutex Objects

創建互斥鎖 : 

```c++
HANDLE CreateMutex(LPSECURITY_ATTRIBUTES lpMutexAttributes,
                   BOOL bInitialOwner,
                   LPCTSTR lpName
);
```


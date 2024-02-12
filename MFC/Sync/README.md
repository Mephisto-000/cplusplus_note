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

- **hSemaphore** : 
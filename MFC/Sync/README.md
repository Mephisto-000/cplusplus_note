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


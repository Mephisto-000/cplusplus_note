# Timers

參考網站 : [CodeProject](https://www.codeproject.com/Articles/1236/Timers-Tutorial#QueueTimers)

翻譯使用 : ChatGPT 3.5



## Introduction

這篇文章的目標是展示不同種類計時器的實際用途。首先，我們將看到如何使用**"標準"Win32計時器(standard Win32 timers)**，然後切換到**多媒體計時器(multimedia timers)**，提及**可等待計時器(waitable timers)**和**佇列計時器(queue timers)**。我會試著對這些解決方案進行一些一般性比較。基於函數QueryPerformanceFrequency和QueryPerformanceCounter的所謂高分辨率計時器不會被考慮在內，因為它只能用於測量時間間隔，而不能在規則的時間間隔內觸發事件。

根據MSDN，應用程序使用計時器在指定的時間間隔後為窗口安排事件。這意味著如果我們創建一個計時器並指定了uElapse毫秒的時間間隔，它將每個uElapse毫秒執行一次"某事"，直到我們銷毀它。由我們來指定"某事"是什麼。

計時器是操作系統為程序員提供的一個有用功能。但是，它們很容易被誤用：對於不同類型的輪詢（例如，每200毫秒檢查一次用戶是否在編輯框中輸入了某個值），**使用計時器幾乎從來都不是一個好主意。適合使用計時器的是那些不太依賴用戶操作，而更依賴時間流逝的應用程序。**

重要的是要理解計時器的準確性是有限的。Windows不是一個實時操作系統（除了Windows CE），期望計時器處理非常小的時間間隔（例如10毫秒）是不合理的。



## 1. Standard Win32 Timers

當提到計時器時，幾乎總是指這種類型的計時器。在本文中，我使用**Win32計時器(Win32 timers)**一詞僅僅是為了將它們與其他計時器區分開來。在一些文本中，這些計時器被稱為**用戶計時器(user timers)**，因為它們不是**內核對象(kernel objects)**，不像可等待計時器和佇列計時器。

Win32計時器是如何工作的？首先，我們創建一個計時器，指定其間隔時間，並（可選地）將其附加到一個窗口。計時器創建後，它會向窗口消息佇列發送`WM_TIMER`消息，或者如果未指定窗口，則發送到應用程式佇列。我們可以處理此消息來調用我們希望以規則性時間間隔執行的代碼。計時器將持續發送`WM_TIMER`消息，直到被銷毀。



### To create a timer : 

創建計時器 : 

```c++
UINT_PTR SetTimer(HWND hWnd, 
                  UINT_PTR nIDEvent, 
                  UINT uElapse, 
                  TIMERPROC lpTimerFunc);
```

or its MFC : 
```c++
UINT CWnd::SetTimer(UINT_PTR nIDEvent, 
                    UINT nElapse, 
                    void (CALLBACK EXPORT* lpfnTimer)(HWND, UINT, UINT_PTR, DWORD)); 
```

#### Arguments : 

- **hWnd** : 視窗的巨柄(handle)，也就是執行 Timer 的視窗位址，在 CWnd 函數中可以由 `this->GetSafeHwnd()`獲得。
- **nIDEvent** : 計時器的編號，視窗可以設定多個計時器，用設定的編號區分。
- **uElapse** : 計時器多久觸發一次，範圍為 10 ms. ~ 2147483647 ms. 。
- **lpTimerFunc** : 計時器每次觸發呼叫的函數，基本用法設定成 `NULL`，呼叫 `OnTimer` (=`WM_TIMER`)這個函數即可。

#### Return Value : 

- 計時器編號。如果 `hWnd`不是`NULL`的話，則會回傳它的`nIDEvent`。如果出現錯誤，則傳回`0`。



銷毀計時器 : 

```c++
BOOL KillTimer(HWND hWnd, 
               UINT_PTR uIDEvent);
```

or its MFC :
```c++
BOOL CWnd::KillTimer(UINT_PTR nIDEvent);
```

#### Arguments : 

- **hWnd** : 視窗的巨柄(handle)，也就是執行 Timer 的視窗位址，在 CWnd 函數中可以由 `this->GetSafeHwnd()`獲得。
- **nIDEvent** : 計時器的編號，輸入要停止的計時器編號

#### Return Value : 

- 如果成功銷毀計時器，傳回`TRUE`，如果失敗，傳回`FALSE`。



### Example : 

```c++
void CTimersDlg::OnButtonBegin()
{
	.
	.
	.
    // create the timer
    SetTimer (m_nTimerID, uElapse, NULL);
}


void CTimersDlg::OnButtonStop()
{
    // destroy the timer
    KillTimer(m_nTimerID);
}

// Callback function
void CTimersDlg::OnTimer(UINT nIDEvent)  // called every uElapse milliseconds
{
	// do something, but quickly
	.
	.
	.

	CDialog::OnTimer(nIDEvent);
}
```

小結 : 
如果我們需要每半小時檢查信箱是否有新郵件，那麼Win32計時器就是我們想要的。然而，對於更精確的時間測量（經過的時間少於1秒），這些計時器幾乎不是解決方案。主要原因是計時器會向消息佇列發送`WM_TIMER`消息，而我們無法確定何時會處理這個消息。現在，您可能會認為設置`lpTimerFunc`是解決這個問題的方法，但事實並非如此。如果指定了`lpTimerFunc`，則默認窗口程序只有在處理`WM_TIMER`時才調用它。因此，我們仍然需要等待處理`WM_TIMER`。

請注意，使用Win32計時器事件處理是從UI執行緒完成的。這個事實的一個好處是，我們不需要擔心在計時器事件處理程序中損壞我們的數據；另一方面，**在`WM_TIMER`處理程序中花費的時間將影響UI的響應性。如果你不相信我，試著在`CTimersDlg::OnTimer()`內調用類似`::Sleep(10000);`的東西**。



## 2. Waitable Timers

可等待計時器於 Windows 98 和 Windows NT 4.0 中引入，它們旨在與需要阻塞一段時間的執行緒一起使用。這些計時器是內核物件，在指定的時間或規則的時間間隔內被觸發。它們可以指定**回調函數(callback function)**（實際上是**非同步程序(asynchronous procedure)**調用，或稱為 **APC(Asynchronous Procedure Call)**），在計時器被觸發時調用該函數。這個回調函數通常被稱為**完成例程(completion routine)**。

為了啟用完成例程的執行，執行緒必須處於警報狀態（執行 `SleepEx()`、`WaitForSingleObjectEx()`、`WaitForMultipleObjectsEx()`、`MsgWaitForMultipleObjectsEx()`、`SignalObjectAndWait()` 函數）。實際上，這意味著當我們使用可等待計時器時，我們的**主執行緒將被阻塞(main thread will be blocked)**。

開始使用可等待計時器，我們必須打開一個現有的計時器，或創建新的計時器。創建可以通過調用以下函數來完成：

### To create a timer : 

創建計時器 : 

```c++
HANDLE CreateWaitableTimer(LPSECURITY_ATTRIBUTES lpTimerAttributes, 
                           BOOL bManualReset, 
                           LPCTSTR lpTimerName);
```

#### Arguments : 

- **lpTimerAttributes** : 指向**安全描述符號(SECURITY_ATTRIBUTES)**(其實就是有關權限的設定)結構的指針，該結構指定了可等待計時器對象的安全描述符。可以為`NULL`。
- **bManualReset** : 指定可等待計時器是手動重設還是自動重設。要是設為手動，需要調用 `SetWaitableTimer`才能將定時器變為有信號的，如果是自動的，則調用`WaitForSingleObject`實現定時器信號重置。
- **lpTimerName** : 定時器名稱，可設為`NULL`。

#### Return Value : 

- 一個指向可等待計時器對象的句柄。



現在，當我們有了可等待計時器對象的句柄，我們可以對它進行一些有用的操作。要設置它，我們將使用以下函數：

```c++
BOOL SetWaitableTimer(HANDLE hTimer, 
                      const LARGE_INTEGER *pDueTime, 
                      LONG lPeriod, 
                      PTIMERAPCROUTINE pfnCompletionRoutine, 
                      LPVOID lpArgToCompletionRoutine, 
                      BOOL fResume); 
```

#### Arguments : 

- **hTimer** : 想要觸發的計時器句柄。
- **pDueTime** : 指定計時器狀態設置為已觸發的時間(第一次觸發的時間)。
- **lPeriod** : 計時器的觸發週期，以毫秒為單位，類似於`SetTimer()`中的`uElapse`。
- **pfnCompletionRoutine** : 指向完成例程的指標。可以為`NULL`。
- **lpArgToCompletionRoutine** : 完成例程中的參數。可以為`NULL`。
- **fResume** : 當計時器狀態已設置為已觸發時，是否繼續執行。

#### Return Value : 

- 當成功執行時，傳回非 0 值。

然後是等到指定的物件處於訊號狀態或超過指定的時間間隔為止 : 

```c++
DWORD WaitForSingleObject(HANDLE hTimer,
 						DWORD  dwMilliseconds
);
```

#### Arguments : 

- **hTimer** : 要等待的物件的句柄，可以是任何一種可等待物件的句柄，如事件、互斥鎖、信號、計時器等。
- **dwMilliseconds** : 等待的最大時間，以毫秒為單位。如果設置為 `INFINITE`，則表示無限等待，直到物件進入信號狀態或發生錯誤。

#### Return Value : 

- 函數返回值為一個 `DWORD` 整數，表示等待的結果：
  - `WAIT_OBJECT_0` : 物件進入了信號狀態，等待成功。
  - `WAIT_TIMEOUT` : 經過了指定的等待時間，等待超時。
  - 其他值：等待失敗，可通過 `GetLastError` 函數獲取錯誤碼。

```c++
DWORD WaitForMultipleObjects(DWORD nCount,          
						   const HANDLE *lpHandles,      
						   BOOL bWaitAll,        
						   DWORD dwMilliseconds   
);
```

#### Arguments : 

- **nCount** : 可等待物件的數量。
- **lpHandles** : 指向一個 HANDLE 數組的指標，包含了待等待的可等待物件的句柄。
- **bWaitAll** : 指示是否等待所有的物件進入信號狀態。如果設置為 `TRUE`，則等待所有物件進入信號狀態；如果設置為 `FALSE`，則等待任何一個物件進入信號狀態。
- **dwMilliseconds** : 等待的最大時間，以毫秒為單位。如果設置為 `INFINITE`，則表示無限等待，直到對象進入信號狀態或發生錯誤。

#### Return Value : 

- 函數返回值為一個 `DWORD` 整數，表示等待的結果：
  - 如果 `bWaitAll` 為 `TRUE`：
    - `WAIT_OBJECT_0` 到 `WAIT_OBJECT_0 + nCount - 1`：所有對象都進入了信號狀態，等待成功。
    - `WAIT_TIMEOUT`：經過了指定的等待時間，等待超時。
  - 如果 `bWaitAll` 為 `FALSE`：
    - `WAIT_OBJECT_0` 到 `WAIT_OBJECT_0 + nCount - 1`：至少有一個對象進入了信號狀態，等待成功。
    - `WAIT_TIMEOUT`：經過了指定的等待時間，等待超時。



最後，停止可等待計時器的函數：

```c++
BOOL CancelWaitableTimer(HANDLE hTimer); 
```

#### Arguments : 

- **hTimer** : 想要銷毀的計時器句柄。

#### Return Value : 

- 當成功執行時，傳回非 0 值。



### Example 1 (參考網站中給的例子) :

```c++
void CTimersDlg::OnButtonBegin()
{
	.
	.
	.
    // 創建計時器
    timerHandle = CreateWaitableTimer(NULL, FALSE, NULL);
    
    // 設置計時器
	LARGE_INTEGER lElapse;
	lElapse.QuadPart = - ((int)elapse * 10000);  // 將時間轉為 100 個納秒單位 (1*10^(-9) sec.)
	BOOL succ = SetWaitableTimer(timerHandle, &lElapse, elapse, TimerProc, this, FALSE);

    // 進入循環等待，直到收到信號或銷毀計時器
	for (int i = 0; i < 10; i++)
    {
		SleepEx(INFINITE, TRUE);
    }
    
    // 銷毀計時器
	CancelWaitableTimer(timerHandle);
    
    // 關閉計時器 handle
	CloseHandle (timerHandle);
}

void CTimersDlg::WaitTimerHandler() // called every elapse milliseconds
{
// do what you want to do, but quickly
	.
	.
	.
}

void CALLBACK TimerProc(LPVOID lpArgToCompletionRoutine,
                                DWORD dwTimerLowValue,
                                DWORD dwTimerHighValue)
{
    // This is used only to call WaitTimerHandler
    // Typically, this function is static member of CTimersDlg
    CTimersDlg* obj = (CTimersDlg*) lpArgToCompletionRoutine;
    obj->WaitTimerHandler();
}
```



### Example 2 (詢問 ChatGPT) :

```c++
#include <windows.h>
#include <iostream>

void BackupData()
{
    // 模擬備份操作
    std::cout << "執行備份操作..." << std::endl;
}

int main()
{
    // 設置備份時間，假設每天的凌晨 2 點執行備份
    SYSTEMTIME st;
    st.wYear = 2024;
    st.wMonth = 2;
    st.wDay = 10;
    st.wHour = 2;
    st.wMinute = 0;
    st.wSecond = 0;
    st.wMilliseconds = 0;

    FILETIME ft;
    // 將系統時間轉換為文件時間
    SystemTimeToFileTime(&st, &ft);

    // 創建可等待計時器
    HANDLE hTimer = CreateWaitableTimer(NULL, TRUE, NULL);
    if (hTimer == NULL)
    {
        std::cerr << "創建可等待計時器失敗。" << std::endl;
        return 1;
    }

    // 設置計時器，使其在每天的特定時間點觸發
    if (!SetWaitableTimer(hTimer, &ft, 24 * 60 * 60 * 1000, NULL, NULL, FALSE))
    {
        std::cerr << "設置可等待計時器失敗。" << std::endl;
        CloseHandle(hTimer);
        return 1;
    }

    // 等待計時器信號
    DWORD dwWaitResult = WaitForSingleObject(hTimer, INFINITE);
    if (dwWaitResult == WAIT_OBJECT_0)
    {
        // 計時器信號觸發，執行備份操作
        BackupData();
    }
    else
    {
        std::cerr << "等待計時器失敗。" << std::endl;
    }

    // 關閉計時器
    CloseHandle(hTimer);

    return 0;
}

```

說明 : 
在此示例中，我們使用可等待計時器來安排每天凌晨 2 點執行的備份操作。一旦計時器信號被觸發，將執行備份操作。這樣，我們可以確保備份操作不會阻塞主執行緒，並且可以在計劃的時間執行，而不需要使用標準 Win32 計時器。

$\star$ **Waitable Timers 與 Standard Win32 Timers 的差異** : 

**如果上述範例使用了Standard Win32 Timers（`SetTimer` 函數），情況將會有所不同。使用Standard Win32 Timers會將計時器事件添加到主窗口的消息佇列中，並在消息循環中處理。這將導致主程序在等待計時器事件期間仍然可以處理其他消息，並且不會進入阻塞狀態。**

**換句話說，當使用Standard Win32 Timers時，程序不會阻塞等待計時器事件，而是可以繼續處理其他消息和事件。這可能會導致在等待計時器事件期間，程序仍然需要消耗 CPU 資源和處理其他消息，而這可能不是理想的行為，特別是在需要長時間等待的情況下。**

**兩者差異結論** : 

**根據上述比較分析，可等待計時器和標準 Win32 計時器在使用方式、程序組織結構和消息處理方面存在顯著差異。選擇哪種計時器取決於應用程序的具體需求和使用情境。如果需要程序在等待計時器信號時暫停並釋放 CPU 資源，則可等待計時器是更好的選擇；如果需要在程序繼續運行的同時處理計時器事件，則標準 Win32 計時器可能更適合。**

### Example 3 (詢問 ChatGPT, 使用 `WaitForMultipleObjects`) :

```c++
#include <windows.h>
#include <iostream>

// 計時器 ID
#define TIMER_ID 1

// 計時器事件處理函數
VOID CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
    // 計時器事件處理
    std::cout << "計時器事件處理中..." << std::endl;
}

int main()
{
    // 創建計時器
    HANDLE hTimer = CreateWaitableTimer(NULL, TRUE, NULL);
    if (hTimer == NULL)
    {
        std::cerr << "創建計時器失敗。" << std::endl;
        return 1;
    }

    // 設置計時器，每秒觸發一次
    if (!SetWaitableTimer(hTimer, NULL, 1000, TimerProc, NULL, FALSE))
    {
        std::cerr << "設置計時器失敗。" << std::endl;
        CloseHandle(hTimer);
        return 1;
    }

    // 創建事件對象
    HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    if (hEvent == NULL)
    {
        std::cerr << "創建事件對象失敗。" << std::endl;
        CloseHandle(hTimer);
        return 1;
    }

    // 將計時器和事件對象加入到待等待對象列表中
    HANDLE handles[2] = { hTimer, hEvent };

    // 等待計時器或事件信號的到來
    DWORD dwWaitResult = WaitForMultipleObjects(2, handles, FALSE, INFINITE);

    // 判斷等待的結果
    switch (dwWaitResult)
    {
    case WAIT_OBJECT_0:
        std::cout << "計時器觸發，進行相應處理。" << std::endl;
        break;
    case WAIT_OBJECT_0 + 1:
        std::cout << "事件信號到來，進行相應處理。" << std::endl;
        break;
    case WAIT_TIMEOUT:
        std::cout << "等待超時。" << std::endl;
        break;
    default:
        std::cerr << "等待失敗，錯誤碼：" << GetLastError() << std::endl;
        break;
    }

    // 關閉計時器和事件對象
    CloseHandle(hTimer);
    CloseHandle(hEvent);

    return 0;
}

```

說明 : 
在給出的程式碼範例中，當計時器或事件觸發時，`WaitForMultipleObjects` 函數會將程序置於等待狀態。這意味著直到其中一個對象進入信號狀態，或者超時時間到達，程序才會繼續執行。在這個等待的過程中，程序處於阻塞狀態，無法執行其他的操作。
當其中一個對象進入信號狀態時，程序會根據等待的結果進行相應的處理，然後再次進入阻塞狀態等待下一次觸發或超時。
在這個程式碼範例中，由於我們設置了無限等待時間（INFINITE），因此程序會一直等待計時器或事件觸發，直到其中一個發生為止，這將導致程序在觸發前處於阻塞狀態。

### Example 4 (模擬非同步過程)

```c++
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
```

### Example 5 (模擬同步過程)

```c++
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
```



## 3. Queue Timers

先比較 Queue Timers 與 Waitable Timers 的差異 : 

微軟的隊列計時器（Queue Timers）是一種計時器服務，允許應用程式在指定時間後透過執行緒池中的執行緒執行一個回呼函數。這種計時器透過將回呼函數排入執行緒池中的執行緒來執行，這樣做避免了應用程式需要自行管理執行緒的複雜性。

隊列計時器的主要功能包括：

- `CreateTimerQueue`：建立一個計時器隊列。
- `CreateTimerQueueTimer`：在指定的計時器隊列中建立一個計時器，設定回呼函數及觸發時間。
- `ChangeTimerQueueTimer`：更改已存在計時器的設定。
- `DeleteTimerQueueTimer`：刪除一個計時器。
- `DeleteTimerQueueEx`：刪除計時器隊列並取消隊列中所有計時器的排程。

相較於隊列計時器，等待式計時器（Waitable Timers）則提供了更底層的計時器機制。等待式計時器在設定的時間到期時發出信號，應用程式可以透過等待這個信號來響應計時器事件。這種計時器可以被設定為一次性觸發或按照指定的周期重複觸發。

等待式計時器的主要特點包括：

- 可以使用 `CreateWaitableTimer` 函數建立。
- 透過 `SetWaitableTimer` 函數來設定計時器的觸發時間和周期。
- 當計時器到期時，相關的事件會被設定為有信號狀態，應用程式可以透過等待這個事件（如使用 `WaitForSingleObject` 函數）來響應計時器的觸發。

隊列計時器與等待式計時器的主要差異在於：

1. **實作層次**：隊列計時器透過執行緒池來執行回呼函數，更適合於輕量級且需要定時執行任務的場景；而等待式計時器則提供了更為底層和靈活的計時器機制，允許應用程式透過等待事件的方式來同步計時器的觸發。

2. **資源使用**：由於隊列計時器使用執行緒池，因此在處理大量短暫且輕量級的計時任務時更為高效。相反，等待式計時器則可能需要應用程式自行管理執行緒或等待機制，可能會導致較高的資源消耗。

3. **使用場景**：隊列計時器更適合於不需要精確控制計時器觸發事件處理方式的場景，而等待式計時器則為需要精確同步和控制計時器事件處理的應用程式提供了更多的靈活性。

**執行緒池（Thread Pool）詳細說明：**

執行緒池是一種程式設計模式，用於管理執行緒的創建和銷毀。它允許應用程式以有效的方式重複使用一組預先創建的執行緒，而不是為每個任務創建新的執行緒。當執行緒池中的執行緒完成任務後，它們不會被銷毀，而是返回到池中，等待分配下一個任務。

使用執行緒池的優點包括：

- **提高效率**：避免了執行緒創建和銷毀的開銷，尤其是在需要處理大量短暫任務的場景中。
- **控制並發**：執行緒池可以限制同時運行的執行緒數量，從而控制資源使用和提高系統穩定性。
- **提高響應性**：預先創建的執行緒可以立即響應任務需求，無需等待執行緒創建的延遲。

在 Windows 中，執行緒池通常由系統提供，應用程式可以透過特定的 API 如隊列計時器來使用執行緒池，這簡化了執行緒管理和任務排程的複雜性。

### To create a timer : 

首先，第一個常見的功能，創建一個計時器佇列 : 

```c++
HANDLE CreateTimerQueue();
```

#### Return Value : 

- 如果創建成功，返回計時器隊列的句柄；如果失敗，返回 `NULL`。

然後是建立計時器，並意義其行為 : 

```c++
BOOL CreateTimerQueueTimer( PHANDLE phNewTimer,
   						  HANDLE TimerQueue,
                            WAITORTIMERCALLBACK Callback,
   						  PVOID Parameter,
             			   DWORD DueTime,
             			   DWORD Period,
           				   ULONG Flags
);
```

#### Arguments : 

- **phNewTimer** : 指向 `HANDLE` 的指針，此函數執行後，該 `HANDLE` 將接收新計時器的句柄。
- **TimerQueue** : 計時器隊列的句柄，該計時器將被加入到這個隊列中。由 `CreateTimerQueue` 函數創建。
- **Callback** : 計時器到期時要調用的回呼函數的指針。此函數必須符合 `WAITORTIMERCALLBACK` 函數的寫法。
- **Parameter** : 傳遞給回呼函數的值。
- **DueTime** : 在計時器第一次設置為觸發狀態前的時間（毫秒）。
- **Period** : 計時器週期（毫秒）。如果為零，計時器只會觸發一次。
- **Flags** : 一個或多個以下值 : 
  - `WT_EXECUTEDEFAULT` : 為預設值，回呼函數將在計時器隊列的執行緒中執行。這個選項提供了最大的靈活性，因為它允許系統決定最適合執行回呼函數的時機。
  - `WT_EXECUTEINTIMERTHREAD` : 指示回呼函數將在計時器隊列維護的專門計時器執行緒中執行。這有助於減少回呼函數的執行對其他線程的干擾。
  - `WT_EXECUTEINIOTHREAD` : 指示回呼函數將在一個專門用於執行輸入/輸出 (I/O) 操作的執行緒中執行。這對於需要進行非阻塞 I/O 操作的回呼函數特別有用。
  - `WT_EXECUTEINPERSISTENTTHREAD` : 表示回呼函數將在一個持續存在的執行緒中執行。使用此選項可以避免執行緒的頻繁創建和銷毀，從而提高性能，但可能會增加資源消耗。
  - `WT_EXECUTELONGFUNCTION` : 表示回呼函數可能需要長時間執行。這個選項提示系統此回呼函數可能會阻塞其他任務，因此可能需要額外的資源管理。
  - `WT_EXECUTEONLYONCE` : 指示計時器只應觸發一次，而不是按照設定的週期重複觸發。設定此選項後，計時器觸發一次就會自動取消。

#### Return Value : 

- 如果成功，返回 `TRUE`；如果失敗，返回 `FALSE`。

若要更新已存在的計時器設定時，需用到 : 

```c++
BOOL ChangeTimerQueueTimer(HANDLE TimerQueue,
  						 HANDLE Timer,
                           ULONG DueTime,
                           ULONG Period
);
```

#### Arguments : 

- **TimerQueue** : 計時器隊列的句柄，計時器必須已在這個隊列中。`CreateTimerQueue`函式會傳回此控制碼。
- **Timer** : 要更改的計時器的句柄。`CreateTimerQueueTimer`函式會傳回這個控制碼。
- **DueTime** : 計時器首次觸發之前的新等待時間（以毫秒為單位）。
- **Period** : 計時器的新觸發間隔時間（以毫秒為單位）。

#### Return Value : 

- 如果成功，返回 `TRUE`；如果失敗，返回 `FALSE`。

若要刪除擱置的計時器，則是要使用 : 

```c++
BOOL DeleteTimerQueueTimer(HANDLE TimerQueue,
                           HANDLE Timer,
                           HANDLE CompletionEvent
);
```

#### Arguments : 

- **TimerQueue** : 計時器隊列的句柄，計時器必須已在這個隊列中。`CreateTimerQueue`函式會傳回此控制碼。
- **Timer** : 要刪除的計時器的句柄。`CreateTimerQueueTimer`函式會傳回這個控制碼。
- **CompletionEvent** : 一個可選的事件句柄。如果此引數不為 `NULL`，函數將等待計時器被刪除，並在指定的事件上設置有信號狀態。如果為 `INVALID_HANDLE_VALUE`，函數會等待計時器被刪除，然後立即返回。

#### Return Value : 

- 如果成功，返回 `TRUE`；如果失敗，返回 `FALSE`。

當完成計時器佇列時，要刪除計時器佇列並取消佇列中所有計時器的排程則使用 : 

```c++
BOOL DeleteTimerQueueEx(HANDLE TimerQueue,
					  HANDLE CompletionEvent
);
```

#### Arguments : 

- **TimerQueue** : 要刪除的計時器佇列的句柄。
- **CompletionEvent** : 一個可選的事件句柄。與 `DeleteTimerQueueTimer` 中的 `CompletionEvent` 相似，用於同步計時器隊列的刪除操作。

#### Return Value : 

- 如果成功，返回 `TRUE`；如果失敗，返回 `FALSE`。

### Example 1

```c++
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
```

要是想要更新計時器佇列的設定 : 

### Example 2(使用 `ChangeTimerQueueTimer`) : 

```c++
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
```








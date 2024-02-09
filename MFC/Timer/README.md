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

請注意，使用Win32計時器事件處理是從UI線程完成的。這個事實的一個好處是，我們不需要擔心在計時器事件處理程序中損壞我們的數據；另一方面，**在`WM_TIMER`處理程序中花費的時間將影響UI的響應性。如果你不相信我，試著在`CTimersDlg::OnTimer()`內調用類似`::Sleep(10000);`的東西**。



## 2. Waitable Timers

可等待計時器於 Windows 98 和 Windows NT 4.0 中引入，它們旨在與需要阻塞一段時間的執行緒一起使用。這些計時器是內核對象，在指定的時間或規則的時間間隔內被觸發。它們可以指定**回調函數(callback function)**（實際上是**非同步程序(asynchronous procedure)**調用，或稱為 **APC(Asynchronous Procedure Call)**），在計時器被觸發時調用該函數。這個回調函數通常被稱為**完成例程(completion routine)**。

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



最後，停止可等待計時器的函數：

```c++
BOOL CancelWaitableTimer(HANDLE hTimer); 
```

#### Arguments : 

- **hTimer** : 想要銷毀的計時器句柄。

#### Return Value : 

- 當成功執行時，傳回非 0 值。



### Example :

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


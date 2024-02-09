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
UINT_PTR SetTimer(HWND hWnd, UINT_PTR nIDEvent, UINT uElapse, TIMERPROC lpTimerFunc);
```

or its MFC : 
```c++
UINT CWnd::SetTimer(UINT_PTR nIDEvent, UINT nElapse, 
	void (CALLBACK EXPORT* lpfnTimer)(HWND, UINT, UINT_PTR, DWORD)); 
```

#### Arguments : 

- **hWnd** : 視窗的巨柄(handle)，也就是執行 Timer 的視窗位址，在 CWnd 函數中可以由 `this->GetSafeHwnd()`獲得。
- **nIDEvent** : 計時器的編號，視窗可以設定多個計時器，用設定的編號區分。
- **lpTimerFunc** : 計時器每次觸發呼叫的函數，基本用法設定成 `NULL`，呼叫 `OnTimer` (=`WM_TIMER`)這個函數即可。

#### Return Value : 

- 計時器編號。如果 `hWnd`不是`NULL`的話，則會回傳它的`nIDEvent`。如果出現錯誤，則傳回`0`。



銷毀計時器 : 

```c++
BOOL KillTimer(HWND hWnd, UINT_PTR uIDEvent);
```

or its MFC :
```c++
BOOL CWnd::KillTimer(UINT_PTR nIDEvent);
```

### Arguments : 

- **hWnd** : 視窗的巨柄(handle)，也就是執行 Timer 的視窗位址，在 CWnd 函數中可以由 `this->GetSafeHwnd()`獲得。
- **nIDEvent** : 計時器的編號，輸入要停止的計時器編號

### Return Value : 


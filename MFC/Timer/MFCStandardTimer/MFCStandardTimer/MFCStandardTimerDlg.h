
// MFCStandardTimerDlg.h: 標頭檔
//

#pragma once

#define _USE_MATH_DEFINES // for C++
#include <cmath>
#include <deque>


// CMFCStandardTimerDlg 對話方塊
class CMFCStandardTimerDlg : public CDialogEx
{
	// 建構
public:
	CMFCStandardTimerDlg(CWnd* pParent = nullptr);	// 標準建構函式

	// 對話方塊資料
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCSTANDARDTIMER_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支援


	// 程式碼實作
protected:
	HICON m_hIcon;

	// 產生的訊息對應函式
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	void DrawToBuffer(CDC* pDC, CRect rectShow);      // 雙緩衝
	void DrawGrid(CDC* pDC, CRect rectShow);          // 繪製顯示區內的格線
	void DrawWave(CDC* pDC, CRect rectShow);          // 繪製模擬圖
public:

	DWORD m_dwTime;                                   // 計時開始時間
	UINT_PTR m_nTimerID;                              // 計時器 ID

	double m_dSimTime;                                // 模擬自變數時間
	double m_dResultValue;                            // 模擬的計算結果

	BOOL m_bTimeStart;                                // 判斷是否按下計時開始按鈕

	CString m_strTimeSec;                             // 顯示時間
	CString m_strTimeSecInit;                         // 初始的時間顯示設置

	CFont m_fontTimeSec;							  // 顯示時間的文字格式設定 

	std::deque<double> m_queueResultValue;            // 儲存結果的佇列

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	double ExampleFun(double dTimeValue);             // 範例模擬函數
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};

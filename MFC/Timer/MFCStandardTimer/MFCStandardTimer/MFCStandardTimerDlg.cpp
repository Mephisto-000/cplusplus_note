﻿
// MFCStandardTimerDlg.cpp: 實作檔案
//

#include "pch.h"
#include "framework.h"
#include "MFCStandardTimer.h"
#include "MFCStandardTimerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 對 App About 使用 CAboutDlg 對話方塊
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 對話方塊資料
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援

	// 程式碼實作
protected:
	DECLARE_MESSAGE_MAP()
};


CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}


void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCStandardTimerDlg 對話方塊
CMFCStandardTimerDlg::CMFCStandardTimerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCSTANDARDTIMER_DIALOG, pParent)
	, m_strTimeSec(_T(""))
	, m_strTimeSecInit(_T("0.00  sec."))
	, m_dSimTime(0.0)
	, m_bTimeStart(FALSE)
	, m_nTimerID(1)
	, m_fontTimeSec()
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_fontTimeSec.CreatePointFont(200, _T("Calibri"));
}


void CMFCStandardTimerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_TIME_SHOW, m_strTimeSec);
}


BEGIN_MESSAGE_MAP(CMFCStandardTimerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_START, &CMFCStandardTimerDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CMFCStandardTimerDlg::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDOK, &CMFCStandardTimerDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CMFCStandardTimerDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CMFCStandardTimerDlg 訊息處理常式
BOOL CMFCStandardTimerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 將 [關於...] 功能表加入系統功能表。

	// IDM_ABOUTBOX 必須在系統命令範圍之中。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 設定此對話方塊的圖示。當應用程式的主視窗不是對話方塊時，
	// 框架會自動從事此作業
	SetIcon(m_hIcon, TRUE);			// 設定大圖示
	SetIcon(m_hIcon, FALSE);		// 設定小圖示

	// TODO: 在此加入額外的初始設定

	return TRUE;  // 傳回 TRUE，除非您對控制項設定焦點
}


// 測試用波形函數
double CMFCStandardTimerDlg::ExampleFun(double dTimeValue)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	/*std::uniform_real_distribution<double> dis(-1.0, 1.0);*/
	std::normal_distribution<double> dis(0.0, 1.0);

	double dRandom = dis(gen);

	return 1.5 * sin(2 * dTimeValue) * cos(dTimeValue) + 0.9 * dRandom;
}


void CMFCStandardTimerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果將最小化按鈕加入您的對話方塊，您需要下列的程式碼，
// 以便繪製圖示。對於使用文件/檢視模式的 MFC 應用程式，
// 框架會自動完成此作業。

void CMFCStandardTimerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 繪製的裝置內容

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 將圖示置中於用戶端矩形
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 描繪圖示
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();

		if (m_bTimeStart == FALSE)
		{
			GetDlgItem(IDC_STATIC_TIME_SHOW)->SetFont(&m_fontTimeSec);
			GetDlgItem(IDC_STATIC_TIME_SHOW)->SetWindowText(m_strTimeSecInit);
		}

		// 取得操作視窗矩形資訊
		CWnd* pDrawShowRegion = GetDlgItem(IDC_STATIC_PAINT_REGION);
		CRect rectDrawShowRegion;
		pDrawShowRegion->GetClientRect(&rectDrawShowRegion);
		int iWidthShowRegion = rectDrawShowRegion.Width();
		int iHeightShowRegion = rectDrawShowRegion.Height();

		// 建立雙緩衝程序
		CDC* pdcDrawShowRegion = pDrawShowRegion->GetDC();
		pDrawShowRegion->UpdateWindow();

		CDC memDC;
		CBitmap memBitmap;

		memDC.CreateCompatibleDC(pdcDrawShowRegion);
		memBitmap.CreateCompatibleBitmap(pdcDrawShowRegion, iWidthShowRegion, iHeightShowRegion);
		CBitmap* pOldMemBitMap = memDC.SelectObject(&memBitmap);

		DrawToBuffer(&memDC, rectDrawShowRegion);

		pdcDrawShowRegion->BitBlt(0, 0, iWidthShowRegion, iHeightShowRegion, &memDC, 0, 0, SRCCOPY);

		// 釋放 GDI 對象和設備上下文
		memDC.SelectObject(pOldMemBitMap);
		memBitmap.DeleteObject();
		memDC.DeleteDC();
		ReleaseDC(pdcDrawShowRegion);
	}
}


// 雙緩衝
void CMFCStandardTimerDlg::DrawToBuffer(CDC* pDC, CRect rectShow)
{
	// 繪製背景網格
	try {
		DrawGrid(pDC, rectShow);
	}
	catch (CResourceException*)
	{
		TRACE(_T("Error\n"));
	}

	try {
		DrawWave(pDC, rectShow);
	}
	catch (CResourceException*)
	{
		TRACE(_T("Error\n"));
	}
}


// 繪製模擬畫面中的網格線
void CMFCStandardTimerDlg::DrawGrid(CDC* pDC, CRect rectShow)
{
	// 設置網格線
	CPen penGrid(PS_SOLID, 1, RGB(0, 130, 70));  // 亮綠色格線
	CPen* pOldPenGrid = pDC->SelectObject(&penGrid);

	// 繪製垂直線
	for (int iVertical = 0; iVertical <= rectShow.right; iVertical += 20)
	{
		// 每隔 20 像素繪製一條線
		pDC->MoveTo(iVertical, 0);
		pDC->LineTo(iVertical, rectShow.bottom);
	}

	// 繪製水平線
	for (int iHorizontal = 0; iHorizontal <= rectShow.bottom; iHorizontal += 20)
	{
		// 每隔 20 像素繪製一條線
		pDC->MoveTo(0, iHorizontal);
		pDC->LineTo(rectShow.right, iHorizontal);
	}

	pDC->SelectObject(pOldPenGrid);
}


// 繪製模擬圖
void CMFCStandardTimerDlg::DrawWave(CDC* pDC, CRect rectShow)
{
	// 設置 y = 0 的線
	CPen penY0(PS_SOLID, 4, RGB(255, 255, 0));
	CPen* pOldPenY0 = pDC->SelectObject(&penY0);

	int iYOriginal = rectShow.CenterPoint().y;

	pDC->MoveTo(0, iYOriginal);
	pDC->LineTo(rectShow.right, iYOriginal);
	pDC->SelectObject(pOldPenY0);

	CFont fontY0Text;
	CFont* pOldY0Text = pDC->SelectObject(&fontY0Text);
	fontY0Text.CreatePointFont(200, _T("Arial"));
	pDC->SelectObject(&fontY0Text);
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(255, 255, 0));
	pDC->TextOut(5, rectShow.CenterPoint().y, _T("y=0"));
	pDC->SelectObject(pOldY0Text);

	////////////////////////////////////////////////////////////////////////////
	// 繪製模擬波線
	CPen penWave(PS_SOLID, 8, RGB(255, 0, 0));
	CPen* pOldPenWave = pDC->SelectObject(&penWave);

	for (int i = 1; i < m_queueResultValue.size(); i++)
	{

		double dY = 1 * m_queueResultValue[i - 1];


		int iScreenX = 5*(i - 1);
		int iScreenY = rectShow.CenterPoint().y - static_cast<int>(dY * 100 + 0.5);
		pDC->MoveTo(CPoint(iScreenX, iScreenY));


		double dY2 = 1 * m_queueResultValue[i];

		int iScreenX2 = 5*i;
		int iScreenY2 = rectShow.CenterPoint().y - static_cast<int>(dY2 * 100 + 0.5);
		pDC->LineTo(CPoint(iScreenX2, iScreenY2));


	}

	pDC->SelectObject(pOldPenWave);
}


// Timer 使用
void CMFCStandardTimerDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == m_nTimerID)
	{
		DWORD dwRecordTime = GetTickCount();
		DWORD dwElapsedTime = dwRecordTime - m_dwTime;
		double dSec = static_cast<double>(dwElapsedTime) / 1000.0;  // 經過的時間

		m_strTimeSec.Format(_T("%.2lf  sec."), dSec);

		CWnd* pTimeShow = GetDlgItem(IDC_STATIC_TIME_SHOW);
		if (pTimeShow)
		{
			pTimeShow->SetFont(&m_fontTimeSec);
			pTimeShow->SetWindowText(m_strTimeSec);
		}



		if (m_dSimTime >= 2 * M_PI)
		{
			m_dSimTime = 0.0;
			m_dSimTime += (2 * M_PI) / 250;  // 250
		}
		else
		{
			m_dSimTime += (2 * M_PI) / 250;
		}


		// 更新計算結果
		m_dResultValue = ExampleFun(m_dSimTime);


		CWnd* pDrawShowRegion = GetDlgItem(IDC_STATIC_PAINT_REGION);
		CRect rectDrawShowRegion;

		pDrawShowRegion->GetClientRect(&rectDrawShowRegion);


		// 將計算結果儲存於佇列中
		if (m_queueResultValue.size() > rectDrawShowRegion.Width()*0.2)
		{
			m_queueResultValue.pop_front();
			m_queueResultValue.push_back(m_dResultValue);
		}
		else
		{
			m_queueResultValue.push_back(m_dResultValue);
		}

		OnPaint();
	}
	CDialogEx::OnTimer(nIDEvent);
}


// 當使用者拖曳最小化視窗時，
// 系統呼叫這個功能取得游標顯示。
HCURSOR CMFCStandardTimerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


// 開始繪製波形圖按鈕控件
void CMFCStandardTimerDlg::OnBnClickedButtonStart()
{
	// 開始計時
	m_bTimeStart = TRUE;

	// 紀錄開始時間
	m_dwTime = GetTickCount();

	// 每 0.01 秒更新模擬資料
	SetTimer(m_nTimerID, 10, nullptr);
}


// 停止繪製波形圖按鈕控件
void CMFCStandardTimerDlg::OnBnClickedButtonStop()
{
	// 停止計時
	m_bTimeStart = FALSE;

	// 將自變數時間歸零
	m_dSimTime = 0.0;

	// 停止 Timer 計時器
	KillTimer(m_nTimerID);

	// 取得停止時的佇列長度
	int iTotalQueueSize = m_queueResultValue.size();

	// 清空儲存的波形數值佇列
	if (m_queueResultValue.empty() != TRUE)
	{
		for (int i = 0; i < iTotalQueueSize; i++)
		{
			m_queueResultValue.pop_front();
		}
	}

	// 更新顯示區域
	CRect rectDrawShowRegion;
	GetDlgItem(IDC_STATIC_PAINT_REGION)->GetWindowRect(&rectDrawShowRegion);
	GetDlgItem(IDC_STATIC_PAINT_REGION)->GetParent()->ScreenToClient(rectDrawShowRegion);
	InvalidateRect(&rectDrawShowRegion, TRUE);
	UpdateWindow();

}


void CMFCStandardTimerDlg::OnBnClickedOk()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	CDialogEx::OnOK();
}


void CMFCStandardTimerDlg::OnBnClickedCancel()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	CDialogEx::OnCancel();
}

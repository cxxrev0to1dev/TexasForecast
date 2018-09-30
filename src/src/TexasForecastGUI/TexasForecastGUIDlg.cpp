
// TexasForecastGUIDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TexasForecastGUI.h"
#include "TexasForecastGUIDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
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


// CTexasForecastGUIDlg dialog



CTexasForecastGUIDlg::CTexasForecastGUIDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TEXASFORECASTGUI_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTexasForecastGUIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTexasForecastGUIDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
  ON_WM_LBUTTONDOWN()
  ON_WM_NCLBUTTONDOWN()
  ON_BN_CLICKED(IDC_BUTTON_RESET, &CTexasForecastGUIDlg::OnBnClickedButtonReset)
  ON_BN_CLICKED(IDC_BUTTON_CALC, &CTexasForecastGUIDlg::OnBnClickedButtonCalc)
END_MESSAGE_MAP()


// CTexasForecastGUIDlg message handlers

BOOL CTexasForecastGUIDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
 // ::SetWindowLong(m_hWnd, GWL_STYLE, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU);
  //::SetWindowLong(m_hWnd, GWL_STYLE, GetWindowLong(m_hWnd, GWL_STYLE)&~WS_SIZEBOX);
 // ::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE || SWP_NOSIZE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTexasForecastGUIDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
    if (nID == SC_MOVE || nID == 0xF012)
      return;
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTexasForecastGUIDlg::OnPaint()
{
	if (IsIconic()){
		CPaintDC dc(this); // device context for painting
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else{
    CString path = AfxGetApp()->m_pszHelpFilePath;
    CString str = AfxGetApp()->m_pszExeName;
    path = path.Left(path.GetLength() - str.GetLength() - 5);
    for (size_t i = 0; i < 52; i++){
      CString poker_res;
      poker_res.Format(L"%s\\poker\\%d.jpg", path, i + 3);
      ShowImage(IDC_STATIC1 + i, poker_res);
    }
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTexasForecastGUIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTexasForecastGUIDlg::ShowImage(int nID, const CString& path)
{
  CImage  image;
  int cx, cy;
  CRect   rect;
  image.Load(path);
  cx = image.GetWidth();
  cy = image.GetHeight();
  CWnd *pWnd = NULL;
  pWnd = GetDlgItem(nID);
  pWnd->GetClientRect(&rect);
  CDC *pDc = NULL;
  pDc = pWnd->GetDC();
  int ModeOld = SetStretchBltMode(pDc->m_hDC, STRETCH_HALFTONE);
  image.StretchBlt(pDc->m_hDC, rect, SRCCOPY);
  SetStretchBltMode(pDc->m_hDC, ModeOld);
  ReleaseDC(pDc);
}
void CTexasForecastGUIDlg::TexasCalcThread(void * arg) {
  CTexasForecastGUIDlg* xxxx = reinterpret_cast<CTexasForecastGUIDlg*>(arg);
  xxxx->Calc();
  if (xxxx->IsRefreshUI()){
    xxxx->Reset();
    ::SendMessageW(xxxx->m_hWnd, WM_PAINT, 0, 0);
  }
  xxxx->GetDlgItem(IDC_BUTTON_CALC)->EnableWindow(TRUE);
  _endthread();
}

void CTexasForecastGUIDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
  CPoint ptCursor;
  GetCursorPos(&ptCursor);
  CRect rc;
  for (size_t i = 0; i < 52; i++) {
    GetDlgItem(IDC_STATIC1 + i)->GetWindowRect(&rc);
    if (rc.PtInRect(ptCursor)) {
      if (!TexasForecast::IsSetFlop()){
        TexasForecast::SetFlop((PokerTable)(i + 1));
      }
      else if (!TexasForecast::IsSetTurn()){
        TexasForecast::SetTurn((PokerTable)(i + 1));
      }
      else if (!TexasForecast::IsSetRiver()){
        TexasForecast::SetRiver((PokerTable)(i + 1));
      }
      else {
        if(TexasForecast::IsSetFlop() && TexasForecast::IsSetTurn() && TexasForecast::IsSetRiver())
          break;
      }
      CRect rect;
      CDC *pDC = NULL;
      pDC = GetDlgItem(IDC_STATIC1 + i)->GetDC();
      GetDlgItem(IDC_STATIC1 + i)->GetClientRect(&rect);
      CBrush br(0xf0f0f0);
      pDC->FillRect(rect, &br);
      SetRect(rect, rect.left, rect.top, rect.right, rect.bottom);
      return;
    }
  }
  CDialogEx::OnLButtonDown(nFlags, point);
}


void CTexasForecastGUIDlg::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
  // TODO: Add your message handler code here and/or call default
  if (HTCAPTION == nHitTest) {
    return;
  }
  __super::OnNcLButtonDown(nHitTest, point);
}


void CTexasForecastGUIDlg::OnBnClickedButtonReset()
{
  TexasForecast::Reset();
  ::SendMessageW(m_hWnd, WM_PAINT, 0, 0);
  // TODO: Add your control notification handler code here
}


void CTexasForecastGUIDlg::OnBnClickedButtonCalc()
{
  GetDlgItem(IDC_BUTTON_CALC)->EnableWindow(FALSE);
  _beginthread(TexasCalcThread, 0, (void *)this);
  // TODO: Add your control notification handler code here
}

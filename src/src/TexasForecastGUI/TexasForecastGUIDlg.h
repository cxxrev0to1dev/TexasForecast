
// TexasForecastGUIDlg.h : header file
//

#pragma once
#include "TexasForecast/TexasForecast.h"
#pragma comment(lib,"TexasForecast.lib")


// CTexasForecastGUIDlg dialog
class CTexasForecastGUIDlg : public CDialogEx, TexasForecast
{
// Construction
public:
	CTexasForecastGUIDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TEXASFORECASTGUI_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
  void ShowImage(int nID,const CString& path);
  static void TexasCalcThread(void * arg);
public:
  afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
  afx_msg void OnBnClickedButtonReset();
  afx_msg void OnBnClickedButtonCalc();
};

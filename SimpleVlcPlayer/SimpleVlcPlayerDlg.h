
// SimpleVlcPlayerDlg.h : header file
//

#pragma once
#include "player/AVPlayer.h"
#include "afxwin.h"

// CSimpleVlcPlayerDlg dialog
class CSimpleVlcPlayerDlg : public CDialogEx
{
// Construction
public:
	CSimpleVlcPlayerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SIMPLEVLCPLAYER_DIALOG };
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
public:
	afx_msg void OnBnClickedButtonOpen();
	afx_msg void OnBnClickedButtonPlay();
	afx_msg void OnBnClickedButtonStop();
private:
	CAVPlayer m_myPlayer;		//	²¥·ÅÆ÷
	CString m_strFilePath;
	BOOL	m_bIsFullScreen;
public:
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	void SetFullScreen(BOOL full);
	CStatic m_playWnd;
};

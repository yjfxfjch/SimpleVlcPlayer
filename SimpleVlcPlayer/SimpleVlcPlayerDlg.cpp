
// SimpleVlcPlayerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SimpleVlcPlayer.h"
#include "SimpleVlcPlayerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HWND gPlayHwnd = NULL;
//枚举VLC窗口
BOOL CALLBACK EnumerateVLC(HWND hWndvlc, LPARAM lParam)
{
	TCHAR szWndTitle[1024];
	int nLen = GetWindowText(hWndvlc, szWndTitle, 1024);
	if (0 != nLen)
	{
		//禁用鼠标消息
		EnableWindow(hWndvlc, FALSE);
	}
	KillTimer(NULL, 1);
	return TRUE;
}

//定时器回调
void CALLBACK TimeProc(
	HWND hwnd,
	UINT message,
	UINT idTimer,
	DWORD dwTime)
{
	EnumChildWindows(gPlayHwnd, EnumerateVLC, NULL);
}
// CAboutDlg dialog used for App About
CStringA UnicodeToUTF8(const CStringW& strWide);

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


// CSimpleVlcPlayerDlg dialog



CSimpleVlcPlayerDlg::CSimpleVlcPlayerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SIMPLEVLCPLAYER_DIALOG, pParent),
	m_strFilePath(L""),
	m_bIsFullScreen(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSimpleVlcPlayerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PLAYWND, m_playWnd);
}

BEGIN_MESSAGE_MAP(CSimpleVlcPlayerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CSimpleVlcPlayerDlg::OnBnClickedButtonOpen)
	ON_BN_CLICKED(IDC_BUTTON_PLAY, &CSimpleVlcPlayerDlg::OnBnClickedButtonPlay)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CSimpleVlcPlayerDlg::OnBnClickedButtonStop)
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()


// CSimpleVlcPlayerDlg message handlers

BOOL CSimpleVlcPlayerDlg::OnInitDialog()
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
	gPlayHwnd = GetDlgItem(IDC_PLAYWND)->GetSafeHwnd();
	m_myPlayer.SetHWND(gPlayHwnd);   // 设置播放器的窗口句柄

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSimpleVlcPlayerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSimpleVlcPlayerDlg::OnPaint()
{
	if (IsIconic())
	{
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
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSimpleVlcPlayerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CSimpleVlcPlayerDlg::OnBnClickedButtonOpen()
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	//设置过滤器   
	TCHAR szFilter[] = _T("视频文件|*.mp4;*.rmvb;*.mkv|所有文件(*.*)|*.*||");
	// 构造打开文件对话框   
	CFileDialog fileDlg(TRUE, _T("选择视频文件"), NULL, 0, szFilter, this);

	// 显示打开文件对话框   
	if (IDOK == fileDlg.DoModal())
	{
		// 如果点击了文件对话框上的“打开”按钮，则将选择的文件路径显示到编辑框里   
		m_strFilePath = fileDlg.GetPathName();
		SetDlgItemText(IDC_EDIT_PATH, m_strFilePath);
	}
	
}


void CSimpleVlcPlayerDlg::OnBnClickedButtonPlay()
{
	// TODO: Add your control notification handler code here
	CString strBtnName = L"";
	GetDlgItemText(IDC_BUTTON_PLAY, strBtnName);
	if (L"播放" == strBtnName)
	{
		if (!m_myPlayer.IsOpen())
		{
			if (m_strFilePath != L"")
			{
				m_myPlayer.Play((LPCSTR)UnicodeToUTF8(m_strFilePath));
				::SetTimer(NULL, 1, 1000, TimeProc);
				SetDlgItemText(IDC_BUTTON_PLAY, L"暂停");
			}
		}
		if (!m_myPlayer.IsPlaying() && m_myPlayer.IsOpen())
		{
			m_myPlayer.Play();
			SetDlgItemText(IDC_BUTTON_PLAY, L"暂停");
		}
	}
	else
	{
		if (m_myPlayer.IsPlaying())
		{
			m_myPlayer.Pause();
			SetDlgItemText(IDC_BUTTON_PLAY, L"播放");
		}
	}
}


void CSimpleVlcPlayerDlg::OnBnClickedButtonStop()
{
	// TODO: Add your control notification handler code here
}


void CSimpleVlcPlayerDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	SetFullScreen(!m_bIsFullScreen);
	CDialogEx::OnLButtonDblClk(nFlags, point);
}

void CSimpleVlcPlayerDlg::SetFullScreen(BOOL full)
{
	m_bIsFullScreen = full;

	if (full)
	{
		ShowWindow(SW_MAXIMIZE);
		CRect rc;
		GetClientRect(&rc);
		m_playWnd.MoveWindow(rc, 1);
	}
	else
	{
		ShowWindow(SW_NORMAL);
		CRect rc;
		GetClientRect(&rc);
		rc.DeflateRect(0, 0, 0, 80);
		m_playWnd.MoveWindow(rc, 1);

	}
}

CStringA UnicodeToUTF8(const CStringW& strWide)
{
	CStringA strUTF8;
	int nLen = ::WideCharToMultiByte(CP_UTF8, 0, strWide, -1, NULL, 0, NULL, NULL);

	if (nLen > 1)
	{
		::WideCharToMultiByte(CP_UTF8, 0, strWide, -1, strUTF8.GetBuffer(nLen - 1), nLen, NULL, NULL);
		strUTF8.ReleaseBuffer();
	}

	return strUTF8;
}
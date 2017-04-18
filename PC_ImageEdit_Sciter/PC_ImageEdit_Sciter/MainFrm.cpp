
// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "ImageEditApp.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_CLOSE()
	ON_MESSAGE(WM_SHOWTASK, onShowTask)
	ON_COMMAND(ID_TRAY_SHOW, &CMainFrame::OnTrayShow)
	ON_COMMAND(ID_TRAY_HIDE, &CMainFrame::OnTrayHide)
	ON_COMMAND(ID_TRAY_EXIT, &CMainFrame::OnTrayExit)
	ON_WM_GETMINMAXINFO()
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
	// TODO: 在此添加成员初始化代码
}

CMainFrame::~CMainFrame()
{
}
void InitConsoleWindow()
{
	AllocConsole();
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	int hCrt = _open_osfhandle((long)handle, _O_TEXT);
	FILE * hf = _fdopen(hCrt, "w");
	*stdout = *hf;
}
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	SetWindowText(_T("17zuoye OCR ImageEdit"));
	InitConsoleWindow();  // add
	_cprintf("mfc init created\n");
	
	//this->SetMenu(NULL);
	// 创建一个视图以占用框架的工作区
	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("未能创建视图窗口\n");
		return -1;
	}
	HICON m_hIcon = NULL;
	m_hIcon = AfxGetApp()->LoadIcon("E:\\C++Project_Git\\ImageEdit_Sciter\\PC_ImageEdit_Sciter\\Release\\res\\ico\\mainFrame.ico");
	SetIcon(m_hIcon, TRUE); // Set big icon 设置大图标
	SetIcon(m_hIcon, FALSE); // Set small icon 设置小图标
	
							 //设置窗口居中
	SetWindowPos(NULL, 0, 0, 1400, 700, SWP_NOMOVE);
	CenterWindow(GetDesktopWindow());
	//TODO 添加自定义代码
	OnCreateTray(m_hIcon, "试卷扫描客户端", "");
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}

// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG

//自定义消息机制
LRESULT CMainFrame::onShowTask(WPARAM wParam, LPARAM lParam)
{

	if (lParam == WM_RBUTTONUP)
	{
		LPPOINT point = new tagPOINT;
		::GetCursorPos(point);
		CMenu menu;
		menu.LoadMenu(IDR_TRAY_MENU);
		CMenu *pSubMenu = menu.GetSubMenu(0);
		SetForegroundWindow();
		pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, point->x, point->y, this);
		delete point;
	}
	else if (lParam == WM_LBUTTONDBLCLK)
	{
		ShowWindow(SW_SHOWNORMAL);
		UpdateWindow();
		ShowWindow(SW_SHOW);
		UpdateWindow();
	}
	return 0;
}
//创建托盘以及提示
void CMainFrame::OnCreateTray(HICON hIcon, CHAR* szTip, CHAR *szInfo)
{
	Shell_NotifyIcon(NIM_DELETE, &m_nid);
	m_nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
	m_nid.hWnd = this->m_hWnd;
	m_nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP | NIF_INFO;
	m_nid.uCallbackMessage = WM_SHOWTASK; //应用程序定义的消息ID号，此消息传递给hWnd
	m_nid.hIcon = hIcon;
	strcpy_s(m_nid.szTip, szTip);
	strcpy_s(m_nid.szInfo, szInfo);
	strcpy_s(m_nid.szInfoTitle, "提示");
	m_nid.dwInfoFlags = NIIF_INFO;
	m_nid.uTimeout = 1000;
	Shell_NotifyIcon(NIM_ADD, &m_nid);
}
// CMainFrame 消息处理程序

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	// 将焦点前移到视图窗口
	m_wndView.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// 让视图第一次尝试该命令
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// 否则，执行默认处理
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CMainFrame::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int result = MessageBox("确定退出程序？","提示",MB_YESNO | MB_ICONQUESTION);
	if (result == IDYES)
	{
		Shell_NotifyIcon(NIM_DELETE, &m_nid);
		CFrameWnd::OnClose();
	}
}


void CMainFrame::OnTrayShow()
{
	ShowWindow(SW_SHOWNORMAL);
	UpdateWindow();
	ShowWindow(SW_SHOW);
	UpdateWindow();
}


void CMainFrame::OnTrayHide()
{
	ShowWindow(SW_HIDE);
	UpdateWindow();
}

void CMainFrame::OnTrayExit()
{
	AfxGetMainWnd()->PostMessage(WM_CLOSE);
}


void CMainFrame::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	lpMMI->ptMinTrackSize.x = 800;   //x宽度    
	lpMMI->ptMinTrackSize.y = 600;   //y高度    
	
	CFrameWnd::OnGetMinMaxInfo(lpMMI);
}


void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	m_wndView.onResizeView(cx, cy);

	CFrameWnd::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
}

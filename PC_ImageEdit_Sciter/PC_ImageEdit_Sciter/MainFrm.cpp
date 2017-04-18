
// MainFrm.cpp : CMainFrame ���ʵ��
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

// CMainFrame ����/����

CMainFrame::CMainFrame()
{
	// TODO: �ڴ���ӳ�Ա��ʼ������
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
	// ����һ����ͼ��ռ�ÿ�ܵĹ�����
	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("δ�ܴ�����ͼ����\n");
		return -1;
	}
	HICON m_hIcon = NULL;
	m_hIcon = AfxGetApp()->LoadIcon("E:\\C++Project_Git\\ImageEdit_Sciter\\PC_ImageEdit_Sciter\\Release\\res\\ico\\mainFrame.ico");
	SetIcon(m_hIcon, TRUE); // Set big icon ���ô�ͼ��
	SetIcon(m_hIcon, FALSE); // Set small icon ����Сͼ��
	
							 //���ô��ھ���
	SetWindowPos(NULL, 0, 0, 1400, 700, SWP_NOMOVE);
	CenterWindow(GetDesktopWindow());
	//TODO ����Զ������
	OnCreateTray(m_hIcon, "�Ծ�ɨ��ͻ���", "");
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}

// CMainFrame ���

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

//�Զ�����Ϣ����
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
//���������Լ���ʾ
void CMainFrame::OnCreateTray(HICON hIcon, CHAR* szTip, CHAR *szInfo)
{
	Shell_NotifyIcon(NIM_DELETE, &m_nid);
	m_nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
	m_nid.hWnd = this->m_hWnd;
	m_nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP | NIF_INFO;
	m_nid.uCallbackMessage = WM_SHOWTASK; //Ӧ�ó��������ϢID�ţ�����Ϣ���ݸ�hWnd
	m_nid.hIcon = hIcon;
	strcpy_s(m_nid.szTip, szTip);
	strcpy_s(m_nid.szInfo, szInfo);
	strcpy_s(m_nid.szInfoTitle, "��ʾ");
	m_nid.dwInfoFlags = NIIF_INFO;
	m_nid.uTimeout = 1000;
	Shell_NotifyIcon(NIM_ADD, &m_nid);
}
// CMainFrame ��Ϣ�������

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	// ������ǰ�Ƶ���ͼ����
	m_wndView.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// ����ͼ��һ�γ��Ը�����
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// ����ִ��Ĭ�ϴ���
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CMainFrame::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	int result = MessageBox("ȷ���˳�����","��ʾ",MB_YESNO | MB_ICONQUESTION);
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	lpMMI->ptMinTrackSize.x = 800;   //x���    
	lpMMI->ptMinTrackSize.y = 600;   //y�߶�    
	
	CFrameWnd::OnGetMinMaxInfo(lpMMI);
}


void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	m_wndView.onResizeView(cx, cy);

	CFrameWnd::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
}

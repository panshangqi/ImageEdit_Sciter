
// ChildView.cpp : CChildView 类的实现
//


#include "stdafx.h"
#include "ImageEditApp.h"
#include "MainFrm.h"
#include "ChildView.h"
#include "Pdf2Png.h"
#include "MuPDFConvert.h"
#include "StrCoding.h"
#include "CopyImageToClipboard.h"


DocSlipt *docSlipt;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CMainFrame *mainFrame;
std::vector<CString>m_fileList;
std::vector<CString>m_cutImageList;
sciter::dom::element m_root;
// CChildView
char* WcharToChar(const wchar_t* wp);
wchar_t* CharToWchar(const char* c);

CChildView::CChildView()
{
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_CLOSE()
	ON_WM_KEYDOWN()
	ON_WM_CREATE()

	ON_COMMAND(ID_OPEN_PDF, &CChildView::OnOpenPdf)
	ON_COMMAND(ID_OPEN_IMAGE, &CChildView::OnOpenImage)
	ON_COMMAND(ID_APP_EXIT, &CChildView::OnAppExit)
	ON_COMMAND(ID_CLOSE_DOC, &CChildView::OnCloseDoc)
END_MESSAGE_MAP()

const double m_imgRate = 0.5; //原图和压缩图的比例

// CChildView 消息处理程序
int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
		return -1;
	this->load_file(L"file://res/index.html");
	this->setup_callback(); // attach sciter::host callbacks
	sciter::attach_dom_event_handler(this->get_hwnd(), this); // attach this as a DOM events 									  
															  
	mainFrame = (CMainFrame*)this->GetParentFrame();
	
	docSlipt = new DocSlipt();
	docSlipt->init();
	return 0;
}
BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return TRUE;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this); // 用于绘制的设备上下文
	
	// TODO: 在此处添加消息处理程序代码
	
	// 不要为绘制消息而调用 CWnd::OnPaint()
}
HWINDOW CChildView::get_hwnd() {
	
	return this->GetSafeHwnd();
}
HINSTANCE CChildView::get_resource_instance()
{
	return theApp.m_hInstance;
}
void CChildView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (nChar == VK_F5)
	{
		this->load_file(L"file://res/index.html");
	}
	__super::OnKeyDown(nChar, nRepCnt, nFlags);
}
LRESULT CChildView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	
	LRESULT lResult;
	BOOL    bHandled = FALSE;

	lResult = SciterProcND(this->GetSafeHwnd(), message, wParam, lParam, &bHandled);
	if (bHandled)      // if it was handled by the Sciter
	{
		return lResult; // then no further processing is required.
	}
	
	return __super::WindowProc(message, wParam, lParam);
	
}
void CChildView::OnClose()
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	__super::OnClose();
}

sciter::value CChildView::getPicPxList()
{
	sciter::value arr[200];
	for (int i = 0; i < (int)m_fileList.size(); i++)
	{
		cv::Mat img = cv::imread(m_fileList[i].GetBuffer());
		int imgWidth = (int)(img.cols*m_imgRate);
		int imgHeight = (int)(img.rows*m_imgRate);
		sciter::value item[3];
		item[0] = sciter::value(imgWidth);
		item[1] = sciter::value(imgHeight);
		arr[i] = sciter::value(item, 2);
	}
	return sciter::value(arr,m_fileList.size());
}
struct cut_params
{
	sciter::value cut_questions_list;
	sciter::value cut_graphics_list;
};
void thread_ocr(cut_params params)
{
	
	m_root.call_function("get_OCR_init");
	
	TCHAR appDataPath[MAX_PATH];
	TCHAR tempDir[MAX_PATH];
	SHGetSpecialFolderPath(NULL, appDataPath, CSIDL_APPDATA, FALSE);
	sprintf(tempDir, "%s\\17zuoye", appDataPath);
	CreateDirectory(tempDir, NULL);
	sprintf(tempDir, "%s\\17zuoye\\cache", appDataPath);
	CreateDirectory(tempDir, NULL);
	//AfxMessageBox("haha");
	int pageNum = params.cut_questions_list.length();

	int count = 0;
	m_cutImageList.clear();
	sciter::value result[1000];
	int resultNum = 1;
	for (int i = 0; i < pageNum; i++)
	{
		sciter::value page_questions_list = params.cut_questions_list[i];
		sciter::value page_graphics_list = params.cut_graphics_list[i];

		cv::Mat img = cv::imread(m_fileList[i].GetBuffer());
		std::vector<regRect>tempQuestions;
		std::vector<regRect>tempGraphics;
		//遍历题目列表
		for (int j = 0; j < page_questions_list.length(); j++)
		{
			sciter::value item = page_questions_list[j];
			int g_left = sciter::value(item.get_item(0)).d;
			int g_top = sciter::value(item.get_item(1)).d;
			int g_width = sciter::value(item.get_item(2)).d;
			int g_height = sciter::value(item.get_item(3)).d;
			double g_rate = img.cols / 700.0;
			regRect rRect;
			rRect.x = (int)(g_left*g_rate);
			rRect.y = (int)(g_top*g_rate);
			rRect.w = (int)(g_width*g_rate);
			rRect.h = (int)(g_height*g_rate);

			tempQuestions.push_back(rRect);
			cv::Mat cutImg = img(cv::Rect(rRect.x,rRect.y,rRect.w,rRect.h));
			TCHAR savePath[256];
			sprintf(savePath, "%s\\cut_%d.jpg", tempDir, count++);
			m_cutImageList.push_back(savePath);
			cv::imwrite(savePath, cutImg);
			Sleep(100);
		}
		//遍历图形列表
		_cprintf("%d\n", page_graphics_list.length());
		for (int j = 0; j < page_graphics_list.length(); j++)
		{
			sciter::value item = page_graphics_list[j];
			int g_left = sciter::value(item.get_item(0)).d;
			int g_top = sciter::value(item.get_item(1)).d;
			int g_width = sciter::value(item.get_item(2)).d;
			int g_height = sciter::value(item.get_item(3)).d;
			double g_rate = img.cols / 700.0;
			regRect rRect;
			rRect.x = (int)(g_left*g_rate);
			rRect.y = (int)(g_top*g_rate);
			rRect.w = (int)(g_width*g_rate);
			rRect.h = (int)(g_height*g_rate);
			_cprintf("%d %d %d %d\n", rRect.x, rRect.h, rRect.w, rRect.h);
			tempGraphics.push_back(rRect);
		}

		std::vector<regRect>graphs;
		std::vector<regRect>questions;
		docSlipt->getRect(m_fileList[i].GetBuffer(), questions, graphs);

		std::vector<string> resultText;
		std::vector<int> questionNo;

		int curNum = docSlipt->getNum(tempQuestions, tempGraphics, questionNo);
		
		FILE *writeFile = fopen("result.txt", "w");
		for (int j = 0; j < (int)questionNo.size(); j++)
		{
			string resultText = docSlipt->getOneOcr(j);
			fputs(resultText.c_str(), writeFile);
			const wchar_t *wText = CharToWchar(resultText.c_str());
			result[resultNum] = sciter::value(wText);
			m_root.call_function("get_OCR_Pb", resultNum, sciter::value(wText));
			resultNum++;
		}
		fclose(writeFile);
		
	}
	m_root.call_function("get_OCR_Done");
	/*
	sciter::value array[1000];
	strCoding url;
	for (int i = 0; i < (int)m_cutImageList.size(); i++)
	{
	string urlEncode = url.UrlPathEncode(m_cutImageList[i].GetBuffer());
	array[i] = sciter::value(urlEncode.c_str());
	}
	return sciter::value(array, count);
	*/
	
	//return sciter::value(array, arr_num);
}
sciter::value CChildView::getImageCutList(sciter::value cut_questions_list, sciter::value cut_graphics_list)
{
	m_root = this->get_root();
	cut_params params;
	params.cut_questions_list = cut_questions_list;
	params.cut_graphics_list = cut_graphics_list;
	sciter::thread(thread_ocr, params);
	return sciter::value("true");
}

void thread_open_pdf(sciter::value g_filePath)
{
	aux::wchars pp = g_filePath.get_chars();
	wchar_t wPdfPath[MAX_PATH];
	swprintf(wPdfPath,L"%s",pp);
	
	std::vector<CString> tempFileList;
	TCHAR appDataPath[MAX_PATH];
	TCHAR tempDir[MAX_PATH];
	SHGetSpecialFolderPath(NULL, appDataPath, CSIDL_APPDATA, FALSE);
	sprintf(tempDir, "%s\\17zuoye", appDataPath);
	CreateDirectory(tempDir, NULL);
	sprintf(tempDir, "%s\\17zuoye\\cache", appDataPath);
	CreateDirectory(tempDir, NULL);

	CMuPDFConvert pdfConvert;
	int nNum = pdfConvert.getPageNumber(wPdfPath, tempDir, "temp");

	char resultPath[300];
	char resizePath[300];
	m_fileList.clear();
	strCoding url;
	cv::Mat img, reimg;
	
	m_root.call_function("loadProgresInit");
	//组建json数组
	//题目数组
	sciter::value ocr_questionsList[400];
	int ocr_questionsNum = 0;
	//图形数组
	sciter::value ocr_graphicsList[100];
	int ocr_graphicsNum = 0;

	for (int i = 0; i < nNum; i++)
	{
		pdfConvert.getPngFromPage(i);
		//原尺寸图片
		sprintf(resultPath, "%s\\temp%d.png", tempDir, i + 1);
		//经过压缩的图片
		sprintf(resizePath, "%s\\resize%d.jpg", tempDir, i + 1);
		img = cv::imread(resultPath);
		int rows = (int)(img.rows*m_imgRate);
		int cols = (int)(img.cols*m_imgRate);
		cv::resize(img, reimg, cv::Size(cols, rows));
		cv::imwrite(resizePath, reimg);
		m_fileList.push_back(resultPath);
		string urlEncode = url.UrlPathEncode(resizePath);

		m_root.call_function("loadProgressCb",i + 1, nNum, urlEncode.c_str()); 

		//检测
		std::vector<regRect>graphs;
		std::vector<regRect>questions;
		_cprintf("%s\n", resultPath);
		if (img.cols>1250 && img.rows>2100)
		{
			docSlipt->getRect(resultPath, questions, graphs);
		}
		double ocr_rate = 700.0 / img.cols;
		//每张图的图
		sciter::value arr[100];
		int arr_num = 0;

		for (int j = 0; j < (int)questions.size(); j++)
		{
			sciter::value item[5];
			item[0] = sciter::value((int)(questions[j].x*ocr_rate));
			item[1] = sciter::value((int)(questions[j].y*ocr_rate));
			item[2] = sciter::value((int)(questions[j].w*ocr_rate));
			item[3] = sciter::value((int)(questions[j].h*ocr_rate));
			arr[arr_num++] = sciter::value(item, 4);
		}
		//arr_num + 1 ： 防止为0；
		ocr_questionsList[ocr_questionsNum++] = sciter::value(arr, arr_num+1);

		arr_num = 0;
		for (int j = 0; j < (int)graphs.size(); j++)
		{
			sciter::value item[5];
			item[0] = sciter::value((int)(graphs[j].x*ocr_rate));
			item[1] = sciter::value((int)(graphs[j].y*ocr_rate));
			item[2] = sciter::value((int)(graphs[j].w*ocr_rate));
			item[3] = sciter::value((int)(graphs[j].h*ocr_rate));
			arr[arr_num++] = sciter::value(item, 4);
		}
		ocr_graphicsList[ocr_graphicsNum++] = sciter::value(arr, arr_num+1);
	}

	m_root.call_function("loadProgressDone", sciter::value(ocr_questionsList, ocr_questionsNum), 
		sciter::value(ocr_graphicsList, ocr_graphicsNum));

	pdfConvert.freeMemory();
}
void thread_open_image(sciter::dom::element root)
{
	TCHAR appDataPath[MAX_PATH];
	TCHAR tempDir[MAX_PATH];
	SHGetSpecialFolderPath(NULL, appDataPath, CSIDL_APPDATA, FALSE);
	sprintf(tempDir, "%s\\17zuoye", appDataPath);
	CreateDirectory(tempDir, NULL);
	sprintf(tempDir, "%s\\17zuoye\\cache", appDataPath);
	CreateDirectory(tempDir, NULL);
	const int len = 300;
	sciter::value arr[len];
	strCoding url;
	cv::Mat img, reimg;
	root.call_function("loadProgresInit");
	int fileSize = (int)m_fileList.size();

	//组建json数组
	//题目数组
	sciter::value ocr_questionsList[400];
	int ocr_questionsNum = 0;
	//图形数组
	sciter::value ocr_graphicsList[100];
	int ocr_graphicsNum = 0;
	for (int i = 0; i < fileSize; i++)
	{
		
		CString tempPath = m_fileList[i];
		//获取文件名
		CString tempFileName = tempPath.Mid(tempPath.ReverseFind('\\') + 1);
		CString copyPath = tempDir;
		copyPath += "\\";
		copyPath += tempFileName;
		CopyFile(tempPath, copyPath, false);
		CString resizePath = tempDir;
		resizePath += "\\resize_";
		resizePath += tempFileName;
		img = cv::imread(copyPath.GetBuffer());
		int rows = (int)(img.rows*m_imgRate);
		int cols = (int)(img.cols*m_imgRate);
		cv::resize(img, reimg, cv::Size(cols, rows));
		cv::imwrite(resizePath.GetBuffer(), reimg);

		string urlEncode = url.UrlPathEncode(resizePath.GetBuffer());
		arr[i] = sciter::value(urlEncode.c_str());
		::Sleep(100);
		
		//检测
		std::vector<regRect>graphs;
		std::vector<regRect>questions;
		if (img.cols>1250 && img.rows>2100)
		{
			docSlipt->getRect(m_fileList[i].GetBuffer(), questions, graphs);
		}
		double ocr_rate = 700.0 / img.cols;
		//每张图的图
		sciter::value arr[100];
		int arr_num = 0;

		for (int j = 0; j < (int)questions.size(); j++)
		{
			sciter::value item[5];
			item[0] = sciter::value((int)(questions[j].x*ocr_rate));
			item[1] = sciter::value((int)(questions[j].y*ocr_rate));
			item[2] = sciter::value((int)(questions[j].w*ocr_rate));
			item[3] = sciter::value((int)(questions[j].h*ocr_rate));
			arr[arr_num++] = sciter::value(item, 4);
		}
		//arr_num + 1 ： 防止为0；
		ocr_questionsList[ocr_questionsNum++] = sciter::value(arr, arr_num + 1);

		arr_num = 0;
		for (int j = 0; j < (int)graphs.size(); j++)
		{
			sciter::value item[5];
			item[0] = sciter::value((int)(graphs[j].x*ocr_rate));
			item[1] = sciter::value((int)(graphs[j].y*ocr_rate));
			item[2] = sciter::value((int)(graphs[j].w*ocr_rate));
			item[3] = sciter::value((int)(graphs[j].h*ocr_rate));
			arr[arr_num++] = sciter::value(item, 4);
		}
		_cprintf("%d %d %d %d\n", questions.size(), graphs.size());
		ocr_graphicsList[ocr_graphicsNum++] = sciter::value(arr, arr_num + 1);
		root.call_function("loadProgressCb", i + 1, fileSize, urlEncode.c_str());
		
	}
	root.call_function("loadProgressDone", sciter::value(ocr_questionsList, ocr_questionsNum),
		sciter::value(ocr_graphicsList, ocr_graphicsNum));

}
void CChildView::OnOpenPdf()
{
	m_root = this->get_root();
	m_root.call_function("QueryDialog");
	CString defaultDir = "C:\\";   //默认打开的文件路径  
	CString fileName = "";         //默认打开的文件名  
	CString filter = "文件 (*.pdf)|*.pdf||";   //文件过虑的类型  
	CFileDialog openFileDlg(TRUE, defaultDir, fileName, OFN_HIDEREADONLY, filter, NULL);
	
	INT_PTR result = openFileDlg.DoModal();
	
	CString pdfPath;
	
	if (result == IDOK) {
		
		pdfPath = openFileDlg.GetPathName();
		sciter::thread(thread_open_pdf,sciter::value(pdfPath.GetBuffer()));
	}
	
}
void CChildView::OnOpenImage()
{
	sciter::dom::element root = this->get_root();
	root.call_function("QueryDialog");

	CString defaultDir = "C:\\";   //默认打开的文件路径  
	CString fileName = "";         //默认打开的文件名  
	CString filter = "文件 (*.jpg; *.jpeg; *.png; *.bmp)|*.jpg;*.jpeg;*.png;*.bmp||";   //文件过虑的类型  
	CFileDialog openFileDlg(TRUE, defaultDir, fileName, OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT, filter, NULL);
	//openFileDlg.GetOFN().lpstrInitialDir = "E:\\FileTest\\test.doc";
	INT_PTR result = openFileDlg.DoModal();
	CString filePath;

	if (result == IDOK) {
		
		m_fileList.clear();
		POSITION pos = openFileDlg.GetStartPosition();
		while (pos != NULL)
		{
			filePath = openFileDlg.GetNextPathName(pos);
			m_fileList.push_back(filePath);
		}
		sciter::thread(thread_open_image, root);
	}
}

void copy_file_to_clipboard(const char *filename)
{
	CopyImageToClipboard citc;
	//citc.CopyTextToClipboard("hahah");
	citc.CopyFileToClipboard(filename);
}
//ctrl+c 复制图片
struct copyed_thread_params
{
	json::value copyedInit;
	json::value copyedDone;
};
void copyed_thread(copyed_thread_params params)
{

	params.copyedInit.call(0);
	::Sleep(1400);
	params.copyedDone.call(0);
	::Sleep(800);
	params.copyedDone.call(1);
}
json::value CChildView::copyBmpToClipboard(json::value pageid, json::value rect, json::value copyedInit, json::value copyedDone)
{
	TCHAR appDataPath[MAX_PATH];
	TCHAR tempDir[MAX_PATH];
	SHGetSpecialFolderPath(NULL, appDataPath, CSIDL_APPDATA, FALSE);
	sprintf(tempDir, "%s\\17zuoye", appDataPath);
	CreateDirectory(tempDir, NULL);
	sprintf(tempDir, "%s\\17zuoye\\cache", appDataPath);
	CreateDirectory(tempDir, NULL);
	TCHAR copyPath[MAX_PATH];
	sprintf(copyPath, "%s\\copycut.jpg", tempDir);
	int pId = pageid.d;
	cv::Mat img = cv::imread(m_fileList[pId].GetBuffer());
	int g_width = img.cols;
	double g_rate = g_width / 700.0;
	int m_left = json::value(rect[0]).d * g_rate;
	int m_top = json::value(rect[1]).d * g_rate;
	int m_width = json::value(rect[2]).d * g_rate;
	int m_height = json::value(rect[3]).d * g_rate;
	
	_cprintf("%d %d %d %d\n",m_left,m_top,m_width,m_height);
	
	cv::Mat cutImg = img(cv::Rect(m_left, m_top, m_width, m_height));
	cv::imwrite(copyPath, cutImg);
	sciter::thread(copy_file_to_clipboard, copyPath);
	
	copyed_thread_params params;
	params.copyedInit = copyedInit;
	params.copyedDone = copyedDone;
	sciter::thread(copyed_thread, params);
	
	return json::value("true");
	
}
//HTML交互

json::value CChildView::loadWeb()
{

	this->load_file(L"file://res/index.html");
	return json::value(0);
}

char* WcharToChar(const wchar_t* wp)
{
	char *m_char;
	int len = WideCharToMultiByte(CP_UTF8, 0, wp, wcslen(wp), NULL, 0, NULL, NULL);
	m_char = new char[len + 1];
	WideCharToMultiByte(CP_UTF8, 0, wp, wcslen(wp), m_char, len, NULL, NULL);
	m_char[len] = '\0';
	return m_char;
}
wchar_t* CharToWchar(const char* c)
{
	wchar_t *m_wchar;
	int len = MultiByteToWideChar(CP_UTF8, 0, c, strlen(c), NULL, 0);
	m_wchar = new wchar_t[len + 1];
	MultiByteToWideChar(CP_UTF8, 0, c, strlen(c), m_wchar, len);
	m_wchar[len] = '\0';
	return m_wchar;
}

void CChildView::OnAppExit()
{
	AfxGetMainWnd()->PostMessage(WM_CLOSE);
}


void CChildView::OnCloseDoc()
{
	
	sciter::dom::element root = this->get_root();
	root.call_function("QueryDialog");
}

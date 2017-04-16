#include "stdafx.h"
#include "Pdf2Png.h"
#include "MuPDFConvert.h"
using namespace std;
#pragma comment(lib,"mupdf/libmupdf.lib")
char* WcharToChar(const wchar_t* wp);
wchar_t* CharToWchar(const char* c);

Pdf2Png::Pdf2Png()
{
}


Pdf2Png::~Pdf2Png()
{
}

std::vector<CString> Pdf2Png::runPdfToPng(char* pdfPath)
{
	std::vector<CString> tempFileList;
	TCHAR appDataPath[MAX_PATH];
	TCHAR tempDir[MAX_PATH];
	TCHAR filePath[MAX_PATH];
	SHGetSpecialFolderPath(NULL, appDataPath, CSIDL_APPDATA, FALSE);
	sprintf(tempDir, "%s\\17zuoye", appDataPath);
	CreateDirectory(tempDir, NULL);
	sprintf(tempDir, "%s\\17zuoye\\cache", appDataPath);
	CreateDirectory(tempDir, NULL);

	wchar_t *buff = CharToWchar(pdfPath);
	CMuPDFConvert pdfConvert;
	int nNum = 0;
	pdfConvert.Pdf2Png(buff, tempDir, "temp", nNum);

	for (int i = 1; i <= nNum; i++)
	{
		sprintf(filePath, "%s\\temp%d.png", tempDir, i);
		tempFileList.push_back(filePath);
	}
	_cprintf("change over\n");
	return tempFileList;
}
char* Pdf2Png::WcharToChar(const wchar_t* wp)
{
	char *m_char;
	int len = WideCharToMultiByte(CP_ACP, 0, wp, wcslen(wp), NULL, 0, NULL, NULL);
	m_char = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, wp, wcslen(wp), m_char, len, NULL, NULL);
	m_char[len] = '\0';
	return m_char;
}
wchar_t* Pdf2Png::CharToWchar(const char* c)
{
	wchar_t *m_wchar;
	int len = MultiByteToWideChar(CP_ACP, 0, c, strlen(c), NULL, 0);
	m_wchar = new wchar_t[len + 1];
	MultiByteToWideChar(CP_ACP, 0, c, strlen(c), m_wchar, len);
	m_wchar[len] = '\0';
	return m_wchar;
}
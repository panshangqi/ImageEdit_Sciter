#include "stdafx.h"
#include "CopyImageToClipboard.h"
#include <vector>
#include "ThreedPool.h"
using namespace std;

CopyImageToClipboard::CopyImageToClipboard()
{
}


CopyImageToClipboard::~CopyImageToClipboard()
{
}

bool CopyImageToClipboard::CopyFileToClipboard(const char *szFileName)
{
	
	UINT uDropEffect;
	HGLOBAL hGblEffect;
	LPDWORD lpdDropEffect;
	DROPFILES stDrop;
	HGLOBAL hGblFiles;
	LPSTR lpData;
	uDropEffect = RegisterClipboardFormat("Preferred DropEffect");
	hGblEffect = GlobalAlloc(GMEM_ZEROINIT | GMEM_MOVEABLE | GMEM_DDESHARE, sizeof(DWORD));
	lpdDropEffect = (LPDWORD)GlobalLock(hGblEffect);
	*lpdDropEffect = DROPEFFECT_MOVE;//复制; 剪贴则用DROPEFFECT_MOVE

	GlobalUnlock(hGblEffect);
	stDrop.pFiles = sizeof(DROPFILES);
	stDrop.pt.x = 0;
	stDrop.pt.y = 0;
	stDrop.fNC = FALSE;
	stDrop.fWide = FALSE;
	hGblFiles = GlobalAlloc(GMEM_ZEROINIT | GMEM_MOVEABLE | GMEM_DDESHARE, \
		sizeof(DROPFILES) + strlen(szFileName) + 2);
	lpData = (LPSTR)GlobalLock(hGblFiles);
	memcpy(lpData, &stDrop, sizeof(DROPFILES));
	strcpy(lpData + sizeof(DROPFILES), szFileName);
	GlobalUnlock(hGblFiles);
	OpenClipboard(NULL);
	EmptyClipboard();
	SetClipboardData(CF_HDROP, hGblFiles);
	SetClipboardData(uDropEffect, hGblEffect);
	CloseClipboard();
	printf("over\n");
	return true;
}
bool CopyImageToClipboard::CopyTextToClipboard(const char *text)
{

	if (!OpenClipboard(NULL) || !EmptyClipboard())
	{
		_cprintf("open clipboard fail\n");
		return -1;
	}
	HGLOBAL hMen;
	hMen = GlobalAlloc(GMEM_MOVEABLE, ((strlen(text) + 1) * sizeof(char)));
	if (!hMen)
	{
		_cprintf("global fail\n");
		CloseClipboard();
		return -1;
	}
	LPSTR lpStr = (LPSTR)GlobalLock(hMen);
	memcpy(lpStr, text, ((strlen(text)) * sizeof(char)));
	lpStr[strlen(text)] = (char)0;
	GlobalUnlock(hMen);
	SetClipboardData(CF_TEXT, hMen);
	CloseClipboard();
	_cprintf("copy successful\n");
	return -1;
	
/*
	vector<CString>vecFiles;
	vecFiles.push_back("timg.jpg");
	UINT  uDropEffect = RegisterClipboardFormat(_T("Preferred DropEffect"));
	HGLOBAL hGblEffect = GlobalAlloc(GMEM_ZEROINIT | GMEM_MOVEABLE | GMEM_DDESHARE, sizeof(DWORD));
	DWORD *pDWDropEffect = (DWORD*)GlobalLock(hGblEffect);
	*pDWDropEffect = DROPEFFECT_COPY;
	GlobalUnlock(hGblEffect);

	DROPFILES dropFiles;
	dropFiles.pFiles = sizeof(DROPFILES);
	dropFiles.pt.x = 0;
	dropFiles.pt.y = 0;
	dropFiles.fNC = FALSE;
	dropFiles.fWide = TRUE;

	UINT uFileListLen = 0;
	for (std::vector<CString>::const_iterator it = vecFiles.begin(); it != vecFiles.end(); ++it)
	{
		uFileListLen += (it->GetLength() + 1);//尾部一个\0结束符  
	}

	UINT uGblLen = sizeof(DROPFILES) + sizeof(TCHAR)* (uFileListLen + 1);
	HGLOBAL hGblFiles = GlobalAlloc(GMEM_ZEROINIT | GMEM_MOVEABLE | GMEM_DDESHARE, uGblLen);
	BYTE *  pData = (BYTE*)GlobalLock(hGblFiles);
	memcpy(pData, (LPVOID)(&dropFiles), sizeof(DROPFILES));

	BYTE *pFileList = pData + sizeof(DROPFILES);

	for (std::vector<CString>::const_iterator it = vecFiles.begin(); it != vecFiles.end(); ++it)
	{
		UINT uLen = (it->GetLength() + 1) * sizeof(TCHAR);
		memcpy(pFileList, (BYTE*)(LPCTSTR)(*it), uLen);
		pFileList += uLen;
	}

	GlobalUnlock(hGblFiles);

	if (OpenClipboard(NULL))
	{
		EmptyClipboard();
		SetClipboardData(CF_HDROP, hGblFiles);
		SetClipboardData(uDropEffect, hGblEffect);
		CloseClipboard();
		_cprintf(" copy successful\n");
		return TRUE;
	}
	else
	{
		::GlobalFree(hGblEffect);
		::GlobalFree(hGblFiles);
	}
	return FALSE;

	OpenClipboard(0);
	EmptyClipboard();
	LPCTSTR path1 = (LPCTSTR)"D:\\test.bmp";
	LPCTSTR path2 = (LPCTSTR)"D:\\update.log";
	UINT           uBuffSize = 0;
	uBuffSize = lstrlen((LPCTSTR)path1) + lstrlen((LPCTSTR)path2) + 2; //+2 pour les deux \0, un pour chaque chaine
	uBuffSize = sizeof(DROPFILES) + sizeof(TCHAR) * (uBuffSize + 1); //=1 pour un dernier \0 a la toute fin
	HGLOBAL hData = ::GlobalAlloc(GHND | GMEM_SHARE, uBuffSize);
	DROPFILES* pDropFiles = (DROPFILES*) ::GlobalLock(hData);
	pDropFiles->pFiles = sizeof(DROPFILES);
	pDropFiles->fWide = FALSE; //la variable UNICODE est definie et pourtant lorsque je met TRUE, Word lis des caractere chinois :/
	TCHAR *pData = (TCHAR*)((LPBYTE)pDropFiles + sizeof(DROPFILES));
	lstrcpy(pData, (LPCTSTR)path1); //premiere chaine apres la structure DROPFILES (offset indique dans pFiles)
	pData = 1 + wcschr(pData, (wchar_t)'\0'); //placement apres l'\0
	lstrcpy(pData, (LPCTSTR)path2); //2e chaine
	pData = 1 + wcschr(pData, (wchar_t)'\0');
	lstrcpy(pData, (LPCTSTR) '\0'); //rajout du dernier \0
	::GlobalUnlock(hData);
	::SetClipboardData(CF_HDROP, hData);
	::CloseClipboard();
	*/
}
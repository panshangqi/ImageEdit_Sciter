// CopyFileToClipboard.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <windows.h>
#include <shlobj.h>
#include <atlbase.h>


#include <windows.h>
#include <sys/stat.h>
#include <fstream>
#include <errno.h>

int CopyFileToClipboard(char szFileName[]);
int main()
{
	CopyFileToClipboard("D:\\test.bmp");
	return 0;
}
int CopyFileToClipboard(char szFileName[])
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
	*lpdDropEffect = DROPEFFECT_COPY;//复制; 剪贴则用DROPEFFECT_MOVE
	
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
	system("pause");
	return 1;
}
#pragma once
class CopyImageToClipboard
{
public:
	CopyImageToClipboard();
	~CopyImageToClipboard();
	bool CopyFileToClipboard(const char *szFileName);
	bool CopyTextToClipboard(const char *text);
	void test(HWND mHwnd);
};


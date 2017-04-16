#pragma once
#include <vector>
class Pdf2Png
{
public:
	Pdf2Png();
	~Pdf2Png();
	std::vector<CString> runPdfToPng(char* pdfPath);
	char* WcharToChar(const wchar_t* wp);
	wchar_t* CharToWchar(const char* c);
};



#ifndef MY_MUPDF_CONVERT_PDF_PNG
#define MY_MUPDF_CONVERT_PDF_PNG


extern "C" 
{
#include "mupdf/fitz.h"
#include "mupdf/fitz-internal.h"
}

class CMuPDFConvert
{
public:
	CMuPDFConvert(void);
	~CMuPDFConvert(void);
	char savePath[512];
	bool Pdf2Png(const wchar_t* pdfFileName,const char* imageDir,const char* imageName, int &pageNum);

private:
	void drawpage(fz_context *ctx, fz_document *doc, int pagenum);
//�Զ���:
public:
	int getPageNumber(const wchar_t* wcharPdfFile, const char* imageDir, const char* imageName);
	bool getPngFromPage(int pageID);
	bool freeMemory();
private:
	static const fz_matrix fz_identity;
	static fz_colorspace k_device_rgb;
	fz_colorspace *fz_device_rgb;

	fz_document *m_doc;
	fz_stream *m_pStream;
	fz_context *m_ctx;

	int uselist;
	int alphabits;

	char output[1024];
	float resolution;
	float rotation;
	int res_specified;
	int width;
	int height;
	int fit;
	fz_colorspace *colorspace;
	int savealpha;
	int invert;
	float gamma_value;

	char filename[1024];
};

#endif
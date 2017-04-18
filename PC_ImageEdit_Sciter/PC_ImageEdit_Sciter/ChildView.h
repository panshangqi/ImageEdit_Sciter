
// ChildView.h : CChildView ��Ľӿ�
//


#pragma once
#include "sciter-x.h"
#include "sciter-x-threads.h"

// CChildView ����

class CChildView : public CWnd,
	public sciter::host<CChildView>,
	public sciter::event_handler
{
// ����
public:
	CChildView();

// ����
public:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	HWINDOW   get_hwnd();
	HINSTANCE get_resource_instance();
	virtual bool handle_mouse(HELEMENT he, MOUSE_PARAMS& params) override { return false; }
	virtual bool handle_key(HELEMENT he, KEY_PARAMS& params) override { return false; }
	virtual bool handle_focus(HELEMENT he, FOCUS_PARAMS& params)  override { return false; }
	virtual bool handle_event(HELEMENT he, BEHAVIOR_EVENT_PARAMS& params) override { return false; }
	virtual bool handle_method_call(HELEMENT he, METHOD_PARAMS& params)  override { return false; }


// ��д
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ʵ��
public:
	virtual ~CChildView();

	// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
	// ����
public:
	void onResizeView(int pWidth, int pHeight);
public:

	sciter::sync::gui_thread_ctx _;
	json::value loadWeb();
	sciter::value getPicPxList();
	sciter::value getImageCutList(sciter::value cut_questions_list, sciter::value cut_graphics_list);
	json::value copyBmpToClipboard(json::value pageid, json::value rect, json::value copyedInit, json::value copyedDone);

	BEGIN_FUNCTION_MAP
		FUNCTION_0("loadWeb", loadWeb)
		FUNCTION_0("getPicPxList", getPicPxList)
		FUNCTION_2("getImageCutList", getImageCutList)
		FUNCTION_4("copyBmpToClipboard", copyBmpToClipboard)
	END_FUNCTION_MAP

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnClose();
	
	afx_msg void OnOpenPdf();
	afx_msg void OnOpenImage();
	afx_msg void OnAppExit();
	afx_msg void OnCloseDoc();
	afx_msg void OnOcrRun();

};


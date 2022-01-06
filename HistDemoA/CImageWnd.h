
#pragma once
class CImageWnd:public CWnd
{
public:
	int HCurrentPosition;
	int HScrollMax;
	int HScrollPosition;
	int VCurrentPosition;
	int VScrollMax;
	int VScrollPosition;
	int nWidth;
	int nHeight;
	int nByteWidth;

public:
	BYTE* lpBits;

public:
	CImageWnd();
	~CImageWnd();

	void SetImage(int cx, int cy, const void* bits);

	void SetScroll(int cx, int cy);

protected:
	// {{AFX_MSG(CImageWnd)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	// }}AFX_MSG

	DECLARE_MESSAGE_MAP()

};


#pragma once
#include "afxcmn.h"

class AFX_EXT_CLASS JFCTabCtrl :	public CTabCtrl
{
public:
	JFCTabCtrl(void);
	virtual ~JFCTabCtrl(void);
	void SetColorBk(const COLORREF &Color);

	DECLARE_MESSAGE_MAP()

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	COLORREF m_ColorBk;
};

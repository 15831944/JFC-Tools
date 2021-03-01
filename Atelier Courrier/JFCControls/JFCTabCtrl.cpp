#include "StdAfx.h"
#include "jfctabctrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

JFCTabCtrl::JFCTabCtrl(void)
{
	m_ColorBk = GetSysColor(COLOR_BACKGROUND);  
}

JFCTabCtrl::~JFCTabCtrl(void)
{
}

void JFCTabCtrl::SetColorBk(const COLORREF &Color)
{
	m_ColorBk = Color;
	Invalidate(1); 
}

BEGIN_MESSAGE_MAP(JFCTabCtrl, CTabCtrl)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

BOOL JFCTabCtrl::OnEraseBkgnd(CDC* pDC)
{
	CRect rcClient;
	GetClientRect(&rcClient);
	CBrush Brush;
	Brush.CreateSolidBrush(m_ColorBk);
	pDC->FillRect(&rcClient, &Brush);
	Brush.DeleteObject();
	pDC->SetBkColor(m_ColorBk);
	return true;//CTabCtrl::OnEraseBkgnd(pDC);
}

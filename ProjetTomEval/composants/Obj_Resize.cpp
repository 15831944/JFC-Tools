// Obj_Resize.cpp : implementation file
//

#include "stdafx.h"
#include "topchrono.h"
#include "Obj_Resize.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CObj_Resize

CObj_Resize::CObj_Resize()
{
	m_fCapture=0;
	m_pWndLeft=0;
	m_NbPixelsLeft=0;
	m_pWndRight=0;
	m_NbPixelsRight=0;
}

CObj_Resize::~CObj_Resize()
{
}


BEGIN_MESSAGE_MAP(CObj_Resize, CStatic)
	//{{AFX_MSG_MAP(CObj_Resize)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CObj_Resize message handlers
extern CTopChronoApp theApp;

void CObj_Resize::OnLButtonDown(UINT nFlags, CPoint point) 
{
	SetCapture();	
	CStatic::OnLButtonDown(nFlags, point);
	HCURSOR cur=theApp.LoadStandardCursor(IDC_SIZEWE);
	SetCursor(cur);
	CRect R,ClipR;
	CWnd * pwnd=GetParent();
	pwnd->GetWindowRect(&ClipR);

	if(m_pWndLeft)
	{
		m_pWndLeft->GetWindowRect(&R);
		ClipR.left=R.left+m_NbPixelsLeft;
	}
	if(m_pWndRight)
	{
		m_pWndRight->GetWindowRect(&R);
		ClipR.right=R.right-m_NbPixelsRight;
	}


	GetWindowRect(&R);
	ClipR.top=R.top;
	ClipR.bottom=R.bottom;

	// R.top=R.bottom=pt.y;
	
	ClipCursor(&ClipR);

	GetClientRect(&m_Rect);
	TRACE("posx=%d\n",m_Rect.left);
	CDC * dc=GetDC();
	dc->DrawDragRect(&m_Rect,CSize(5,1),NULL,CSize(0,0));
	ReleaseDC(dc);
	m_fCapture=1;
	GetCursorPos(&m_StartPoint);
}

void CObj_Resize::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(!m_fCapture)return;
	CDC * dc=GetDC();
	dc->DrawDragRect(&m_Rect,CSize(5,1),NULL,CSize(0,0));
	CPoint pt;
	GetCursorPos(&pt);
	ScreenToClient(&pt);
	m_Rect.left=pt.x;
	m_Rect.right=pt.x+4;
	TRACE("posx=%d\n",m_Rect.left);
	dc->DrawDragRect(&m_Rect,CSize(5,1),NULL,CSize(0,0));
	ReleaseDC(dc);


		
	CStatic::OnMouseMove(nFlags, point);
}

void CObj_Resize::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_fCapture=0;
	CDC * dc=GetDC();
	dc->DrawDragRect(&m_Rect,CSize(4,1),NULL,CSize(0,0));
	ReleaseDC(dc);
	ReleaseCapture();
	ClipCursor(NULL);
	CStatic::OnLButtonUp(nFlags, point);
	
	CPoint pt;
	GetCursorPos(&pt);


	m_pMM->HResize(pt.x-m_StartPoint.x,0);
	GetParent()->Invalidate();
}

BOOL CObj_Resize::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	HCURSOR cur=::LoadCursor(NULL,IDC_SIZEWE);
	::SetCursor(cur);
	return(1);
//	return CStatic::OnSetCursor(pWnd, nHitTest, message);
}

void CObj_Resize::SetMoveManager(CMoveManager * pMM)
{
	m_pMM=pMM;
}




void CObj_Resize::SetLeftRange(CWnd *pWnd, int NbPixels)
{
	m_pWndLeft=pWnd;
	m_NbPixelsLeft=NbPixels;
}

void CObj_Resize::SetRightRange(CWnd *pWnd, int NbPixels)
{
	m_pWndRight=pWnd;
	m_NbPixelsRight=NbPixels;
}

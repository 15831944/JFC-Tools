// Obj_Gray.cpp : implementation file
//

#include "stdafx.h"
#include "Obj_Gray.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CObj_Gray

CObj_Gray::CObj_Gray()
{
	m_Fond=::GetSysColor(COLOR_3DFACE); //RGB (192,192,192)
}

CObj_Gray::~CObj_Gray()
{
}


BEGIN_MESSAGE_MAP(CObj_Gray, CStatic)
	//{{AFX_MSG_MAP(CObj_Gray)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_MOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CObj_Gray message handlers

void CObj_Gray::OnPaint() 
{
	CBrush m_brush;
	m_brush.CreateSolidBrush(m_Fond);

	CPaintDC dc(this); // device context for painting

	CRect rect;
	GetClientRect(&rect);
	dc.FillRect(&rect,&m_brush);
}


void CObj_Gray::OnSize(UINT nType, int cx, int cy) 
{
	CStatic::OnSize(nType, cx, cy);
	
	m_MovMan.Resize(cx, cy);	
}

void CObj_Gray::OnMove(int x, int y) 
{
	CStatic::OnMove(x, y);
	
	m_MovMan.Move(x,y);	
	
}

void CObj_Gray::AddWindow(CWnd*pCWnd, short style)
{
	m_MovMan.AddWindow(pCWnd,style);
}

void CObj_Gray::ValideChild(CRect RectOrigine)
{
	CRect r;

	GetWindowRect(r);
	m_MovMan.SetMasterRect(RectOrigine,r);
}

BOOL CALLBACK CObj_Gray_ChildWindowProc(HWND hwnd,LPARAM lParam)
{
	CRect r;
	GetWindowRect(hwnd,r);
//	CObj_Gray * ptr;
//	if(IntersectRect(ptr->m_MyRect,r){
//		AfficheMeaasge("WndTrouvé");
//	}
	return(TRUE);
}

void CObj_Gray::AddChildInRect(HWND hParent)
{
	GetWindowRect(m_MyRect);
	EnumChildWindows(hParent,CObj_Gray_ChildWindowProc,(LPARAM)this);
}

void CObj_Gray::SetFond(COLORREF Color)
{
	m_Fond=Color;

}

CMoveManager * CObj_Gray::GetPrivateMoveManager()
{
	return(&m_MovMan);
}

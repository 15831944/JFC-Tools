// Obj_ttle.cpp : implementation file
//

#include "stdafx.h"
#include "Obj_ttle.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CObj_ttle

CObj_ttle::CObj_ttle()
{
	m_font.CreateFont(-8, 0, 0, 0,FW_BOLD,0, 0, 0, 0, 0, 0, 0,(BYTE)(VARIABLE_PITCH | FF_SWISS),"Helv");
}

CObj_ttle::~CObj_ttle()
{
}


BEGIN_MESSAGE_MAP(CObj_ttle, CStatic)
	//{{AFX_MSG_MAP(CObj_ttle)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CObj_ttle message handlers

void CObj_ttle::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	char Title[100];
	GetWindowText(Title,100);
	
	CRect rect;

	GetClientRect(&rect);

	dc.SetBkMode(TRANSPARENT);
	dc.SelectObject(m_font);
	dc.SetTextColor(RGB(255,255,255));
	dc.DrawText(Title,-1,&rect,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
}


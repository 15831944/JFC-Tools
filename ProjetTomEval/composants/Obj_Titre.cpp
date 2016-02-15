// Obj_Titre.cpp : implementation file
//

#include "stdafx.h"
#include "Obj_Titre.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CObj_Titre

CObj_Titre::CObj_Titre()
{
	m_BkColor=RGB(128,128,128);
//	m_Font.CreatePointFont(-80, "MS Sans Serif");

	m_Font.CreateFont(-8, 0, 0, 0,FW_BOLD,0, 0, 0, 0, 0, 0, 0,(BYTE)(VARIABLE_PITCH | FF_SWISS),"MS Sans Serif");         /* and face name only */
	//m_pTimesNewRoman->CreateFont(-12, 0, 0, 0,FW_NORMAL,0, 0, 0, 0, 0, 0, 0,(BYTE)(VARIABLE_PITCH | FF_SWISS),"Times New Roman");         /* and face name only */


//	m_Font2.CreatePointFont(100, "Courrier");
//	m_Font2.CreatePointFont(90, "Arial");
//	m_Font.CreatePointFont(100, "Times New Roman");

}

CObj_Titre::~CObj_Titre()
{
}


BEGIN_MESSAGE_MAP(CObj_Titre, CStatic)
	//{{AFX_MSG_MAP(CObj_Titre)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CObj_Titre message handlers

void CObj_Titre::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CRect rect;
	GetClientRect(&rect);
	CBrush fond(m_BkColor);
	dc.FillRect(rect,&fond);
	dc.SetTextColor(RGB(255,255,255));
	dc.SetBkMode(TRANSPARENT);
	dc.SelectObject(m_Font);
	dc.DrawText(m_txt,&rect,DT_CENTER|DT_VCENTER|DT_NOPREFIX|DT_SINGLELINE);
	
	// Do not call CStatic::OnPaint() for painting messages
}

void CObj_Titre::SetText(LPCSTR txt)
{
	m_txt=txt;
	Invalidate();
}

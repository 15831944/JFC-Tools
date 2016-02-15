// Obj_Graz.cpp : implementation file
//

#include "stdafx.h"
#include "Obj_Graz.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CObj_Graz

CObj_Graz::CObj_Graz()
{
}

CObj_Graz::~CObj_Graz()
{
}


BEGIN_MESSAGE_MAP(CObj_Graz, CStatic)
	//{{AFX_MSG_MAP(CObj_Graz)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CObj_Graz message handlers

void CObj_Graz::OnPaint() 
{
	CBrush m_brush;
	m_brush.CreateSolidBrush(RGB (192,192,192));

	CPaintDC dc(this); // device context for painting

	CRect rect;
	GetClientRect(&rect);
	dc.FillRect(&rect,&m_brush);
}

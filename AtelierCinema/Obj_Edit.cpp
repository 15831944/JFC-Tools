// Obj_Edit.cpp : implementation file
//

#include "stdafx.h"
#include "cinema.h"
#include "Obj_Edit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CObj_Edit

CObj_Edit::CObj_Edit()
{
	m_BrushFond.CreateSolidBrush(RGB(192,255,192));
}

CObj_Edit::~CObj_Edit()
{
}


BEGIN_MESSAGE_MAP(CObj_Edit, CEdit)
	//{{AFX_MSG_MAP(CObj_Edit)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CObj_Edit message handlers

HBRUSH CObj_Edit::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
//	HBRUSH hbr = CEdit::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	
	// TODO: Return a different brush if the default is not desired
	return m_BrushFond;
}

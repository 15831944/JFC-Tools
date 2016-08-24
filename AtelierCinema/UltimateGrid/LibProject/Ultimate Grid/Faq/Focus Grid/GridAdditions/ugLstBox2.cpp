/***********************************************
	Ultimate Grid 97
	Copyright 1994 - 1997 Dundas Software Ltd.

	Class 
		CUGLstBox2
	Purpose
		This class is used by the CUGDropList class
		as the listbox that appears when the droplist
		is activated.
	Details
		This class is responsible for returning any
		selection make in the listbox, and destroying
		itself when an item is selected or when the
		selection process is canceled.
************************************************/

#include "stdafx.h"
#include "UGCtrl.h"
#include "ugLstBox2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/***************************************************
****************************************************/
CUGLstBox2::CUGLstBox2()
{
}

/***************************************************
****************************************************/
CUGLstBox2::~CUGLstBox2()
{
}

/***************************************************
****************************************************/
BEGIN_MESSAGE_MAP(CUGLstBox2, CListBox)
	//{{AFX_MSG_MAP(CUGLstBox2)
	ON_WM_KILLFOCUS()
	ON_WM_MOUSEACTIVATE()
	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()
	ON_WM_SETFOCUS()
	ON_WM_GETDLGCODE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/***************************************************
****************************************************/
void CUGLstBox2::OnSetFocus(CWnd* pOldWnd) 
{
	CUGLstBox::OnSetFocus( pOldWnd );
}

/***************************************************
****************************************************/
void CUGLstBox2::OnKillFocus(CWnd* pNewWnd) 
{
	ReleaseCapture();

	CListBox::OnKillFocus(pNewWnd);

	DestroyWindow();
	m_HasFocus = FALSE;

	if ( pNewWnd != m_ctrl->m_CUGGrid )
	{	// added to inform the control class that the focus is lost
		m_ctrl->OnKillFocus( UG_GRID );
	}
	else
	{
		if(m_selected == FALSE)
		{
			*m_col = -2;
			*m_row = -2;
		}
		m_selected = FALSE;
		m_ctrl->m_CUGGrid->SendMessage(WM_PAINT,0,0);
	}
}

/***************************************************
****************************************************/
int CUGLstBox2::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message) 
{
	return CUGLstBox::OnMouseActivate( pDesktopWnd, nHitTest, message );
}

/***************************************************
****************************************************/
void CUGLstBox2::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CUGLstBox::OnLButtonUp( nFlags, point );
}

/***************************************************
****************************************************/
void CUGLstBox2::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CUGLstBox::OnKeyDown(nChar, nRepCnt, nFlags);
}

UINT CUGLstBox2::OnGetDlgCode() 
{
	return CUGLstBox::OnGetDlgCode();
}

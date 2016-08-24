/***********************************************
	Ultimate Grid 97
	Copyright 1994 - 1997 Dundas Software Ltd.

	Class 
		CUGEdit2
	Purpose
		This is the standard edit control for the 
		grid. The grid can use any control derived
		from CWnd as a edit control.
	Details
		-This control automatically expands to the
		 right as text is entered, once the control
		 reaches the right side of the grid then it
		 expands downward until it reaches the bottom.
		 Once it reaches the bottom then it will start
		 scrolling text as it is entered.
		-When editing first starts the control also
		 automatically expands to fit the inital text.
		-When editing finishes it is the responsability
		 of the edit control to call the grid's 
		 CUGCtrl::EditCtrlFinished method to notify
		 the grid that editing has finished.
		-The edit control can also call the grid's
		 OnEditVerify during editing as well to give
		 the programmer greater control over what is
		 being entered.
		-New edit controls can be used by the grid
		 by calling CUGCtrl::SetNewEditClass. This
		 function can be called just before editing
		 starts, thus allowing different cells to use
		 different edit controls.
************************************************/

#include "stdafx.h"
#include <ctype.h>
#include "UGCtrl.h"
#include "UGEdit2.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/***************************************************
****************************************************/
CUGEdit2::CUGEdit2()
{
}

/***************************************************
****************************************************/
CUGEdit2::~CUGEdit2()
{
}

/***************************************************
****************************************************/
BEGIN_MESSAGE_MAP(CUGEdit2, CEdit)
	//{{AFX_MSG_MAP(CUGEdit2)
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	ON_WM_KEYDOWN()
	ON_WM_CHAR()
	ON_WM_MOUSEACTIVATE()
	ON_WM_SETCURSOR()
	ON_WM_GETDLGCODE()      
	ON_CONTROL_REFLECT(EN_UPDATE, OnUpdate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/***************************************************
****************************************************/
void CUGEdit2::OnKillFocus(CWnd* pNewWnd) 
{
	CUGEdit::OnKillFocus(pNewWnd);

	// added to inform the control class that the focus is lost
	if ( pNewWnd != (CWnd*)m_ctrl->m_CUGGrid )
	{
		m_ctrl->OnKillFocus( UG_GRID );
	}
}

/***************************************************
****************************************************/
void CUGEdit2::OnSetFocus(CWnd* pOldWnd) 
{
	CUGEdit::OnSetFocus(pOldWnd);
}

/***************************************************
****************************************************/
void CUGEdit2::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CUGEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

/***************************************************
****************************************************/
void CUGEdit2::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CUGEdit::OnChar(nChar, nRepCnt, nFlags);
}


/***************************************************
****************************************************/
int CUGEdit2::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message) 
{
	return CUGEdit::OnMouseActivate(pDesktopWnd, nHitTest, message);
}

/***************************************************
****************************************************/
BOOL CUGEdit2::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	return CUGEdit::OnSetCursor( pWnd, nHitTest, message );
}

/***************************************************
****************************************************/
void CUGEdit2::OnUpdate() 
{
	CUGEdit::OnUpdate();
}

/***************************************************
****************************************************/
UINT CUGEdit2::OnGetDlgCode() 
{
	return CUGEdit::OnGetDlgCode();
}

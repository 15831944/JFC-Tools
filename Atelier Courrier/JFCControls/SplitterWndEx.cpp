// SplitterWndEx.cpp : implementation file
//

#include "stdafx.h"
#include "SplitterWndEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//The CSplitterWndEx class primary extened use is for Replacing Views.
//In the function CSplitterWndEx::ReplaceView pDoc->AutoDelete has to be changed to prevent
//the one and only doc object from being deleted.
/////////////////////////////////////////////////////////////////////////////
// CSplitterWndEx

CSplitterWndEx::CSplitterWndEx()
{
	m_bBarLocked=FALSE;
}

CSplitterWndEx::~CSplitterWndEx()
{
}


BEGIN_MESSAGE_MAP(CSplitterWndEx, CSplitterWnd)
	//{{AFX_MSG_MAP(CSplitterWndEx)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSplitterWndEx message handlers

void CSplitterWndEx::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (!m_bBarLocked)
		CSplitterWnd::OnLButtonDown(nFlags, point);
}

void CSplitterWndEx::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (!m_bBarLocked)
		CSplitterWnd::OnMouseMove(nFlags, point);
	else
		CWnd::OnMouseMove(nFlags, point);
}

BOOL CSplitterWndEx::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	if (!m_bBarLocked)
		return CWnd::OnSetCursor(pWnd, nHitTest, message);

	return CSplitterWnd::OnSetCursor(pWnd, nHitTest, message);
}

BOOL CSplitterWndEx::ReplaceView(int row, int col,CRuntimeClass * pViewClass,SIZE size)
{
	CCreateContext context;
	BOOL bSetActive;


	if ((GetPane(row,col)->IsKindOf(pViewClass))==TRUE)
		return FALSE;

	CView * pActiveView = GetParentFrame()->GetActiveView();
	if (pActiveView == NULL || pActiveView==GetPane(row,col))
		bSetActive=TRUE;
	else
		bSetActive=FALSE;

	// Delete existing view 
		
	GetParentFrame()->SetActiveView(NULL);
	GetPane(row,col)->DestroyWindow();

	// Create new view
	context.m_pNewViewClass=pViewClass;
	//context.m_pCurrentDoc=pDoc;
	context.m_pNewDocTemplate=NULL;
	context.m_pLastView=NULL;
	context.m_pCurrentFrame=NULL;

	CreateView(row,col,pViewClass,size, &context);

	CView * pNewView= (CView *)GetPane(row,col);

	//if (bSetActive==TRUE)
	//	GetParentFrame()->SetActiveView(NULL);
	GetPane(row,col)->UpdateData(0);

	RecalcLayout(); 
	GetPane(row,col)->SendMessage(WM_PAINT);

	return TRUE;
}




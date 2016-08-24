// ChildFrm.cpp : implementation of the CChildFrame class
//

#include "stdafx.h"
#include "cinema.h"
#include "ChildFrm.h"
#include "CinemaDoc.h"
#include "CinemaView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CCinemaApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, SECMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, SECMDIChildWnd)
	//{{AFX_MSG_MAP(CChildFrame)
	ON_WM_GETMINMAXINFO()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrame construction/destruction

CChildFrame::CChildFrame()
{
}

CChildFrame::~CChildFrame()
{
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.x = 0;
	cs.y = 0;
	if( !SECMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	SECMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	SECMDIChildWnd::Dump(dc);
}

#endif 

/////////////////////////////////////////////////////////////////////////////
// CChildFrame message handlers

void CChildFrame::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	// MODIF FRED : 06/05/2002 (adaptation de la taille en fction de la largeur de la bordure)
	//CPoint pt(800,510);
	// CPoint pt(785,505);  dernier
	
	// Modif Sept 2013
	CPoint pt(830,580);


	int ytitre=GetSystemMetrics(SM_CYCAPTION);
	int bordx=GetSystemMetrics(SM_CXSIZEFRAME);
	int bordy=GetSystemMetrics(SM_CYSIZEFRAME);


	// Cette partie empêche le resize de la fenêtre grille !!!! A VOIR
	pt.x+=2*bordx;
	pt.y+=2*bordy;
	lpMMI->ptMaxSize=lpMMI->ptMaxPosition=lpMMI->ptMinTrackSize=lpMMI->ptMaxTrackSize=pt;
	
	SECMDIChildWnd::OnGetMinMaxInfo(lpMMI);

}

void CChildFrame::OnClose() 
{
	SECMDIChildWnd::OnClose();
}


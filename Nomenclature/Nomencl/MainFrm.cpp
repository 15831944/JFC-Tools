// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Nomenclature.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_GETMINMAXINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	/*
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	/}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
	*/

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// Supprime le nom du document
	cs.style&=~FWS_ADDTOTITLE; 

	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


void CMainFrame::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	
	// on définit la taille maximum de la fenêtre
	//lpMMI->ptMaxSize      = CPoint(400,555);
	//lpMMI->ptMaxPosition  = CPoint(400,555);
	//lpMMI->ptMinTrackSize = CPoint(398,555);  // 400,555
	lpMMI->ptMinTrackSize = CPoint(404,555);  // 400,555
	//lpMMI->ptMaxTrackSize = CPoint(400,555);
	
	CFrameWnd::OnGetMinMaxInfo(lpMMI);
}

void CMainFrame::OnSetPreviewMode(BOOL bPreview, CPrintPreviewState* pState) 
{
	if (bPreview == TRUE)
    /*
	{
		// Sauve taille fenetre avant appel preview
		GetWindowRect(m_wndRectOrg);
		// Au cas ou c'est une fenetre enfant
		//MapWindowPoints( GetParent( ), m_wndRectOrg );
	}
	else
	{
		// Au retour on récupère taille fenetre avant appel preview
		SetWindowPos(NULL,m_wndRectOrg.left,m_wndRectOrg.top,m_wndRectOrg.right-m_wndRectOrg.left,m_wndRectOrg.bottom-m_wndRectOrg.top,SWP_NOZORDER);
	}
	*/

	{
		// on récupère l'état de la fenêtre
		m_WindowState.length = sizeof(m_WindowState);
		GetWindowPlacement(&m_WindowState);
	}
	else
	{
		// on restaure l'état de la fenêtre
		m_WindowState.length = sizeof(m_WindowState);
		SetWindowPlacement(&m_WindowState);
	}


	CFrameWnd::OnSetPreviewMode(bPreview, pState);
}

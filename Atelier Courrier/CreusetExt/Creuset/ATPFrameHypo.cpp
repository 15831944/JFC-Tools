// on inclut les définitions nécessaires
#include "stdafx.h"
#include "ATPFrameHypo.h"
#include "ATPEtude.h"
#include "ATPLibelle.h"
#include "ATPPeriodeAction.h"
#include "ATPVersion.h"
#include "ATPPupitre.h"
#include ".\atpframehypo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// on définit la taille des splits
static const LONG g_CXSplit = 3;
static const LONG g_CYSplit = 3;

const CRect CATPFrameHypo::s_rectDefault(100, 100, 1010, 700);  // statique


//////////////////
// le constructeur

CATPFrameHypo::CATPFrameHypo(JATPDocCreuset *pDoc)
:CATPBaseFrame(pDoc)
{
    m_bFirstTimeHypo = TRUE;
}

//////////////////
// le destructeur

CATPFrameHypo::~CATPFrameHypo()
{
	// on ne fait rien
}

BEGIN_MESSAGE_MAP(CATPFrameHypo, CATPBaseFrame)
	//{{AFX_MSG_MAP(CATPFrameHypo)
	ON_WM_CREATE()
	ON_WM_GETMINMAXINFO()
	ON_WM_SIZE()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
	ON_COMMAND(IDM_CR_EXIT, OnClose)
END_MESSAGE_MAP()


//////////////////////////////////////////
// la fonction pour mettre à jour les vues

VOID CATPFrameHypo::UpdateAllViews(const JFCUpdate & Message)
{
	m_pSplitterHypo->UpdateAllViews(Message);

	//if(Message == UPDATE_AFF_CONTRIB)
	//{
	//	m_dlgContribution.Update();
	//	return;
	//}
	
	if(m_pATPDocCreuset)
	{
		if (IsIconic())
		{
			ShowWindow( SW_RESTORE );
			m_pATPDocCreuset->m_AFFCreuset.SetAffHypo(1);
			m_pATPDocCreuset->m_AFFCreuset.SetHypoActif(1);
		}
		else
			ShowWindow((m_pATPDocCreuset->m_AFFCreuset.GetAffHypo()) ?  SW_SHOW : SW_HIDE);
	}
	
		
	// on met à jour les composants
	//m_dlgContribution.Update();

	if(Message == UPDATE_COLOR)
	{
		m_pSplitterHypo->SetColors();
		m_dlgContribution.SetColors();
		m_dlgContribution.Invalidate();
		Invalidate();
		return;
	}

	// Construction du titre fenêtre
	CString   LabelEtude;
	CString   LabelFen;
	LabelEtude = m_pATPDocCreuset->m_KEYEtude.GetLabel();
	LabelFen = "Test de plans - " + LabelEtude;
	SetWindowText(LabelFen);

}

////////////////////////////////
// CATPFrameHypo message handlers

int CATPFrameHypo::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	// on appelle le gestionnaire de base
	if (this->CATPBaseFrame::OnCreate(lpCreateStruct) == -1) return (-1);

	// on crée les composants
	m_pSplitterHypo = new CATPSplitterHypo();
	if (m_pSplitterHypo->Create(NULL, NULL, WS_CHILD | WS_VISIBLE |	WS_CLIPCHILDREN, CRect(0, 0, CATPSplitterHypo::GetCXMin(), CATPSplitterHypo::GetCYMin()), this, 0) == FALSE) return (-1);


	m_pSplitterHypo->SetDocument(m_pATPDocCreuset);

	//if (m_dlgContribution.Create(CDialogContrib::IDD, this) == FALSE) return (-1);
	//m_dlgContribution.SetDocument(m_pATPDocCreuset);

	// on quitte	
	return (0);
}

void CATPFrameHypo::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	// on appelle le gestionnaire de base
	this->JFCFrame::OnGetMinMaxInfo(lpMMI);

	// on récupère les rectangles de la fenêtre
	RECT RectC; this->GetClientRect(&RectC);
	RECT RectW; this->GetWindowRect(&RectW); this->ScreenToClient(&RectW);

	// on corrige la taille minimum de la fenêtre
	lpMMI->ptMinTrackSize.x = CATPPupitre::GetCXMin() + RectC.left - RectW.left + RectW.right  - RectC.right;
	lpMMI->ptMinTrackSize.y = CATPSplitterHypo::GetCYMin() + RectC.top  - RectW.top  + RectW.bottom - RectC.bottom;
}

void CATPFrameHypo::OnSize(UINT nType, int cx, int cy) 
{
	// on appelle le gestionnaire de base
	this->JFCFrame::OnSize(nType, cx, cy);

	// on repositionne les composants
	if (m_pSplitterHypo->GetSafeHwnd() != NULL) m_pSplitterHypo->SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
}

void CATPFrameHypo::OnClose() 
{
	if(CanClose())
	{
		// Hide Zoom and Distribution boxes
		m_pATPDocCreuset->m_AFFFiltreAudience = false;
		m_pATPDocCreuset->m_AFFOptionsMoteur = false;
		m_pATPDocCreuset->m_AFFCourbeReponse = false;
		m_pATPDocCreuset->m_AFFZoomPlan.SetVisible(false);
		m_pATPDocCreuset->m_AFFContribution.SetVisible(false);
		m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_PANEL);


		m_pATPDocCreuset->m_AFFVentilation.SetVisible(false);

		// Hide this window
		m_pATPDocCreuset->m_AFFCreuset.SetAffHypo(false);
		m_pATPDocCreuset->m_AFFCreuset.SetHypoActif(false);

		m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_HYPO);
	}

}

void CATPFrameHypo::OnDestroy()
{
    CString strText;
    BOOL bIconic, bMaximized;

    WINDOWPLACEMENT wndpl;
    wndpl.length = sizeof(WINDOWPLACEMENT);
    // lit la position courante de la fenêtre et
    // l'état (zoomé/réduit)
    BOOL bRet = GetWindowPlacement(&wndpl);
    if (wndpl.showCmd == SW_SHOWNORMAL)
	{
        bIconic = FALSE;
        bMaximized = FALSE;
    }
    else if (wndpl.showCmd == SW_SHOWMAXIMIZED)
	{
        bIconic = FALSE;
        bMaximized = TRUE;
    } 
    else if (wndpl.showCmd == SW_SHOWMINIMIZED)
	{
        bIconic = TRUE;
        if (wndpl.flags)
            bMaximized = TRUE;
        else
            bMaximized = FALSE;
    }
 
	strText.Format("%04d %04d %04d %04d",
                   wndpl.rcNormalPosition.left,
                   wndpl.rcNormalPosition.top,
                   wndpl.rcNormalPosition.right,
                   wndpl.rcNormalPosition.bottom);
 
	AfxGetApp()->WriteProfileString("Options\\FrameHypo", "Rect", strText);
	
    //AfxGetApp()->WriteProfileInt("Options\\FrameHypo", "Icon", bIconic);
    AfxGetApp()->WriteProfileInt("Options\\FrameHypo", "Max", bMaximized);

	CATPBaseFrame::OnDestroy();
}

void CATPFrameHypo::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CString strText;
    BOOL bIconic, bMaximized;
    UINT flags;
    WINDOWPLACEMENT wndpl;
    CRect rect;

    if (m_bFirstTimeHypo)
	{
        m_bFirstTimeHypo = FALSE;

        strText = AfxGetApp()->GetProfileString("Options\\FrameHypo", "Rect");
		if (!strText.IsEmpty())
		{
			// Récupére les coordonnées de la fenêtre plan par hypothèses
            rect.left = atoi((const char*)		strText);
            rect.top = atoi((const char*)		strText + 5);
            rect.right = atoi((const char*)		strText + 10);
            rect.bottom = atoi((const char*)	strText + 15);
        }
        else 
            rect = s_rectDefault;

        bIconic = AfxGetApp()->GetProfileInt("Options\\FrameHypo", "Icon", 0);
        bMaximized = AfxGetApp()->GetProfileInt("Options\\FrameHypo", "Max", 0);   
        if (bIconic)
		{
            nStatus = SW_SHOWMINNOACTIVE;
            if (bMaximized)
                flags = WPF_RESTORETOMAXIMIZED;
            else
                flags = WPF_SETMINPOSITION;
        }
        else
		{
            if (bMaximized)
			{
                nStatus = SW_SHOWMAXIMIZED;
                flags = WPF_RESTORETOMAXIMIZED;
            }
            else
			{
                nStatus = SW_NORMAL;
                flags = WPF_SETMINPOSITION;
            }
        }
        wndpl.length = sizeof(WINDOWPLACEMENT);
        wndpl.showCmd = nStatus;
        wndpl.flags = flags;
        wndpl.ptMinPosition = CPoint(0, 0);
        wndpl.ptMaxPosition =  CPoint(-::GetSystemMetrics(SM_CXBORDER), -::GetSystemMetrics(SM_CYBORDER));
        wndpl.rcNormalPosition = rect;

		// définit la position de la fenêtre 
		// et l'état (zoomé/réduit)
        BOOL bRet = SetWindowPlacement(&wndpl);
    }

	//if(nStatus == SW_SHOWNORMAL)
	//{
	//	m_dlgFiltres.SetParent(this);
	//	m_dlgCourbeReponse.SetParent(this);
	//	m_dlgChoixMoteur.SetParent(this);
	//}

	CATPBaseFrame::OnShowWindow(bShow, nStatus);
}


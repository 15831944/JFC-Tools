// on inclut les définitions nécessaires
#include "stdafx.h"
#include "ATPFramePlan.h"
#include "ATPEtude.h"
#include "ATPLibelle.h"
#include "ATPPeriodeAction.h"
#include "ATPVersion.h"
#include ".\atpframeplan.h"
#include "ATPPupitre.h"
#include "ATPZoomPlan.h"
#include "PrintAnyCWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// on définit la taille des splits
static const LONG g_CXSplit = 3;
static const LONG g_CYSplit = 3;
const CRect CATPFramePlan::s_rectDefault(100, 100, 1010, 700);  // statique

//////////////////
// le constructeur

CATPFramePlan::CATPFramePlan(JATPDocCreuset *pDoc)
:CATPBaseFrame(pDoc)
{
    m_bFirstTimePlan = TRUE;
}

//////////////////
// le destructeur

CATPFramePlan::~CATPFramePlan()
{
	// on ne fait rien
}

BEGIN_MESSAGE_MAP(CATPFramePlan, CATPBaseFrame)
	//{{AFX_MSG_MAP(CATPFramePlan)
	ON_WM_CREATE()
	ON_WM_GETMINMAXINFO()
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
	ON_COMMAND(IDM_CR_EXIT, OnClose)
	
	ON_COMMAND(IDM_CR_GRPHCLIPBOARD, OnClipboardGraph)
	ON_UPDATE_COMMAND_UI(IDM_CR_GRPHCLIPBOARD, OnUpdateClipboardGraph)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()


//////////////////////////////////////////
// la fonction pour mettre à jour les vues

VOID CATPFramePlan::UpdateAllViews(const JFCUpdate & Message)
{
	CATPBaseFrame::UpdateAllViews(Message);

	if(Message == UPDATE_AFF_ZOOM_PLAN)
	{
		m_Z2.Update();
		return;
	}

	m_pSplitterPlan->UpdateAllViews(Message);

	if(Message == UPDATE_OFFRE || Message == UPDATE_INITIAL)
		m_Z2.SetNbFormats(m_pATPDocCreuset->m_Offre.m_TBLIdFormats.GetCount());



	if(m_pATPDocCreuset)
	{
		if (IsIconic())
		{
			ShowWindow( SW_RESTORE );
			m_pATPDocCreuset->m_AFFCreuset.SetAffPlan(1);
			m_pATPDocCreuset->m_AFFCreuset.SetPlanActif(1);
		}
		else
			ShowWindow((m_pATPDocCreuset->m_AFFCreuset.GetAffPlan()) ?  SW_SHOW : SW_HIDE);
	}


	// on met à jour les composants
	m_Z2.Update();


	if(Message == UPDATE_COLOR)
	{
		m_pSplitterPlan->SetColors();
		m_Z2.SetColors();
		Invalidate();
	}

	// on crée la table des études
	// Construction du titre fenêtre
	CString   LabelEtude;
	CString   LabelFen;
	LabelEtude = m_pATPDocCreuset->m_KEYEtude.GetLabel();
	LabelFen = "Grille Calendaire - " + LabelEtude;
	SetWindowText(LabelFen);
}

////////////////////////////////
// CATPFramePlan message handlers

int CATPFramePlan::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	// on appelle le gestionnaire de base
	if (this->CATPBaseFrame::OnCreate(lpCreateStruct) == -1) return (-1);

	// on crée les composants
	// ALAIN SPLIT
	m_pSplitterPlan = new CATPSplitterPlan();
	if (m_pSplitterPlan->Create(NULL, NULL, WS_CHILD | WS_VISIBLE |	WS_CLIPCHILDREN, CRect(0, 0, CATPSplitterPlan::GetCXMin(), CATPSplitterPlan::GetCYMin()), this, 0) == FALSE) return (-1);

	if (m_Z2.Create(CATPZoomPlan::IDD, this) == FALSE)
		return (-1);

	// on initialise les composants
	// ALAIN SPLIT
	m_pSplitterPlan->SetDocument(m_pATPDocCreuset);

	m_Z2.SetDocument(m_pATPDocCreuset);

	// on quitte	
	return (0);
}




void CATPFramePlan::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	// on appelle le gestionnaire de base
	this->JFCFrame::OnGetMinMaxInfo(lpMMI);

	// on récupère les rectangles de la fenêtre
	RECT RectC; this->GetClientRect(&RectC);
	RECT RectW; this->GetWindowRect(&RectW); this->ScreenToClient(&RectW);

	// on corrige la taille minimum de la fenêtre
	lpMMI->ptMinTrackSize.x = CATPPupitre::GetCXMin() + RectC.left - RectW.left + RectW.right  - RectC.right;
	lpMMI->ptMinTrackSize.y = CATPEchelle::GetCYMin() + RectC.top  - RectW.top  + RectW.bottom - RectC.bottom;
}

void CATPFramePlan::OnSize(UINT nType, int cx, int cy) 
{
	// on appelle le gestionnaire de base
	this->JFCFrame::OnSize(nType, cx, cy);

	// on repositionne les composants
	if (m_pSplitterPlan->GetSafeHwnd() != NULL)
		m_pSplitterPlan->SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
}

void CATPFramePlan::OnClose() 
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

		// Hide this window
		m_pATPDocCreuset->m_AFFCreuset.SetAffPlan(false);
		m_pATPDocCreuset->m_AFFCreuset.SetPlanActif(false);

		m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_PLAN);
	}

}

void CATPFramePlan::OnDestroy()
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
 
	AfxGetApp()->WriteProfileString("Options\\FramePlan", "Rect", strText);
    //AfxGetApp()->WriteProfileInt("Options\\FramePlan", "Icon", bIconic);
    AfxGetApp()->WriteProfileInt("Options\\FramePlan", "Max", bMaximized);

	CATPBaseFrame::OnDestroy();
}

void CATPFramePlan::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CString strText;
    BOOL bIconic, bMaximized;
    UINT flags;
    WINDOWPLACEMENT wndpl;
    CRect rect;

    if (m_bFirstTimePlan)
	{
        m_bFirstTimePlan = FALSE;
        strText = AfxGetApp()->GetProfileString("Options\\FramePlan", "Rect");
        if (!strText.IsEmpty())
		{
			// Récupére les coordonnées de la fenêtre plan calendaire
            rect.left = atoi((const char*)		strText);
            rect.top = atoi((const char*)		strText + 5);
            rect.right = atoi((const char*)		strText + 10);
            rect.bottom = atoi((const char*)	strText + 15);
        }
        else 
            rect = s_rectDefault;

        bIconic = AfxGetApp()->GetProfileInt("Options\\FramePlan", "Icon", 0);
        bMaximized = AfxGetApp()->GetProfileInt("Options\\FramePlan", "Max", 0);   
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

//void CATPFramePlan::OnPrintGRPJour()
//{
//	CString str;
//	if (m_pATPDocCreuset->m_MoteurPresse.GetType() == CMoteurPresse::PROBA)
//		str.FormatMessage(IDS_CR_GRPJOUR);
//	else
//		str.FormatMessage(IDS_CR_XGRPJOUR);
//
//	if (m_pSplitterPlan->m_wndCreusetResEch.GetPane(1,0)->IsKindOf(RUNTIME_CLASS(CATPResultPlan)))
//		CPrintAnyCWnd::Print( &((CATPResultPlan*)m_pSplitterPlan->m_wndCreusetResEch.GetPane(1,0))->m_grphGRPJour, str);
//
//}
//void CATPFramePlan::OnPrintGRPCumul()
//{
//	CString str;
//	if (m_pATPDocCreuset->m_MoteurPresse.GetType() == CMoteurPresse::PROBA)
//		str.FormatMessage(IDS_CR_GRPCUMUL);
//	else
//		str.FormatMessage(IDS_CR_XGRPCUMUL);
//
//	if (m_pSplitterPlan->m_wndCreusetResEch.GetPane(1,0)->IsKindOf(RUNTIME_CLASS(CATPResultPlan)))
//		CPrintAnyCWnd::Print( &((CATPResultPlan*)m_pSplitterPlan->m_wndCreusetResEch.GetPane(1,0))->m_grphGRPCumul, str);
//}
//void CATPFramePlan::OnUpdatePrintGraphGRP(CCmdUI *pCmdUI)
//{
//	pCmdUI->Enable(m_pATPDocCreuset->IsValid());
//	CString str;
//	if (m_pATPDocCreuset->m_MoteurPresse.GetType() == CMoteurPresse::PROBA)
//	{
//		if(pCmdUI->m_nIndex == 1)
//			str.FormatMessage(IDS_CR_GRPJOUR);
//		else
//			str.FormatMessage(IDS_CR_GRPCUMUL);
//	}
//	else
//	{
//		if(pCmdUI->m_nIndex == 1)
//			str.FormatMessage(IDS_CR_XGRPJOUR);
//		else
//			str.FormatMessage(IDS_CR_XGRPCUMUL);
//	}
//	pCmdUI->SetText(str);
//}
//void CATPFramePlan::OnPrintCouvJour()
//{
//	CString str;
//	str = "Apport Couverture/jour";
//
//	if (m_pSplitterPlan->m_wndCreusetResEch.GetPane(1,0)->IsKindOf(RUNTIME_CLASS(CATPResultPlan)))
//		CPrintAnyCWnd::Print( &((CATPResultPlan*)m_pSplitterPlan->m_wndCreusetResEch.GetPane(1,0))->m_grphCouvJour, str);
//}
//void CATPFramePlan::OnPrintCouvCumul()
//{
//	CString str;
//	str = "Couverture cumulée";
//
//	if (m_pSplitterPlan->m_wndCreusetResEch.GetPane(1,0)->IsKindOf(RUNTIME_CLASS(CATPResultPlan)))
//		CPrintAnyCWnd::Print( &((CATPResultPlan*)m_pSplitterPlan->m_wndCreusetResEch.GetPane(1,0))->m_grphCouvCumul, str);
//}
//void CATPFramePlan::OnUpdatePrintGraphCouv(CCmdUI *pCmdUI)
//{
//	pCmdUI->Enable(m_pATPDocCreuset->IsValid() && m_pATPDocCreuset->m_MoteurPresse.GetType() != CMoteurPresse::PROBA);
//}

void CATPFramePlan::OnClipboardGraph()
{
	if (m_pSplitterPlan->m_wndCreusetResEch.GetPane(1,0)->IsKindOf(RUNTIME_CLASS(CATPResultPlan)))
	{
		if(m_pATPDocCreuset->m_AFFResultPlan.GetGraphCouvCumul())
			CPrintAnyCWnd::CopyToClipBoard( &((CATPResultPlan*)m_pSplitterPlan->m_wndCreusetResEch.GetPane(1,0))->m_grphCouvCumul);
		if(m_pATPDocCreuset->m_AFFResultPlan.GetGraphCouvJour())
			CPrintAnyCWnd::CopyToClipBoard( &((CATPResultPlan*)m_pSplitterPlan->m_wndCreusetResEch.GetPane(1,0))->m_grphCouvJour);
		if(m_pATPDocCreuset->m_AFFResultPlan.GetGraphGRPCumul())
			CPrintAnyCWnd::CopyToClipBoard( &((CATPResultPlan*)m_pSplitterPlan->m_wndCreusetResEch.GetPane(1,0))->m_grphGRPCumul);
		if(m_pATPDocCreuset->m_AFFResultPlan.GetGraphGRPJour())
			CPrintAnyCWnd::CopyToClipBoard( &((CATPResultPlan*)m_pSplitterPlan->m_wndCreusetResEch.GetPane(1,0))->m_grphGRPJour);
	}
}
void CATPFramePlan::OnUpdateClipboardGraph(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_pATPDocCreuset->IsValid() && m_pSplitterPlan->m_wndCreusetResEch.GetPane(1,0)->IsKindOf(RUNTIME_CLASS(CATPResultPlan)));
}




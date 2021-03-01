// on inclut les définitions nécessaires
#include "stdafx.h"
#include "ATPSplitterPlan.h"
#include ".\atpsplitterplan.h"
#include "ATPPupitre.h"
#include "ATPPanelBas.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////
// le constructeur
//IMPLEMENT_DYNCREATE(CATPSplitterPlan, JFCFrame)
CATPSplitterPlan::CATPSplitterPlan()
{
	// on initialise le document
	m_pATPDocCreuset = 0;
}

///////////////////////////////////////
// la fonction pour définir le document

JVoid CATPSplitterPlan::SetDocument(JATPDocCreuset* pATPDocCreuset)
{
	// on mémorise le document
	m_pATPDocCreuset = pATPDocCreuset;

	// on définit le document des composants
	((JFCSplitterPane*)m_wndPanelSplitter.GetPane(0,0))->SetDocument(m_pATPDocCreuset);
	((CATPPanelBas*)m_wndPanelSplitter.GetPane(1,0))->SetDocument(m_pATPDocCreuset);

	((JFCSplitterPane*)m_wndCreusetResEch.GetPane(0,0))->SetDocument(m_pATPDocCreuset);

	if (m_wndCreusetResEch.GetPane(1,0)->IsKindOf(RUNTIME_CLASS(CATPResultPlan)))
		((CATPResultPlan*)m_wndCreusetResEch.GetPane(1,0))->SetDocument(m_pATPDocCreuset);
	else
		((JFCSplitterPane*)m_wndCreusetResEch.GetPane(1,0))->SetDocument(m_pATPDocCreuset);

	// ALAIN SPLIT
	((CResultBar*)m_wndBarSplitter.GetPane(1,0))->SetDocument(m_pATPDocCreuset);

	// on met à jour le composant
	this->OnUpdate();
}

void CATPSplitterPlan::SetColors()
{
	((JFCSplitterPane*)m_wndPanelSplitter.GetPane(0,0))->SetColors();
	((CATPPanelBas*)m_wndPanelSplitter.GetPane(1,0))->SetColors();

	((JFCSplitterPane*)m_wndCreusetResEch.GetPane(0,0))->SetColors();

	if (m_wndCreusetResEch.GetPane(1,0)->IsKindOf(RUNTIME_CLASS(CATPResultPlan)))
		((CATPResultPlan*)m_wndCreusetResEch.GetPane(1,0))->SetColors();
	else
		((JFCSplitterPane*)m_wndCreusetResEch.GetPane(1,0))->SetColors();

	((CResultBar*)m_wndBarSplitter.GetPane(1,0))->SetColors();
}

//////////////////////////////////////////////
// la fonction pour mettre à jour le composant
VOID CATPSplitterPlan::UpdateAllViews(const JFCUpdate & Message)
{

	//if(Message == UPDATE_INIPROGRESS)
	//{
	//	((CATPCreusetPlan*)m_wndCreusetResEch.GetPane(0,0))->m_progress.SetRange(0, m_pATPDocCreuset->m_Progress.m_Range);
	//	((CATPCreusetPlan*)m_wndCreusetResEch.GetPane(0,0))->m_progress.SetStep(m_pATPDocCreuset->m_Progress.m_Step);
	//	((CATPCreusetPlan*)m_wndCreusetResEch.GetPane(0,0))->m_progress.ShowWindow(SW_SHOW);
	//}
	//if(Message == UPDATE_PROGRESS)
	//	((CATPCreusetPlan*)m_wndCreusetResEch.GetPane(0,0))->m_progress.StepIt();
	//if(Message == UPDATE_ENDPROGRESS)
	//	((CATPCreusetPlan*)m_wndCreusetResEch.GetPane(0,0))->m_progress.ShowWindow(SW_HIDE );

	if(Message == UPDATE_GRILLE_ZOOM)//TODO à intercepter dans les objets
	{
		((CATPCreusetPlan*)m_wndCreusetResEch.GetPane(0,0))->m_C1.SetHorzItemSize(m_pATPDocCreuset->m_ZoomGrille, TRUE); // Creuset Plan
		((CATPCreusetPlan*)m_wndCreusetResEch.GetPane(0,0))->m_C2.SetHorzItemSize(m_pATPDocCreuset->m_ZoomGrille, TRUE); // Creuset Plan
		((CATPCreusetPlan*)m_wndCreusetResEch.GetPane(0,0))->m_C3.SetHorzItemSize(m_pATPDocCreuset->m_ZoomGrille, TRUE); // Creuset Plan

		if (m_wndCreusetResEch.GetPane(1,0)->IsKindOf(RUNTIME_CLASS(CATPResultPlan)))
		{
			((CATPResultPlan*)m_wndCreusetResEch.GetPane(1,0))->m_grphCouvCumul.m_B1.SetHorzItemSize(m_pATPDocCreuset->m_ZoomGrille, TRUE); // Couverture
			((CATPResultPlan*)m_wndCreusetResEch.GetPane(1,0))->m_grphCouvCumul.m_A2.SetHorzItemSize(m_pATPDocCreuset->m_ZoomGrille, TRUE); // Couverture
			((CATPResultPlan*)m_wndCreusetResEch.GetPane(1,0))->m_grphGRPJour.m_B1.SetHorzItemSize(m_pATPDocCreuset->m_ZoomGrille, TRUE); // Couverture
			((CATPResultPlan*)m_wndCreusetResEch.GetPane(1,0))->m_grphGRPJour.m_A2.SetHorzItemSize(m_pATPDocCreuset->m_ZoomGrille, TRUE); // Couverture
			((CATPResultPlan*)m_wndCreusetResEch.GetPane(1,0))->m_grphCouvJour.m_B1.SetHorzItemSize(m_pATPDocCreuset->m_ZoomGrille, TRUE); // Couverture
			((CATPResultPlan*)m_wndCreusetResEch.GetPane(1,0))->m_grphCouvJour.m_A2.SetHorzItemSize(m_pATPDocCreuset->m_ZoomGrille, TRUE); // Couverture
			((CATPResultPlan*)m_wndCreusetResEch.GetPane(1,0))->m_grphGRPCumul.m_B1.SetHorzItemSize(m_pATPDocCreuset->m_ZoomGrille, TRUE); // Couverture
			((CATPResultPlan*)m_wndCreusetResEch.GetPane(1,0))->m_grphGRPCumul.m_A2.SetHorzItemSize(m_pATPDocCreuset->m_ZoomGrille, TRUE); // Couverture
		}

	}

	// on met à jour les composants
	((JFCSplitterPane*)m_wndCreusetResEch.GetPane(0,0))->Update();

	if (m_wndCreusetResEch.GetPane(1,0)->IsKindOf(RUNTIME_CLASS(CATPResultPlan)))
		((CATPResultPlan*)m_wndCreusetResEch.GetPane(1,0))->Update();
	else
		((JFCSplitterPane*)m_wndCreusetResEch.GetPane(1,0))->Update();

	((CResultBar*)m_wndBarSplitter.GetPane(1,0))->Update();

	((JFCSplitterPane*)m_wndPanelSplitter.GetPane(0,0))->Update();
	((CATPPanelBas*)m_wndPanelSplitter.GetPane(1,0))->Update();

	if(Message == UPDATE_INITIAL)
	{
		if (m_wndCreusetResEch.GetPane(1,0)->IsKindOf(RUNTIME_CLASS(CATPResultPlan)))
		{
			((CATPResultPlan*)m_wndCreusetResEch.GetPane(1,0))->m_grphCouvCumul.SetAuto();
			((CATPResultPlan*)m_wndCreusetResEch.GetPane(1,0))->m_grphGRPJour.SetAuto();
			((CATPResultPlan*)m_wndCreusetResEch.GetPane(1,0))->m_grphCouvJour.SetAuto();
			((CATPResultPlan*)m_wndCreusetResEch.GetPane(1,0))->m_grphGRPCumul.SetAuto();
		}
	}

	// on met à jour le composant
	this->OnUpdate();
}


/////////////////
// le destructeur

CATPSplitterPlan::~CATPSplitterPlan()
{
}

///////////////////////////////////////////////////////////////////
// les fonctions pour récupérer les dimensions minimales du panneau

LONG CATPSplitterPlan::GetCXMin()
{
	// on calcule la largeur des composants
	LONG CX1 = CATPCreusetPlan::GetCXMin();

	// on calcule la largeur des bordures et des interstices
	LONG CX2 = 0;

	// on renvoie la largeur
	return (CX1 + CX2);
}

LONG CATPSplitterPlan::GetCYMin()
{
	// on calcule la hauteur des composants
	LONG CY1 = CATPCreusetPlan::GetCYMin();

	// on calcule la largeur des bordures et des interstices
	LONG CY2 = 2*CATPEchelle::GetCYMin();

	// on renvoie la hauteur
	return (CY1 + CY2);
}

BEGIN_MESSAGE_MAP(CATPSplitterPlan, JFCFrame)
	//{{AFX_MSG_MAP(CATPSplitterPlan)
	//}}AFX_MSG_MAP
	ON_WM_SIZE()
END_MESSAGE_MAP()

//////////////////////////////////////////////
// la fonction pour appliquer les mises à jour

JVoid CATPSplitterPlan::OnUpdate()
{
	if (m_pATPDocCreuset)
	{
		if(m_pATPDocCreuset->m_AFFCreuset.GetEchelleResult())
		{
			if (m_wndCreusetResEch.GetPane(1,0)->IsKindOf(RUNTIME_CLASS(CATPResultPlan)))
			{
				//on remplace les resultats par l'echelle
				m_wndBarSplitter.HideBar(1,false);
				((CATPCreusetPlan*)m_wndCreusetResEch.GetPane(0,0))->RemoveScroll(&((CATPResultPlan*)m_wndCreusetResEch.GetPane(1,0))->m_grphGRPJour.SetHorzScrollBar(((CATPCreusetPlan*)m_wndCreusetResEch.GetPane(0,0))->GetHorzScrollBar()));
				((CATPCreusetPlan*)m_wndCreusetResEch.GetPane(0,0))->RemoveScroll(&((CATPResultPlan*)m_wndCreusetResEch.GetPane(1,0))->m_grphCouvCumul.SetHorzScrollBar(((CATPCreusetPlan*)m_wndCreusetResEch.GetPane(0,0))->GetHorzScrollBar()));
				((CATPCreusetPlan*)m_wndCreusetResEch.GetPane(0,0))->RemoveScroll(&((CATPResultPlan*)m_wndCreusetResEch.GetPane(1,0))->m_grphGRPCumul.SetHorzScrollBar(((CATPCreusetPlan*)m_wndCreusetResEch.GetPane(0,0))->GetHorzScrollBar()));
				((CATPCreusetPlan*)m_wndCreusetResEch.GetPane(0,0))->RemoveScroll(&((CATPResultPlan*)m_wndCreusetResEch.GetPane(1,0))->m_grphCouvJour.SetHorzScrollBar(((CATPCreusetPlan*)m_wndCreusetResEch.GetPane(0,0))->GetHorzScrollBar()));

				((CATPCreusetPlan*)m_wndCreusetResEch.GetPane(0,0))->RemoveScroll(&((CATPResultPlan*)m_wndCreusetResEch.GetPane(1,0))->m_grphGRPJour.GetAbcsisseControl());
				((CATPCreusetPlan*)m_wndCreusetResEch.GetPane(0,0))->RemoveScroll(&((CATPResultPlan*)m_wndCreusetResEch.GetPane(1,0))->m_grphCouvCumul.GetAbcsisseControl());
				((CATPCreusetPlan*)m_wndCreusetResEch.GetPane(0,0))->RemoveScroll(&((CATPResultPlan*)m_wndCreusetResEch.GetPane(1,0))->m_grphGRPCumul.GetAbcsisseControl());
				((CATPCreusetPlan*)m_wndCreusetResEch.GetPane(0,0))->RemoveScroll(&((CATPResultPlan*)m_wndCreusetResEch.GetPane(1,0))->m_grphCouvJour.GetAbcsisseControl());

				m_wndCreusetResEch.ReplaceView(1, 0,	RUNTIME_CLASS(CATPEchelle), CSize(CATPEchelle::GetCXMin(), CATPEchelle::GetCYMin()));
				((JFCSplitterPane*)m_wndCreusetResEch.GetPane(1,0))->SetDocument(m_pATPDocCreuset);
			}
		}
		else
		{
			if (m_wndCreusetResEch.GetPane(1,0)->IsKindOf(RUNTIME_CLASS(CATPEchelle)))
			{
				//on remplace l'echelle par les resultats 
				m_wndBarSplitter.HideBar(1,true);
	
				m_wndCreusetResEch.ReplaceView(1, 0,	RUNTIME_CLASS(CATPResultPlan), CSize(CATPResultPlan::GetCXMin(), CATPResultPlan::GetCYMin()));
				((CATPResultPlan*)m_wndCreusetResEch.GetPane(1,0))->Init();
				((CATPResultPlan*)m_wndCreusetResEch.GetPane(1,0))->SetDocument(m_pATPDocCreuset);

				((CATPResultPlan*)m_wndCreusetResEch.GetPane(1,0))->m_grphGRPJour.m_B1.SetHorzItemSize(m_pATPDocCreuset->m_ZoomGrille, TRUE); // GRP
				((CATPResultPlan*)m_wndCreusetResEch.GetPane(1,0))->m_grphGRPJour.m_A2.SetHorzItemSize(m_pATPDocCreuset->m_ZoomGrille, TRUE); // GRP
				((CATPResultPlan*)m_wndCreusetResEch.GetPane(1,0))->m_grphCouvCumul.m_B1.SetHorzItemSize(m_pATPDocCreuset->m_ZoomGrille, TRUE); // Couverture
				((CATPResultPlan*)m_wndCreusetResEch.GetPane(1,0))->m_grphCouvCumul.m_A2.SetHorzItemSize(m_pATPDocCreuset->m_ZoomGrille, TRUE); // GRP
				((CATPResultPlan*)m_wndCreusetResEch.GetPane(1,0))->m_grphGRPCumul.m_B1.SetHorzItemSize(m_pATPDocCreuset->m_ZoomGrille, TRUE); // GRP
				((CATPResultPlan*)m_wndCreusetResEch.GetPane(1,0))->m_grphGRPCumul.m_A2.SetHorzItemSize(m_pATPDocCreuset->m_ZoomGrille, TRUE); // GRP
				((CATPResultPlan*)m_wndCreusetResEch.GetPane(1,0))->m_grphCouvJour.m_B1.SetHorzItemSize(m_pATPDocCreuset->m_ZoomGrille, TRUE); // Couverture
				((CATPResultPlan*)m_wndCreusetResEch.GetPane(1,0))->m_grphCouvJour.m_A2.SetHorzItemSize(m_pATPDocCreuset->m_ZoomGrille, TRUE); // GRP

				// on configure l'ascenseur

				((CATPCreusetPlan*)m_wndCreusetResEch.GetPane(0,0))->SetGraphe(((CATPResultPlan*)m_wndCreusetResEch.GetPane(1,0))->m_grphGRPJour.m_B1);
				((CATPCreusetPlan*)m_wndCreusetResEch.GetPane(0,0))->SetGraphe(((CATPResultPlan*)m_wndCreusetResEch.GetPane(1,0))->m_grphGRPJour.m_A2);
				((CATPResultPlan*)m_wndCreusetResEch.GetPane(1,0))->m_grphGRPJour.m_B1.SetHorzScroll(&((CATPCreusetPlan*)m_wndCreusetResEch.GetPane(0,0))->GetHorzScrollBar(),FALSE,FALSE);
				((CATPResultPlan*)m_wndCreusetResEch.GetPane(1,0))->m_grphGRPJour.m_A2.SetHorzScroll(&((CATPCreusetPlan*)m_wndCreusetResEch.GetPane(0,0))->GetHorzScrollBar(),FALSE,FALSE);
				
				((CATPCreusetPlan*)m_wndCreusetResEch.GetPane(0,0))->SetGraphe(((CATPResultPlan*)m_wndCreusetResEch.GetPane(1,0))->m_grphCouvCumul.m_B1);
				((CATPCreusetPlan*)m_wndCreusetResEch.GetPane(0,0))->SetGraphe(((CATPResultPlan*)m_wndCreusetResEch.GetPane(1,0))->m_grphCouvCumul.m_A2);
				((CATPResultPlan*)m_wndCreusetResEch.GetPane(1,0))->m_grphCouvCumul.m_B1.SetHorzScroll(&((CATPCreusetPlan*)m_wndCreusetResEch.GetPane(0,0))->GetHorzScrollBar(),FALSE,FALSE);
				((CATPResultPlan*)m_wndCreusetResEch.GetPane(1,0))->m_grphCouvCumul.m_A2.SetHorzScroll(&((CATPCreusetPlan*)m_wndCreusetResEch.GetPane(0,0))->GetHorzScrollBar(),FALSE,FALSE);

				((CATPCreusetPlan*)m_wndCreusetResEch.GetPane(0,0))->SetGraphe(((CATPResultPlan*)m_wndCreusetResEch.GetPane(1,0))->m_grphGRPCumul.m_B1);
				((CATPCreusetPlan*)m_wndCreusetResEch.GetPane(0,0))->SetGraphe(((CATPResultPlan*)m_wndCreusetResEch.GetPane(1,0))->m_grphGRPCumul.m_A2);
				((CATPResultPlan*)m_wndCreusetResEch.GetPane(1,0))->m_grphGRPCumul.m_B1.SetHorzScroll(&((CATPCreusetPlan*)m_wndCreusetResEch.GetPane(0,0))->GetHorzScrollBar(),FALSE,FALSE);
				((CATPResultPlan*)m_wndCreusetResEch.GetPane(1,0))->m_grphGRPCumul.m_A2.SetHorzScroll(&((CATPCreusetPlan*)m_wndCreusetResEch.GetPane(0,0))->GetHorzScrollBar(),FALSE,FALSE);

				((CATPCreusetPlan*)m_wndCreusetResEch.GetPane(0,0))->SetGraphe(((CATPResultPlan*)m_wndCreusetResEch.GetPane(1,0))->m_grphCouvJour.m_B1);
				((CATPCreusetPlan*)m_wndCreusetResEch.GetPane(0,0))->SetGraphe(((CATPResultPlan*)m_wndCreusetResEch.GetPane(1,0))->m_grphCouvJour.m_A2);
				((CATPResultPlan*)m_wndCreusetResEch.GetPane(1,0))->m_grphCouvJour.m_B1.SetHorzScroll(&((CATPCreusetPlan*)m_wndCreusetResEch.GetPane(0,0))->GetHorzScrollBar(),FALSE,FALSE);
				((CATPResultPlan*)m_wndCreusetResEch.GetPane(1,0))->m_grphCouvJour.m_A2.SetHorzScroll(&((CATPCreusetPlan*)m_wndCreusetResEch.GetPane(0,0))->GetHorzScrollBar(),FALSE,FALSE);

				((CATPResultPlan*)m_wndCreusetResEch.GetPane(1,0))->Update();
			}
		}
	}
//*/
}

//////////////////////////////////
// CATPSplitterPlan message handlers

void CATPSplitterPlan::Invalidate(BOOL bErase)
{
	m_wndPanelSplitter.GetPane(0,0)->Invalidate(bErase);
	m_wndPanelSplitter.GetPane(1,0)->Invalidate(bErase);

	m_wndCreusetResEch.GetPane(0,0)->Invalidate(bErase);
	m_wndCreusetResEch.GetPane(1,0)->Invalidate(bErase);
	m_wndBarSplitter.GetPane(1,0)->Invalidate(bErase);
}

BOOL CATPSplitterPlan::OnCreateClient(LPCREATESTRUCT,	 CCreateContext* pContext)
{
	//###//
	//Le splitter vertical, contient le Splitter Bar et le Splitter Panel
	if (!m_wndVertSplitter.CreateStatic(this, 1, 2))
	{
		TRACE0("Failed to Create VerticalSplitter\n");
		return FALSE;
	}

	//le Splitter Bar
	if (!m_wndBarSplitter.CreateStatic(
		&m_wndVertSplitter,     
		2, 1,               
		WS_CHILD | WS_VISIBLE | WS_BORDER,  
		m_wndVertSplitter.IdFromRowCol(0, 0)
	   ))
	{
		TRACE0("Failed to Create Bar Splitter\n");
		return FALSE;
	}

	//le Splitter Panel
	if (!m_wndPanelSplitter.CreateStatic(
		&m_wndVertSplitter,     
		2, 1,               
		WS_CHILD | WS_VISIBLE | WS_BORDER,  
		m_wndVertSplitter.IdFromRowCol(0, 1)
	   ))
	{
		TRACE0("Failed to Create Panel Splitter\n");
		return FALSE;
	}

	//###//
	////On cree les panneaux du Splitter Panel////
	if (!m_wndPanelSplitter.CreateView(0, 0,
		RUNTIME_CLASS(CATPPupitre), CSize(CATPPupitre::GetCXMin(), CATPPupitre::GetCYMin()), pContext))
	{
		TRACE0("Failed to create first pane\n");
		return FALSE;
	}
	
	if (!m_wndPanelSplitter.CreateView(1, 0,
		RUNTIME_CLASS(CATPPanelBas), CSize(CATPPanelBas::GetCXMin(), CATPPanelBas::GetCYMin()), pContext))
	{
		TRACE0("Failed to create second pane\n");
		return FALSE;
	}


	//###//
	////On cree les panneaux du Splitter Bar////

	// La barre de boutons resultats
	if (!m_wndBarSplitter.CreateView(1, 0,
		RUNTIME_CLASS(CResultBar), CSize(CResultBar::GetCXMin(), CResultBar::GetCYMin()), pContext))
	{
		TRACE0("Failed to create CResultBar pane\n");
		return FALSE;
	}

	// le splitter Creuset + echelle/resultat
	if (!m_wndCreusetResEch.CreateStatic(
		&m_wndBarSplitter,    
		2, 1,               
		WS_CHILD | WS_VISIBLE | WS_BORDER,
		m_wndBarSplitter.IdFromRowCol(0, 0)
	   ))
	{
		TRACE0("Failed to Create   Creuset + echelle/resultat  Splitter\n");
		return FALSE;
	}

	//###//
	////On cree les panneaux du Splitter CreusetEchRes////
	
	//Le creuset
	if (!m_wndCreusetResEch.CreateView(0, 0,
		RUNTIME_CLASS(CATPCreusetPlan), CSize(CATPCreusetPlan::GetCXMin(), CATPCreusetPlan::GetCYMin()), pContext))
	{
		TRACE0("Failed to create creuset pane\n");
		return FALSE;
	}

	// le resultat
	if (!m_wndCreusetResEch.CreateView(1, 0,
		RUNTIME_CLASS(CATPResultPlan), CSize(CATPResultPlan::GetCXMin(), CATPResultPlan::GetCYMin()), pContext))
	{
		TRACE0("Failed to create resultat pane\n");
		return FALSE;
	}


	
	m_wndCreusetResEch.SetRowInfo(0,CATPCreusetPlan::GetCYIdeal(), CATPCreusetPlan::GetCYMin());

//	m_wndCreusetResEch.DblClickBar();
	m_wndVertSplitter.ClickBar();
	m_wndPanelSplitter.LockBar();

	m_wndBarSplitter.LockBar();
//	m_wndBarSplitter.ClickBar();


	return TRUE;
}

void CATPSplitterPlan::OnSize(UINT nType, int cx, int cy)
{

	if (cx >= CATPPupitre::GetCXMin()+10)
	{
		int cx0, cx1;
		int xmin0, xmin1;
		m_wndVertSplitter.GetColumnInfo(0, cx0, xmin0);
		m_wndVertSplitter.GetColumnInfo(1, cx1, xmin1);
		if(cx1!=0)
		{
			m_wndVertSplitter.SetColumnInfo(0, cx-CATPPupitre::GetCXMin()-10, cx-CATPPupitre::GetCXMin()-10);
			m_wndVertSplitter.SetColumnInfo(1, CATPPupitre::GetCXMin(), 0);

			// test aurelien
			 //m_wndVertSplitter.SetColumnInfo(0, cx-CATPPupitre::GetCXMin()-10, 200);
			 //m_wndVertSplitter.SetColumnInfo(1, CATPPupitre::GetCXMin(), 0);
		}
		else
		{
			m_wndVertSplitter.SetColumnInfo(0, cx, cx-CATPPupitre::GetCXMin()-10);
			m_wndVertSplitter.SetColumnInfo(1, 0, 0);
		}
	}

	if (cy >= CATPPanelBas::GetCYMin()+10)
		m_wndPanelSplitter.SetRowInfo(0, cy-CATPPanelBas::GetCYMin()-10, 10);

	if (cy >= CResultBar::GetCYMin())
//		m_wndBarSplitter.SetRowInfo(0, cy-CResultBar::GetCYMin(), cy-CResultBar::GetCYMin());
	{
		int cx0, cx1;
		int xmin0, xmin1;
		m_wndBarSplitter.GetRowInfo(0, cx0, xmin0);
		m_wndBarSplitter.GetRowInfo(1, cx1, xmin1);
		if(cx1!=0)
		{
			m_wndBarSplitter.SetRowInfo(0, cy-CResultBar::GetCYMin(), cy-CResultBar::GetCYMin());
			m_wndBarSplitter.SetRowInfo(1, CResultBar::GetCYMin(), 0);
		}
		else
		{
			m_wndBarSplitter.SetRowInfo(0, cy, cy-CResultBar::GetCYMin());
			m_wndBarSplitter.SetRowInfo(1, 0, 0);
		}
	}

	JFCFrame::OnSize(nType, cx, cy);
}

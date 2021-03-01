// on inclut les définitions nécessaires
#include "stdafx.h"
#include "ATPSplitterHypo.h"
#include "ResultBar.h"
#include ".\atpsplitterhypo.h"
#include "ATPPupitre.h"
#include "ATPPanelBas.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////
// le constructeur
//IMPLEMENT_DYNCREATE(CATPSplitterHypo, JFCFrame)
CATPSplitterHypo::CATPSplitterHypo()
{
	// on initialise le document
	m_pATPDocCreuset = 0;
}

///////////////////////////////////////
// la fonction pour définir le document

JVoid CATPSplitterHypo::SetDocument(JATPDocCreuset* pATPDocCreuset)
{
	// on mémorise le document
	m_pATPDocCreuset = pATPDocCreuset;

	((JFCSplitterPane*)m_wndPanelSplitter.GetPane(0,0))->SetDocument(m_pATPDocCreuset);
	((CATPPanelBas*)m_wndPanelSplitter.GetPane(1,0))->SetDocument(m_pATPDocCreuset);

	// on définit le document des composants
	((JFCSplitterPane*)m_wndCreusetResEch.GetPane(0,0))->SetDocument(m_pATPDocCreuset);
	((JFCSplitterPane*)m_wndCreusetResEch.GetPane(1,0))->SetDocument(m_pATPDocCreuset);
	((CResultBar*)m_wndBarSplitter.GetPane(1,0))->SetDocument(m_pATPDocCreuset);

	// on met à jour le composant
	this->OnUpdate();
}

//////////////////////////////////////////////
// la fonction pour mettre à jour le composant
VOID CATPSplitterHypo::UpdateAllViews(const JFCUpdate & Message)
{
	// on met à jour les composants
	if(!m_pATPDocCreuset->m_AFFCreuset.GetEchelleResult() && Message == UPDATE_INIPROGRESS)
	{
		((CATPResultHypo*)m_wndCreusetResEch.GetPane(1,0))->m_progress.SetRange(0,m_pATPDocCreuset->m_Progress.m_Range);
		((CATPResultHypo*)m_wndCreusetResEch.GetPane(1,0))->m_progress.SetStep(m_pATPDocCreuset->m_Progress.m_Step);
		((CATPResultHypo*)m_wndCreusetResEch.GetPane(1,0))->m_progress.SetPos(0);
		((CATPResultHypo*)m_wndCreusetResEch.GetPane(1,0))->m_progress.ShowWindow(SW_SHOW);
	}
	if(!m_pATPDocCreuset->m_AFFCreuset.GetEchelleResult() &&Message == UPDATE_PROGRESS)
		((CATPResultHypo*)m_wndCreusetResEch.GetPane(1,0))->m_progress.StepIt();
	if(!m_pATPDocCreuset->m_AFFCreuset.GetEchelleResult() &&Message == UPDATE_ENDPROGRESS)
		((CATPResultHypo*)m_wndCreusetResEch.GetPane(1,0))->m_progress.ShowWindow(SW_HIDE);

	((JFCSplitterPane*)m_wndPanelSplitter.GetPane(0,0))->Update();
	((CATPPanelBas*)m_wndPanelSplitter.GetPane(1,0))->Update();

	((JFCSplitterPane*)m_wndCreusetResEch.GetPane(0,0))->Update();
	((JFCSplitterPane*)m_wndCreusetResEch.GetPane(1,0))->Update();
	((CResultBar*)m_wndBarSplitter.GetPane(1,0))->Update();
	// on met à jour le composant
	this->OnUpdate();
}


/////////////////
// le destructeur

CATPSplitterHypo::~CATPSplitterHypo()
{
}

///////////////////////////////////////////////////////////////////
// les fonctions pour récupérer les dimensions minimales du panneau

LONG CATPSplitterHypo::GetCXMin()
{
	// on calcule la largeur des composants
	LONG CX1 = CATPCreusetHypo::GetCXMin();

	// on calcule la largeur des bordures et des interstices
	LONG CX2 = 0;

	// on renvoie la largeur
	return (CX1 + CX2);
}

LONG CATPSplitterHypo::GetCYMin()
{
	// on calcule la hauteur des composants
	LONG CY1 = CATPCreusetHypo::GetCYMin() + CATPEchelle::GetCYMin();

	// on calcule la largeur des bordures et des interstices
	LONG CY2 =  CResultBar::GetCYMin();

	// on renvoie la hauteur
	return (CY1 + CY2);
}

BEGIN_MESSAGE_MAP(CATPSplitterHypo, JFCFrame)
	//{{AFX_MSG_MAP(CATPSplitterHypo)
	//}}AFX_MSG_MAP
	ON_WM_SIZE()
END_MESSAGE_MAP()

//////////////////////////////////////////////
// la fonction pour appliquer les mises à jour

JVoid CATPSplitterHypo::OnUpdate()
{
	// on ne fait rien
	if (m_pATPDocCreuset)
	{
		if(m_pATPDocCreuset->m_AFFCreuset.GetEchelleResult())
		{
			if (m_wndCreusetResEch.GetPane(1,0)->IsKindOf(RUNTIME_CLASS(CATPResultHypo)))
			{
				m_wndBarSplitter.HideBar(1,false);
				((JFCSplitterPane*)m_wndCreusetResEch.GetPane(0,0))->RemoveScroll(&((CATPResultHypo*)m_wndCreusetResEch.GetPane(1,0))->SetHorzScrollBar1(((CATPCreusetHypo*)m_wndCreusetResEch.GetPane(0,0))->GetHorzScrollBar()));
				((JFCSplitterPane*)m_wndCreusetResEch.GetPane(0,0))->RemoveScroll(&((CATPResultHypo*)m_wndCreusetResEch.GetPane(1,0))->SetHorzScrollBar2(((CATPCreusetHypo*)m_wndCreusetResEch.GetPane(0,0))->GetHorzScrollBar()));
			
				m_wndCreusetResEch.ReplaceView(1, 0,	RUNTIME_CLASS(CATPEchelle), CSize(CATPEchelle::GetCXMin(), CATPEchelle::GetCYMin()));
				((JFCSplitterPane*)m_wndCreusetResEch.GetPane(1,0))->SetDocument(m_pATPDocCreuset);
				m_wndCreusetResEch.SetRowInfo(1,CATPEchelle::GetCYMin(),CATPEchelle::GetCYMin());
				m_wndCreusetResEch.RecalcLayout();

			}
		}
		else
		{
			if (m_wndCreusetResEch.ReplaceView(1, 0,
				RUNTIME_CLASS(CATPResultHypo), CSize(CATPResultHypo::GetCXMin(), CATPResultHypo::GetCYMin())))
			{
				m_wndBarSplitter.HideBar(1,true);
				((JFCSplitterPane*)m_wndCreusetResEch.GetPane(1,0))->SetDocument(m_pATPDocCreuset);

				((CATPCreusetHypo*)m_wndCreusetResEch.GetPane(0,0))->SetGraphe(((CATPResultHypo*)m_wndCreusetResEch.GetPane(1,0))->SetHorzScrollBar1(((CATPCreusetHypo*)m_wndCreusetResEch.GetPane(0,0))->GetHorzScrollBar()));
				((CATPCreusetHypo*)m_wndCreusetResEch.GetPane(0,0))->SetGraphe(((CATPResultHypo*)m_wndCreusetResEch.GetPane(1,0))->SetHorzScrollBar2(((CATPCreusetHypo*)m_wndCreusetResEch.GetPane(0,0))->GetHorzScrollBar()));

				m_wndCreusetResEch.SetRowInfo(1, CATPEchelle::GetCYMin(), CATPEchelle::GetCYMin());
				m_wndCreusetResEch.RecalcLayout();
			}
		}
	}
}

//////////////////////////////////
// CATPSplitterHypo message handlers

void CATPSplitterHypo::Invalidate(BOOL bErase)
{
	m_wndPanelSplitter.GetPane(0,0)->Invalidate(bErase);
	m_wndPanelSplitter.GetPane(1,0)->Invalidate(bErase);
	m_wndCreusetResEch.GetPane(0,0)->Invalidate(bErase);
	m_wndCreusetResEch.GetPane(1,0)->Invalidate(bErase);
	m_wndBarSplitter.GetPane(1,0)->Invalidate(bErase);
}

void CATPSplitterHypo::SetColors()
{
	((JFCSplitterPane*)m_wndPanelSplitter.GetPane(0,0))->SetColors();
	((CATPPanelBas*)m_wndPanelSplitter.GetPane(1,0))->SetColors();
	((JFCSplitterPane*)m_wndCreusetResEch.GetPane(0,0))->SetColors();
	((JFCSplitterPane*)m_wndCreusetResEch.GetPane(1,0))->SetColors();
	((CResultBar*)m_wndBarSplitter.GetPane(1,0))->SetColors();
}


BOOL CATPSplitterHypo::OnCreateClient(LPCREATESTRUCT,	 CCreateContext* pContext)
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
	////On cree les panneaux du Splitter Bar////
	if (!m_wndPanelSplitter.CreateView(0, 0,
		RUNTIME_CLASS(CATPPupitre), CSize(CATPPupitre::GetCXMin(), CATPPupitre::GetCYMin()), pContext))
	{
		TRACE0("Failed to first second pane\n");
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
		RUNTIME_CLASS(CATPCreusetHypo), CSize(CATPCreusetHypo::GetCXMin(), CATPCreusetHypo::GetCYMin()), pContext))
	{
		TRACE0("Failed to create creuset pane\n");
		return FALSE;
	}

	// le resultat
	if (!m_wndCreusetResEch.CreateView(1, 0,
		RUNTIME_CLASS(CATPResultHypo), CSize(CATPResultHypo::GetCXMin(), CATPResultHypo::GetCYMin()), pContext))
	{
		TRACE0("Failed to create echelle pane\n");
		return FALSE;
	}


	m_wndCreusetResEch.SetRowInfo(0,CATPCreusetHypo::GetCYIdeal(),CATPCreusetHypo::GetCYMin());

	m_wndVertSplitter.ClickBar();
	m_wndPanelSplitter.LockBar();

	m_wndBarSplitter.LockBar();

	return TRUE;
}

void CATPSplitterHypo::OnSize(UINT nType, int cx, int cy)
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



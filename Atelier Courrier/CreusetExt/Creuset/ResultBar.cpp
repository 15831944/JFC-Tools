// ResultBar.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "ResultBar.h"
#include ".\resultbar.h"

#include "DialogVentilation.h"

// CBtnFrmV

IMPLEMENT_DYNCREATE(CResultBar, ETSLayoutFormView)

CResultBar::CResultBar()
	: ETSLayoutFormView(CResultBar::IDD)
{
}

CResultBar::~CResultBar()
{
}

JVoid CResultBar::SetDocument(JATPDocCreuset* pATPDocCreuset)
{
	m_pATPDocCreuset = pATPDocCreuset;
}

LONG CResultBar::GetCXMin()
{
	// on calcule la largeur des composants
	LONG CX1 = 5 * XLn;

	// on calcule la largeur des bordures et des interstices
	LONG CX2 =  2 * XBorder + 4 * XInter;

	// on renvoie la largeur
	return (CX1 + CX2);
}

LONG CResultBar::GetCYMin()
{
	// on renvoie la hauteur
	return 44;
}

void CResultBar::DoDataExchange(CDataExchange* pDX)
{
	ETSLayoutFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CR_B1RG,			m_BtnRG);
	DDX_Control(pDX, IDC_CR_B2DISTRIN,		m_BtnDistribN);
	DDX_Control(pDX, IDC_CR_B3DISTRIC,		m_BtnDistribC);
	DDX_Control(pDX, IDC_CR_B4VTL,			m_BtnVentil);
	DDX_Control(pDX, IDC_CR_B5MEMDEMEMO,	m_BtnMemo);
	DDX_Control(pDX, IDC_CR_BTNEVAL,		m_BtnEval);
	DDX_Control(pDX, IDC_CR_B6GRAPH,		m_BtnGraph);
}

BEGIN_MESSAGE_MAP(CResultBar, ETSLayoutFormView)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_CR_B1RG,							OnBtnRG)
	ON_BN_CLICKED(IDC_CR_B2DISTRIN,						OnBtnDistriN)
	ON_BN_CLICKED(IDC_CR_B3DISTRIC,						OnBtnDistriC)
	ON_BN_CLICKED(IDC_CR_B4VTL,							OnBtnVtl)
	ON_BN_CLICKED(IDC_CR_B5MEMDEMEMO,					OnContrib)
	ON_BN_CLICKED(IDC_CR_BTNEVAL,						OnBtnEval)

	ON_COMMAND(IDM_CR_CONTRIBCOUV,						OnContribCouv)
	ON_COMMAND(IDM_CR_CONTRIBGRP,						OnContribGRP)
	ON_COMMAND(IDM_CR_CONTRIBPGRP,						OnContribPGRP)
	ON_COMMAND(IDM_CR_CONTRIBBUDG,						OnContribBudg)
	ON_COMMAND(IDM_CR_CONTRIBPBUDG,						OnContribPBudg)
	ON_COMMAND(IDM_CR_CONTRIBCTCOUV,					OnContribCtCouv)

	ON_COMMAND(IDM_CR_CONTRIB_MODE_SUPPORTS,			OnContribModeSupports)
	ON_COMMAND(IDM_CR_CONTRIB_MODE_GRANDEURS,			OnContribModeGrandeurs)

	ON_UPDATE_COMMAND_UI(IDM_CR_CONTRIBCOUV,			OnUpdContribCouv)
	ON_UPDATE_COMMAND_UI(IDM_CR_CONTRIBGRP,				OnUpdContribGRP)
	ON_UPDATE_COMMAND_UI(IDM_CR_CONTRIBPGRP,			OnUpdContribPGRP)
	ON_UPDATE_COMMAND_UI(IDM_CR_CONTRIBBUDG,			OnUpdContribBudg)
	ON_UPDATE_COMMAND_UI(IDM_CR_CONTRIBPBUDG,			OnUpdContribPBudg)
	ON_UPDATE_COMMAND_UI(IDM_CR_CONTRIBCTCOUV,			OnUpdContribCtCouv)

	ON_UPDATE_COMMAND_UI(IDM_CR_CONTRIB_MODE_SUPPORTS,	OnUpdContribModeSupports)
	ON_UPDATE_COMMAND_UI(IDM_CR_CONTRIB_MODE_GRANDEURS,	OnUpdContribModeGrandeurs)
	
	ON_COMMAND(ID_VENTIL_NBCONTACTS,					OnVentilNbContacts)
	ON_COMMAND(ID_VENTIL_AUDIENCE000,					OnVentilAudience000)
	ON_COMMAND(ID_VENTIL_GRP,							OnVentilGRP)
	ON_COMMAND(ID_VENTIL_INDGRP,						OnVentilIndGRP)
	ON_COMMAND(ID_VENTIL_COUV,							OnVentilCouv)
	ON_COMMAND(ID_VENTIL_INDCOUV,						OnVentilIndCouv)
	ON_COMMAND(ID_VENTIL_R,								OnVentilRepet)
	
	ON_UPDATE_COMMAND_UI(ID_VENTIL_NBCONTACTS,			OnUpdVentilNbContacts)
	ON_UPDATE_COMMAND_UI(ID_VENTIL_AUDIENCE000,			OnUpdVentilAudience000)
	ON_UPDATE_COMMAND_UI(ID_VENTIL_GRP,					OnUpdVentilGRP)
	ON_UPDATE_COMMAND_UI(ID_VENTIL_INDGRP,				OnUpdVentilIndGRP)
	ON_UPDATE_COMMAND_UI(ID_VENTIL_COUV,				OnUpdVentilCouv)
	ON_UPDATE_COMMAND_UI(ID_VENTIL_INDCOUV,				OnUpdVentilIndCouv)
	ON_UPDATE_COMMAND_UI(ID_VENTIL_R,					OnUpdVentilRepet)

	// Commandes concernant les graphes
	ON_COMMAND(IDM_CR_GRAPH_GRPJOUR,					OnGraphGRPJour)
	ON_COMMAND(IDM_CR_GRAPH_GRPCUM,						OnGraphGRPCum)
	ON_COMMAND(IDM_CR_GRAPH_COUVCUM,					OnGraphCouvCum)
	ON_COMMAND(IDM_CR_GRAPH_APPORTCOUVCUM,				OnGraphApportCouvCum)

	ON_UPDATE_COMMAND_UI(IDM_CR_GRAPH_GRPJOUR,			OnUpdGraphGRPJour)
	ON_UPDATE_COMMAND_UI(IDM_CR_GRAPH_GRPCUM,			OnUpdGraphGRPCum)
	ON_UPDATE_COMMAND_UI(IDM_CR_GRAPH_COUVCUM,			OnUpdGraphCouvCum)
	ON_UPDATE_COMMAND_UI(IDM_CR_GRAPH_APPORTCOUVCUM,	OnUpdGraphApportCouvCum)

	ON_COMMAND(ID_VENTIL_ITEMSDEVENTILATION,OnItemVtl)
	
END_MESSAGE_MAP()


// Diagnostics CBtnFrmV

#ifdef _DEBUG
void CResultBar::AssertValid() const
{
	ETSLayoutFormView::AssertValid();
}

void CResultBar::Dump(CDumpContext& dc) const
{
	ETSLayoutFormView::Dump(dc);
}
#endif //_DEBUG


// Gestionnaires de messages CBtnFrmV
void CResultBar::OnInitialUpdate()
{
	ETSLayoutFormView::OnInitialUpdate();

	CString str;
	str.LoadString(IDS_CR_RESULTG);
	m_BtnRG.SetWindowText(str);
	str.LoadString(IDS_CR_DISTRIBNETTE);
	m_BtnDistribN.SetWindowText(str);
	str.LoadString(IDS_CR_DISTRIBCUMUL);
	m_BtnDistribC.SetWindowText(str);
	str.LoadString(IDS_CR_VENTIL);
	m_BtnVentil.SetWindowText(str);
	
	//str.LoadString(IDS_CR_MEMODEMEMO);
	//m_BtnMemo.SetWindowText(str);
	m_BtnMemo.SetWindowText("Contribution");

	// Le bouton pour les graphiques
	str.LoadString(IDS_CR_GRAPH);
	m_BtnGraph.SetWindowText(str);

	UpdateLayout(pane(HORIZONTAL)	<<item (IDC_CR_B1RG,		GREEDY)
									<<item (IDC_CR_B2DISTRIN,	GREEDY)
									<<item (IDC_CR_B3DISTRIC,	GREEDY)
									<<item (IDC_CR_B4VTL,		GREEDY)
									<<item (IDC_CR_B5MEMDEMEMO, GREEDY)
									<<item (IDC_CR_BTNEVAL,		GREEDY)
									<<item (IDC_CR_B6GRAPH,		GREEDY)
									);

	SetColors();
}

void CResultBar::SetColors()
{
	m_BtnRG.SetColor( CButtonST::BTNST_COLOR_BK_IN,			CATPColors::GetColorLight(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );
	m_BtnRG.SetColor( CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );
	m_BtnRG.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS,		CATPColors::GetColorDark(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );

	m_BtnDistribN.SetColor( CButtonST::BTNST_COLOR_BK_IN,	CATPColors::GetColorLight(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );
	m_BtnDistribN.SetColor( CButtonST::BTNST_COLOR_BK_OUT,	CATPColors::GetColorMedium(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );
	m_BtnDistribN.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS,CATPColors::GetColorDark(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );

	m_BtnDistribC.SetColor( CButtonST::BTNST_COLOR_BK_IN,	CATPColors::GetColorLight(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );
	m_BtnDistribC.SetColor( CButtonST::BTNST_COLOR_BK_OUT,	CATPColors::GetColorMedium(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );
	m_BtnDistribC.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS,CATPColors::GetColorDark(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );

	m_BtnVentil.SetColor( CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );
	m_BtnVentil.SetColor( CButtonST::BTNST_COLOR_BK_OUT,	CATPColors::GetColorMedium(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );
	m_BtnVentil.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );

	m_BtnMemo.SetColor( CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );
	m_BtnMemo.SetColor( CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );
	m_BtnMemo.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );

	m_BtnGraph.SetColor( CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );
	m_BtnGraph.SetColor( CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );
	m_BtnGraph.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );

	m_BtnEval.SetColor( CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorDark(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );
	m_BtnEval.SetColor( CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorSelect(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );
	m_BtnEval.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );

	/*
	m_BtnEval.SetColor( CButtonST::BTNST_COLOR_FG_IN,		CATPColors::GetColorWhite(),CATPColors::COLORCREUSET );
	m_BtnEval.SetColor( CButtonST::BTNST_COLOR_FG_OUT,		CATPColors::GetColorWhite(),CATPColors::COLORCREUSET );
	m_BtnEval.SetColor( CButtonST::BTNST_COLOR_FG_FOCUS,	CATPColors::GetColorWhite(),CATPColors::COLORCREUSET );
	*/


	Invalidate();
}

void CResultBar::OnSize(UINT nType, int cx, int cy)
{
	UpdateLayout(
				pane(HORIZONTAL)
							<<item(IDC_CR_B1RG,			GREEDY)
							<<item(IDC_CR_B2DISTRIN,	GREEDY)
							<<item(IDC_CR_B3DISTRIC,	GREEDY)
							<<item(IDC_CR_B4VTL,		GREEDY)
							<<item(IDC_CR_B5MEMDEMEMO,	GREEDY)
							<<item (IDC_CR_BTNEVAL,		GREEDY)
							<<item(IDC_CR_B6GRAPH,		GREEDY)
			     );

				 /*
				 IDC_CR_B1RG,			m_BtnRG);
	DDX_Control(pDX, IDC_CR_B2DISTRIN,		m_BtnDistribN);
	DDX_Control(pDX, IDC_CR_B3DISTRIC,		m_BtnDistribC);
	DDX_Control(pDX, IDC_CR_B4VTL,			m_BtnVentil);
	DDX_Control(pDX, IDC_CR_B5MEMDEMEMO,	m_BtnMemo);
	DDX_Control(pDX, IDC_CR_BTNEVAL,		m_BtnEval);
	DDX_Control(pDX, IDC_CR_B6GRAPH,		m_BtnGraph);
				 */

	ETSLayoutFormView::OnSize(nType, cx, cy);
}

void CResultBar::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// N'appelez pas CFormView::OnPaint() pour la peinture des messages
	RECT Rect;
	// on récupère la zone cliente
	this->GetClientRect(&Rect);

	CBrush Brush(CATPColors::GetColorMedium(CATPColors::COLORCREUSET));

	// on dessine le fond
	dc.FillRect(&Rect, &Brush);
}

JVoid CResultBar::Update()
{

	CheckRadioButton(IDC_CR_B1RG, IDC_CR_B5MEMDEMEMO, IDC_CR_B1RG);

	if(m_pATPDocCreuset->m_AFFDistributionN)
		CheckRadioButton(IDC_CR_B1RG, IDC_CR_B5MEMDEMEMO, IDC_CR_B2DISTRIN);
	if(m_pATPDocCreuset->m_AFFDistributionC)
		CheckRadioButton(IDC_CR_B1RG, IDC_CR_B5MEMDEMEMO, IDC_CR_B3DISTRIC);

	if(m_pATPDocCreuset->m_AFFVentilation.IsVisible())
	{
		CheckRadioButton(IDC_CR_B1RG, IDC_CR_B5MEMDEMEMO, IDC_CR_B4VTL);
	}

	//m_BtnMemo.ShowWindow(SW_HIDE);
	m_BtnMemo.SetWindowText("Contribution");
	if(m_pATPDocCreuset->m_AFFContribution_New.IsVisible())
	{
		CheckRadioButton(IDC_CR_B1RG, IDC_CR_B5MEMDEMEMO, IDC_CR_B5MEMDEMEMO);
	}

	// Bouton graphiques
	m_BtnGraph.SetWindowText("xxxxx");

	m_BtnEval.ShowWindow( m_pATPDocCreuset->m_bCalculsDiff? SW_SHOW : SW_HIDE );


	if (m_pATPDocCreuset->m_AFFCreuset.GetAffQuickPlan()|| !m_pATPDocCreuset->m_AFFCreuset.GetAffHypo())
	{
		m_BtnVentil.ShowWindow(FALSE);
        m_BtnMemo.ShowWindow(FALSE);
	}
	else if (!m_pATPDocCreuset->m_AFFCreuset.GetAffQuickPlan() || m_pATPDocCreuset->m_AFFCreuset.GetAffHypo())
	{
		m_BtnVentil.ShowWindow(TRUE);
		m_BtnMemo.ShowWindow(TRUE);
	}
	/*
	m_BtnVentil.ShowWindow( m_pATPDocCreuset->m_AFFCreuset.GetAffQuickPlan()? SW_HIDE : SW_SHOW);
	m_BtnMemo.ShowWindow( m_pATPDocCreuset->m_AFFCreuset.GetAffQuickPlan()? SW_HIDE : SW_SHOW);

	m_BtnVentil.ShowWindow( m_pATPDocCreuset->m_AFFCreuset.GetAffHypo()? SW_SHOW : SW_HIDE);
	m_BtnMemo.ShowWindow( m_pATPDocCreuset->m_AFFCreuset.GetAffHypo()? SW_SHOW : SW_HIDE);
	*/

	// Menu Contribution
	m_BtnMemo.SetTypeBtnMenu(TRUE,FALSE); 
	m_BtnMemo.SetMenu(IDR_CR_MENUCONTRIB, this,FALSE);

	// Menu Ventilation
	m_BtnVentil.SetTypeBtnMenu(TRUE,FALSE); 
	m_BtnVentil.SetMenu(IDR_CR_VENTIL, this,FALSE);

	// En + le menu pour les graphiques
	m_BtnGraph.ShowWindow(m_pATPDocCreuset->m_AFFCreuset.GetAffPlan()? SW_SHOW : SW_HIDE);
	m_BtnGraph.SetTypeBtnMenu(TRUE,FALSE);
	m_BtnGraph.SetMenu(IDR_CR_MENUGRAPH, this,FALSE);


	// Mise à jour 
	OnInitialUpdate();
}

void CResultBar::OnBtnRG()
{
	m_pATPDocCreuset->m_AFFDistributionN = 0;
	m_pATPDocCreuset->m_AFFDistributionC = 0;
	m_pATPDocCreuset->m_AFFVentilation.SetVisible(false);
	m_pATPDocCreuset->m_AFFContribution_New.SetVisible(false);
	m_pATPDocCreuset->m_AFFMemoDememo = 0;
	m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_RESULTBAR);
}

void CResultBar::OnBtnDistriN()
{
	m_pATPDocCreuset->m_AFFDistributionN = 1;
	m_pATPDocCreuset->m_AFFDistributionC = 0;
	m_pATPDocCreuset->m_AFFVentilation.SetVisible(false);
	m_pATPDocCreuset->m_AFFContribution_New.SetVisible(false);
	m_pATPDocCreuset->m_AFFMemoDememo = 0;
	m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_RESULTBAR);
}

void CResultBar::OnBtnDistriC()
{
	m_pATPDocCreuset->m_AFFDistributionN = 0;
	m_pATPDocCreuset->m_AFFDistributionC = 1;
	m_pATPDocCreuset->m_AFFVentilation.SetVisible(false);
	m_pATPDocCreuset->m_AFFContribution_New.SetVisible(false);
	m_pATPDocCreuset->m_AFFMemoDememo = 0;
	m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_RESULTBAR);
}

void CResultBar::OnBtnVtl()
{
	if(m_pATPDocCreuset &&	 m_pATPDocCreuset->IsValid()/* &&
			(m_pATPDocCreuset->m_AFFVentilation.IsVisible() || m_pATPDocCreuset->m_AFFVentilation.GetNbItem()==0 )*/)
	{
		m_pATPDocCreuset->m_AFFDistributionN = 0;
		m_pATPDocCreuset->m_AFFDistributionC = 0;
		m_pATPDocCreuset->m_AFFVentilation.SetVisible(true);
		m_pATPDocCreuset->m_AFFContribution_New.SetVisible(false);
		m_pATPDocCreuset->m_AFFMemoDememo = 0;
		m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_RESULTBAR);
	}
}

void CResultBar::OnItemVtl()
{
	if(m_pATPDocCreuset &&	 m_pATPDocCreuset->IsValid()/* &&
			(m_pATPDocCreuset->m_AFFVentilation.IsVisible() || m_pATPDocCreuset->m_AFFVentilation.GetNbItem()==0 )*/)
	{
		CDialogVentilation dlg(this);
		dlg.SetDocument(m_pATPDocCreuset);
		CKeyTerrain keyterrain;
		m_pATPDocCreuset->m_MapIdTerrainsAudienceSel.MoveFirst();
		keyterrain.Set(m_pATPDocCreuset->m_IdSrcAudienceSel, m_pATPDocCreuset->m_MapIdTerrainsAudienceSel.GetKey());
		dlg.SetTerrain(keyterrain);

		if(dlg.DoModal()==IDOK)
		{
			//======>	//basculer en tableur quand en calendrier
			if(m_pATPDocCreuset->m_AFFCreuset.GetAffHypo() == FALSE)
				if(m_pATPDocCreuset->m_AFFZoomPlan.IsVisible())
			{
				m_pATPDocCreuset->m_AFFZoomPlan.SetVisible(false);
				m_pATPDocCreuset->m_AFFContribution.SetVisible(true);
			}

			m_pATPDocCreuset->m_AFFCreuset.SetPlanActif(false);
			m_pATPDocCreuset->m_AFFCreuset.SetAffPlan(false);
			m_pATPDocCreuset->m_AFFCreuset.SetHypoActif(true);
			m_pATPDocCreuset->m_AFFCreuset.SetAffHypo(true);

			m_pATPDocCreuset->m_AFFDistributionN = 0;
			m_pATPDocCreuset->m_AFFDistributionC = 0;
			m_pATPDocCreuset->m_AFFVentilation.SetVisible(true);
			m_pATPDocCreuset->m_AFFContribution_New.SetVisible(false);
			m_pATPDocCreuset->m_AFFMemoDememo = 0;
			m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_RESULTBAR);
		}

	}
}

void CResultBar::OnContrib() 
{
	if(m_pATPDocCreuset->m_AFFZoomPlan.IsVisible())
	{
		m_pATPDocCreuset->m_AFFZoomPlan.SetVisible(false);
		m_pATPDocCreuset->m_AFFContribution.SetVisible(true);
	}

	m_pATPDocCreuset->m_AFFCreuset.SetPlanActif(false);
	m_pATPDocCreuset->m_AFFCreuset.SetAffPlan(false);
	m_pATPDocCreuset->m_AFFCreuset.SetHypoActif(true);
	m_pATPDocCreuset->m_AFFCreuset.SetAffHypo(true);

	m_pATPDocCreuset->m_AFFDistributionN = 0;
	m_pATPDocCreuset->m_AFFDistributionC = 0;
	m_pATPDocCreuset->m_AFFVentilation.SetVisible(false);
	m_pATPDocCreuset->m_AFFContribution_New.SetVisible(true);


	// Par défaut au moins 1ere grandeur de sélectionner
	if (m_pATPDocCreuset->m_AFFContribution_New.NbValue() == 0)
	{
		m_pATPDocCreuset->m_AFFContribution_New.SetType(JATPAFFContrib_New::COUV);
		m_pATPDocCreuset->m_AFFContribution_New.ShowCouv(true);
	}	

	m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_CONTRIBNEW);
}

void CResultBar::OnContribCouv()
{
	m_pATPDocCreuset->m_AFFContribution_New.SetType(JATPAFFContrib_New::COUV);

	if (m_pATPDocCreuset->m_AFFContribution_New.CanCouv())
		m_pATPDocCreuset->m_AFFContribution_New.ShowCouv(false);
	else
		m_pATPDocCreuset->m_AFFContribution_New.ShowCouv(true);

	m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_CONTRIBNEW);

	//OnContrib();
}

void CResultBar::OnUpdContribCouv(CCmdUI *pCmdUI)
{
	// Par dfaut au moins 1ere grandeur de sélectionner
	if (m_pATPDocCreuset->m_AFFContribution_New.NbValue() == 0)
	{
		m_pATPDocCreuset->m_AFFContribution_New.SetType(JATPAFFContrib_New::COUV);
		m_pATPDocCreuset->m_AFFContribution_New.ShowCouv(true);
	}	

	pCmdUI->SetCheck(m_pATPDocCreuset->m_AFFContribution_New.CanCouv());
}

void CResultBar::OnContribGRP()
{
	m_pATPDocCreuset->m_AFFContribution_New.SetType(JATPAFFContrib_New::GRP);

	if (m_pATPDocCreuset->m_AFFContribution_New.CanGRP())
		m_pATPDocCreuset->m_AFFContribution_New.ShowGRP(false);
	else
		m_pATPDocCreuset->m_AFFContribution_New.ShowGRP(true);

	m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_CONTRIBNEW);

	//OnContrib();
}

void CResultBar::OnUpdContribGRP(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_pATPDocCreuset->m_AFFContribution_New.CanGRP());
}

void CResultBar::OnContribPGRP()
{
	m_pATPDocCreuset->m_AFFContribution_New.SetType(JATPAFFContrib_New::PARTGRP);

	if (m_pATPDocCreuset->m_AFFContribution_New.CanPGRP())
		m_pATPDocCreuset->m_AFFContribution_New.ShowPGRP(false);
	else
		m_pATPDocCreuset->m_AFFContribution_New.ShowPGRP(true);

	m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_CONTRIBNEW);
}

void CResultBar::OnUpdContribPGRP(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_pATPDocCreuset->m_AFFContribution_New.CanPGRP());
}

void CResultBar::OnContribBudg()
{
	m_pATPDocCreuset->m_AFFContribution_New.SetType(JATPAFFContrib_New::BUDGET);
	
	if (m_pATPDocCreuset->m_AFFContribution_New.CanBudg())
		m_pATPDocCreuset->m_AFFContribution_New.ShowBudg(false);
	else
		m_pATPDocCreuset->m_AFFContribution_New.ShowBudg(true);

	m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_CONTRIBNEW);
}

void CResultBar::OnUpdContribBudg(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_pATPDocCreuset->m_AFFContribution_New.CanBudg());
}

void CResultBar::OnContribPBudg()
{
	m_pATPDocCreuset->m_AFFContribution_New.SetType(JATPAFFContrib_New::PARTBUDGET);

	if (m_pATPDocCreuset->m_AFFContribution_New.CanPBudg())
		m_pATPDocCreuset->m_AFFContribution_New.ShowPBudg(false);
	else
		m_pATPDocCreuset->m_AFFContribution_New.ShowPBudg(true);

	m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_CONTRIBNEW);
}

void CResultBar::OnUpdContribPBudg(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_pATPDocCreuset->m_AFFContribution_New.CanPBudg());
}

void CResultBar::OnContribCtCouv()
{
	m_pATPDocCreuset->m_AFFContribution_New.SetType(JATPAFFContrib_New::CTCOUV);
	
	if (m_pATPDocCreuset->m_AFFContribution_New.CanCtCouv())
		m_pATPDocCreuset->m_AFFContribution_New.ShowCtCouv(false);
	else
		m_pATPDocCreuset->m_AFFContribution_New.ShowCtCouv(true);

	m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_CONTRIBNEW);
}

void CResultBar::OnUpdContribCtCouv(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_pATPDocCreuset->m_AFFContribution_New.CanCtCouv());

}

// Sélection
void CResultBar::OnContribModeSupports()
{
	m_pATPDocCreuset->m_AFFContribution_New.SetModeAffich(JATPAFFContrib_New::AFFICH_BLOC_SUPPORTS);
	m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_CONTRIBNEW);
}

void CResultBar::OnUpdContribModeSupports(CCmdUI *pCmdUI)
{
	if (m_pATPDocCreuset->m_AFFContribution_New.GetModeAffich() == JATPAFFContrib_New::AFFICH_BLOC_SUPPORTS)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}

void CResultBar::OnContribModeGrandeurs()
{
	m_pATPDocCreuset->m_AFFContribution_New.SetModeAffich(JATPAFFContrib_New::AFFICH_BLOC_GRANDEURS);
	m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_CONTRIBNEW);
}

void CResultBar::OnUpdContribModeGrandeurs(CCmdUI *pCmdUI)
{
	if (m_pATPDocCreuset->m_AFFContribution_New.GetModeAffich() == JATPAFFContrib_New::AFFICH_BLOC_GRANDEURS)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}

void CResultBar::OnVentilNbContacts()
{
	m_pATPDocCreuset->m_AFFVentilation.ShowNbContacts(!m_pATPDocCreuset->m_AFFVentilation.CanNbContacts());
	m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_RESULTBAR);
}

void CResultBar::OnVentilAudience000()
{
	m_pATPDocCreuset->m_AFFVentilation.ShowAudience000(!m_pATPDocCreuset->m_AFFVentilation.CanAudience000());
	m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_RESULTBAR);
}
void CResultBar::OnVentilGRP()
{
	m_pATPDocCreuset->m_AFFVentilation.ShowGRP(!m_pATPDocCreuset->m_AFFVentilation.CanGRP());
	m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_RESULTBAR);
}
void CResultBar::OnVentilIndGRP()
{
	m_pATPDocCreuset->m_AFFVentilation.ShowIndGRP(!m_pATPDocCreuset->m_AFFVentilation.CanIndGRP());
	m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_RESULTBAR);
}
void CResultBar::OnVentilCouv()
{
	m_pATPDocCreuset->m_AFFVentilation.ShowCouv(!m_pATPDocCreuset->m_AFFVentilation.CanCouv());
		m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_RESULTBAR);
}
void CResultBar::OnVentilIndCouv()
{
	m_pATPDocCreuset->m_AFFVentilation.ShowIndCouv(!m_pATPDocCreuset->m_AFFVentilation.CanIndCouv());
		m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_RESULTBAR);
}
void CResultBar::OnVentilRepet()
{
	m_pATPDocCreuset->m_AFFVentilation.ShowRepet(!m_pATPDocCreuset->m_AFFVentilation.CanRepet());
	m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_RESULTBAR);
}

void CResultBar::OnUpdVentilNbContacts(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_pATPDocCreuset->m_AFFVentilation.CanNbContacts());
}

void CResultBar::OnUpdVentilAudience000(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_pATPDocCreuset->m_AFFVentilation.CanAudience000());
}

void CResultBar::OnUpdVentilGRP(CCmdUI *pCmdUI)
{
	//BOOL bEnable = m_pATPDocCreuset->m_AFFVentilation.IsVisible() || m_pATPDocCreuset->m_AFFVentilation.GetNbItem()!=0;
	//pCmdUI->Enable(bEnable);
	pCmdUI->SetCheck(m_pATPDocCreuset->m_AFFVentilation.CanGRP());
}
void CResultBar::OnUpdVentilIndGRP(CCmdUI *pCmdUI)
{
	//BOOL bEnable = m_pATPDocCreuset->m_AFFVentilation.IsVisible() || m_pATPDocCreuset->m_AFFVentilation.GetNbItem()!=0;
	//pCmdUI->Enable(bEnable);
	pCmdUI->SetCheck(m_pATPDocCreuset->m_AFFVentilation.CanIndGRP());
}
void CResultBar::OnUpdVentilCouv(CCmdUI *pCmdUI)
{
	//BOOL bEnable = m_pATPDocCreuset->m_AFFVentilation.IsVisible() || m_pATPDocCreuset->m_AFFVentilation.GetNbItem()!=0;
	//pCmdUI->Enable(bEnable);
	pCmdUI->SetCheck(m_pATPDocCreuset->m_AFFVentilation.CanCouv());
}
void CResultBar::OnUpdVentilIndCouv(CCmdUI *pCmdUI)
{
	//BOOL bEnable = m_pATPDocCreuset->m_AFFVentilation.IsVisible() || m_pATPDocCreuset->m_AFFVentilation.GetNbItem()!=0;
	//pCmdUI->Enable(bEnable);
	pCmdUI->SetCheck(m_pATPDocCreuset->m_AFFVentilation.CanIndCouv());
}
void CResultBar::OnUpdVentilRepet(CCmdUI *pCmdUI)
{
	//BOOL bEnable = m_pATPDocCreuset->m_AFFVentilation.IsVisible() || m_pATPDocCreuset->m_AFFVentilation.GetNbItem()!=0;
	//pCmdUI->Enable(bEnable);
	pCmdUI->SetCheck(m_pATPDocCreuset->m_AFFVentilation.CanRepet());
}


void CResultBar::OnBtnEval()
{
	m_pATPDocCreuset->UpdateAllViews(UPDATE_ETUDE);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//											LES GRAPHIQUES GRP ET COUV
// 
// Appel graphique GRP Jour
void CResultBar::OnGraphGRPJour()
{
	m_pATPDocCreuset->m_AFFResultPlan.SetGraphGRPJour();
	m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_RESULT_PLAN);
}

//////////////////////////////////////////////////////
// Appel graphique GRP Cumulee
void CResultBar::OnGraphGRPCum()
{
	m_pATPDocCreuset->m_AFFResultPlan.SetGraphGRPCumul();
	m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_RESULT_PLAN);
}

//////////////////////////////////////////////////////
// Appel graphique C% cumulée
void CResultBar::OnGraphCouvCum()
{
	m_pATPDocCreuset->m_AFFResultPlan.SetGraphCouvCumul();
	m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_RESULT_PLAN);
}

//////////////////////////////////////////////////////
// Appel graphique Apport C% Jour
void CResultBar::OnGraphApportCouvCum()
{
	m_pATPDocCreuset->m_AFFResultPlan.SetGraphCouvJour();
	m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_RESULT_PLAN);
}

//////////////////////////////////////////////////////
// Validation ou dévalidation menu Graphe GRP Jour
void CResultBar::OnUpdGraphGRPJour(CCmdUI *pCmdUI)
{
	// Toujours Valide
	pCmdUI->Enable(TRUE); 
}

void CResultBar::OnUpdGraphGRPCum(CCmdUI *pCmdUI)
{
	// Toujours Valide
	pCmdUI->Enable(TRUE); 
}

void CResultBar::OnUpdGraphCouvCum(CCmdUI *pCmdUI)
{
	// Valide ssi moteur probabiliste
	// m_pATPDocCreuset->m_MoteurPresse.GetType() == CMoteurPresse::PROBA ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE); 
	m_pATPDocCreuset->m_MoteurPresse.GetType() == CMoteurPresse::ABAQUE_ETAL ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE); 
}

void CResultBar::OnUpdGraphApportCouvCum(CCmdUI *pCmdUI)
{
	// Valide ssi moteur probabiliste
	// m_pATPDocCreuset->m_MoteurPresse.GetType() == CMoteurPresse::PROBA ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE); 
	m_pATPDocCreuset->m_MoteurPresse.GetType() == CMoteurPresse::ABAQUE_ETAL ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE); 
}

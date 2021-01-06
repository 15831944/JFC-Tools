// ATPPanelBas.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "resource.h"
#include "ATPPanelBas.h"
#include ".\atppanelbas.h"

// CATPPanelBas

IMPLEMENT_DYNCREATE(CATPPanelBas, ETSLayoutFormView)

CATPPanelBas::CATPPanelBas()
	: ETSLayoutFormView(CATPPanelBas::IDD)
{
	m_pATPDocCreuset = NULL;
}

CATPPanelBas::~CATPPanelBas()
{
}

void CATPPanelBas::DoDataExchange(CDataExchange* pDX)
{
	ETSLayoutFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CR_ECHELLE, m_Echelle);
	DDX_Control(pDX, IDC_CR_RESULTATS, m_Resultats);
	DDX_Control(pDX, IDC_CR_BTNZOOM, m_Zoom);
	DDX_Control(pDX, IDC_CR_BTNSWITCH, m_SwitchPlan_Hypo);
	DDX_Control(pDX, IDC_CR_BTNFSUPP, m_FicheSupp);
}

BEGIN_MESSAGE_MAP(CATPPanelBas, ETSLayoutFormView)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_CR_ECHELLE, OnBtnEchelle)
	ON_BN_CLICKED(IDC_CR_RESULTATS, OnBtnResultats)
	ON_BN_CLICKED(IDC_CR_BTNZOOM, OnBtnZoom)
	ON_BN_CLICKED(IDC_CR_BTNSWITCH, OnBtnSwitch)
	ON_BN_CLICKED(IDC_CR_BTNFSUPP, OnBtnFicheSupp)
END_MESSAGE_MAP()


// Diagnostics CATPPanelBas

#ifdef _DEBUG
void CATPPanelBas::AssertValid() const
{
	ETSLayoutFormView::AssertValid();
}

void CATPPanelBas::Dump(CDumpContext& dc) const
{
	ETSLayoutFormView::Dump(dc);
}
#endif //_DEBUG


// Gestionnaires de messages CATPPanelBas

void CATPPanelBas::OnInitialUpdate()
{
	ETSLayoutFormView::OnInitialUpdate();

	m_SwitchPlan_Hypo.SetIcon(IDI_CR_FLECHE);

	UpdateLayout(pane(VERTICAL)		<<item (IDC_CR_ECHELLE, GREEDY)
									<<item (IDC_CR_RESULTATS, GREEDY)
									<<item (IDC_CR_BTNZOOM, GREEDY)
									<<item (IDC_CR_BTNFSUPP, GREEDY)
								 // <<item (IDC_CR_BTNCRBREP, GREEDY)
								 // <<item (IDC_CR_BTNFILTRE, GREEDY)
									<<item (IDC_CR_BTNSWITCH, GREEDY)
									);

	SetColors();
}

void CATPPanelBas::SetColors()
{
	m_Echelle.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );
	m_Echelle.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );
	m_Echelle.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );

	m_Resultats.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );
	m_Resultats.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );
	m_Resultats.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );

	m_Zoom.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );
	m_Zoom.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );
	m_Zoom.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );

	m_FicheSupp.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );
	m_FicheSupp.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );
	m_FicheSupp.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );

	m_SwitchPlan_Hypo.SetColor( CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );
	m_SwitchPlan_Hypo.SetColor( CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );
	m_SwitchPlan_Hypo.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );

	Invalidate();
}

LONG CATPPanelBas::GetCXMin()
{
	// on renvoie la largeur
	return 10;
}

LONG CATPPanelBas::GetCYMin()
{
	// on renvoie la hauteur
	return 2*YBorder + 6*HBtn + 5*YInter+10 /*ajout AURELIEN*/ +40;
}
LONG CATPPanelBasQP::GetCYMin()
{
	// on renvoie la hauteur
	return 2*YBorder + 4*HBtn + 2*YInter+10;
}
//void CATPPanelBas::OnSize(UINT nType, int cx, int cy)
//{
////	UpdateLayout();
//	ETSLayoutFormView::OnSize(nType, cx, cy);
//}

void CATPPanelBas::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// N'appelez pas ETSLayoutFormView::OnPaint() pour la peinture des messages
	RECT Rect;
	// on récupère la zone cliente
	this->GetClientRect(&Rect);

	CBrush Brush(CATPColors::GetColorMedium(CATPColors::COLORCREUSET));

	// on dessine le fond
	dc.FillRect(&Rect, &Brush);
}

JVoid CATPPanelBas::SetDocument(JATPDocCreuset* pATPDocCreuset)
{
	m_pATPDocCreuset = pATPDocCreuset;
}

JVoid CATPPanelBas::Update()
{
	///
	if(m_pATPDocCreuset->m_AFFCreuset.GetAffQuickPlan() && m_pATPDocCreuset->m_AFFCreuset.GetEchelleQP())
	{
		m_Echelle.SetCheck(0);
		m_Resultats.SetCheck(0);
		m_FicheSupp.SetCheck(1);
	}
	else
	{
		m_Echelle.SetCheck(m_pATPDocCreuset->m_AFFCreuset.GetEchelleResult());
		m_Resultats.SetCheck(!m_pATPDocCreuset->m_AFFCreuset.GetEchelleResult());
		m_FicheSupp.SetCheck(0);
	}

	///
	if(m_pATPDocCreuset->m_AFFCreuset.GetPlanActif())
		m_Zoom.SetCheck(m_pATPDocCreuset->m_AFFZoomPlan.IsVisible());
	else
		m_Zoom.SetCheck(m_pATPDocCreuset->m_AFFContribution.IsVisible());

	///
	CString lbl;
	if(m_pATPDocCreuset->m_AFFCreuset.GetPlanActif())
		// on écrit le libellé Tableur
		lbl.FormatMessage(IDS_CR_TABLEUR);
	else
		// on écrit le libellé Calendaire
		lbl.FormatMessage(IDS_CR_CALEND);
	m_SwitchPlan_Hypo.SetWindowText(lbl);

	if(m_pATPDocCreuset && m_pATPDocCreuset->m_AFFCreuset.GetAffQuickPlan())
	{
		UpdateLayout(pane(VERTICAL)
			<<item (IDC_CR_BTNFSUPP, GREEDY)
			<<item (IDC_CR_ECHELLE, GREEDY)
			<<item (IDC_CR_RESULTATS, GREEDY)
			<<item (IDC_CR_BTNZOOM, GREEDY)
			);
		m_FicheSupp.ShowWindow(SW_SHOW);
		m_FicheSupp.EnableWindow();
		m_FicheSupp.SetWindowText("Formats");
		m_SwitchPlan_Hypo.ShowWindow(SW_HIDE);
	}
	else
	{
		UpdateLayout(pane(VERTICAL)		<<item (IDC_CR_ECHELLE, GREEDY)
			<<item (IDC_CR_RESULTATS, GREEDY)
			<<item (IDC_CR_BTNZOOM, GREEDY)
			<<item (IDC_CR_BTNFSUPP, GREEDY)
			<<item (IDC_CR_BTNSWITCH, GREEDY)
			);
		m_FicheSupp.ShowWindow(SW_HIDE);

		m_Zoom.ShowWindow(SW_SHOW);
		m_SwitchPlan_Hypo.ShowWindow(SW_SHOW);
	}
	
}

void CATPPanelBas::OnBtnEchelle()
{	
	if(m_pATPDocCreuset->m_AFFCreuset.GetAffQuickPlan())
		m_pATPDocCreuset->m_AFFCreuset.SetEchelleQP(false);

	m_pATPDocCreuset->m_AFFCreuset.SetEchelleResult(1);
	m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_CREUSET);
}

void CATPPanelBas::OnBtnResultats()
{	
	if(m_pATPDocCreuset->m_AFFCreuset.GetAffQuickPlan())
		m_pATPDocCreuset->m_AFFCreuset.SetEchelleQP(false);

	m_pATPDocCreuset->m_AFFCreuset.SetEchelleResult(0);
	m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_CREUSET);
}

void CATPPanelBas::OnBtnZoom()
{
	if(m_pATPDocCreuset->m_AFFCreuset.GetHypoActif() || m_pATPDocCreuset->m_AFFCreuset.GetAffQuickPlan())
		m_pATPDocCreuset->m_AFFContribution.SetVisible(!m_pATPDocCreuset->m_AFFContribution.IsVisible());
	else
		m_pATPDocCreuset->m_AFFZoomPlan.SetVisible(!m_pATPDocCreuset->m_AFFZoomPlan.IsVisible());
	// on met à jour les vues du document
	m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_PANEL);
}

void CATPPanelBas::OnBtnCourbe()
{
	if(m_pATPDocCreuset->IsValid())
		m_pATPDocCreuset->m_AFFCourbeReponse = !m_pATPDocCreuset->m_AFFCourbeReponse;
	m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_PANEL);
}

void CATPPanelBas::OnBtnFiltre()
{
	if(m_pATPDocCreuset->IsValid())
		m_pATPDocCreuset->m_AFFFiltreAudience = !m_pATPDocCreuset->m_AFFFiltreAudience;
	m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_PANEL);
}


void CATPPanelBas::OnBtnSwitch()
{
	//On inverse l'affichage des Zoom
	if(m_pATPDocCreuset->m_AFFCreuset.GetPlanActif() && m_pATPDocCreuset->m_AFFZoomPlan.IsVisible())
	{
		m_pATPDocCreuset->m_AFFZoomPlan.SetVisible(false);
		m_pATPDocCreuset->m_AFFContribution.SetVisible(true);
	}
	if(m_pATPDocCreuset->m_AFFCreuset.GetHypoActif() && m_pATPDocCreuset->m_AFFContribution.IsVisible())
	{
		m_pATPDocCreuset->m_AFFZoomPlan.SetVisible(true);
		m_pATPDocCreuset->m_AFFContribution.SetVisible(false);
	}

////////////////////////////////////////////////////////////////////////////////////////

	if(m_pATPDocCreuset->m_AFFCreuset.GetPlanActif())
	{
		int nbFormat = 0;
		for(m_pATPDocCreuset->m_MapIdFormatsSel.MoveFirst(); m_pATPDocCreuset->m_MapIdFormatsSel.IsValid(); m_pATPDocCreuset->m_MapIdFormatsSel.MoveNext())
		{
			if(m_pATPDocCreuset->m_MapIdFormatsSel.GetItem()>=2)
				nbFormat++;
		}
		if(nbFormat == m_pATPDocCreuset->m_MapIdFormatsSel.GetCount() && m_pATPDocCreuset->m_MapIdFormatsSel.GetCount()!=1)
			m_pATPDocCreuset->m_AFFColonneTotal = true;
	}


	if(m_pATPDocCreuset->m_AFFCreuset.GetHypoActif())
	{
		int nbFormat = 0;
		int idF;
		for(m_pATPDocCreuset->m_MapIdFormatsSel.MoveFirst(); m_pATPDocCreuset->m_MapIdFormatsSel.IsValid(); m_pATPDocCreuset->m_MapIdFormatsSel.MoveNext())
		{
			if(m_pATPDocCreuset->m_MapIdFormatsSel.GetItem()>=2)
			{
				nbFormat++;
				idF = m_pATPDocCreuset->m_MapIdFormatsSel.GetKey();
			}
		}
		if(nbFormat == 1)
		{
			m_pATPDocCreuset->m_MapIdFormatsSel.MoveTo(idF);
			m_pATPDocCreuset->m_MapIdFormatsSel.GetItem() = 3;
		}
	}
	if(m_pATPDocCreuset->m_AFFCreuset.GetHypoActif() && m_pATPDocCreuset->m_AFFColonneTotal)
	{
		for(m_pATPDocCreuset->m_MapIdFormatsSel.MoveFirst(); m_pATPDocCreuset->m_MapIdFormatsSel.IsValid(); m_pATPDocCreuset->m_MapIdFormatsSel.MoveNext())
		{
			if(m_pATPDocCreuset->m_MapIdFormatsSel.GetItem()==0)
				m_pATPDocCreuset->m_MapIdFormatsSel.GetItem() = 2;
		}
		m_pATPDocCreuset->UpdateAllViews(UPDATE_SEL_FORMAT);
	}

////////////////////////////////////////////////////////////////////////////

	//On inverse l'affichage
	m_pATPDocCreuset->m_AFFCreuset.SetPlanActif(!m_pATPDocCreuset->m_AFFCreuset.GetPlanActif());
	m_pATPDocCreuset->m_AFFCreuset.SetAffPlan(!m_pATPDocCreuset->m_AFFCreuset.GetAffPlan());
	m_pATPDocCreuset->m_AFFCreuset.SetHypoActif(!m_pATPDocCreuset->m_AFFCreuset.GetHypoActif());
	m_pATPDocCreuset->m_AFFCreuset.SetAffHypo(!m_pATPDocCreuset->m_AFFCreuset.GetAffHypo());


	//On met à jour
	m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_HYPO);
	m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_PLAN);
}

void CATPPanelBas::OnBtnFicheSupp()
{
	// TODO : ajoutez ici le code de votre gestionnaire de notification de contrôle
	if(m_pATPDocCreuset->m_AFFCreuset.GetAffQuickPlan())
	{
		m_pATPDocCreuset->m_AFFCreuset.SetEchelleQP(true);
		m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_CREUSET);
	}

}


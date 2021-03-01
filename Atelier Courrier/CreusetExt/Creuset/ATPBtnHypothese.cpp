// on inclut les définitions nécessaires
#include "stdafx.h"
#include "ATPBtnHypothese.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////
// le constructeur

CATPBtnHypothese::CATPBtnHypothese()
{
	// on initialise le document
	m_pATPDocCreuset = 0;
}

///////////////////////////////////////
// la fonction pour définir le document

JVoid CATPBtnHypothese::SetDocument(JATPDocCreuset* pATPDocCreuset)
{
	// on mémorise le document
	m_pATPDocCreuset = pATPDocCreuset;

	// on met à jour le composant
	this->OnUpdate();
}

//////////////////////////////////////////////
// la fonction pour mettre à jour le composant

JVoid CATPBtnHypothese::Update()
{
	// on met à jour le composant
	this->OnUpdate();
}

/////////////////
// le destructeur

CATPBtnHypothese::~CATPBtnHypothese()
{
	// on ne fait rien
}

BEGIN_MESSAGE_MAP(CATPBtnHypothese, JFCButtonList)
	//{{AFX_MSG_MAP(CATPBtnHypothese)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// JFCWave message handlers

void CATPBtnHypothese::PreSubclassWindow() 
{
	// on appelle le gestionnaire de base
	JFCButtonList::PreSubclassWindow();

	// on initialise la taille des cellules
	SetHorzItemSize(46,2,0, FALSE);
	SetVertItemSize(20,0,0, FALSE);
	//	SetDefaultColors(CATPColors::GetColorMedium(CATPColors::COLORCREUSET),CATPColors::GetColorBlack());   
}
 

JVoid CATPBtnHypothese::OnUpdate()
{
	if(m_pATPDocCreuset)
	{
		if(m_pATPDocCreuset->m_TblHypotheses.GetCount())
			SetHorzItemRange(1, m_pATPDocCreuset->m_TblHypotheses.GetCount()-1,true);
		else
			SetHorzItemRange(-1,-1,true);
	}
	// on redessine la fenêtre
	InvalidateRect(NULL, FALSE);
}

void CATPBtnHypothese::OnButtonSel(LONG i, LONG j)
{
	if(m_pATPDocCreuset)
	{
		// Affichage hypothèse / plan master inactif
		if(i>=0 && i<=m_pATPDocCreuset->m_TblHypotheses.GetCount() && j==0)
		{
			// Nouvelle hypothèse active
			m_pATPDocCreuset->m_IdxHypotheseActif	= i;
			if(m_pATPDocCreuset->m_MapAffHypothese.GetNbAff() >1 )
				m_pATPDocCreuset->m_MapAffHypothese.Set(i);
			else
				m_pATPDocCreuset->m_MapAffHypothese.ResetSet(i);
			

			if (m_pATPDocCreuset->m_AFFPlanEtendu)
				// Mode plan étendu
				m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_BAR_PLAN);
			else
			{
				// Mode hypothèse / Mise à jour hypothèse active
				m_pATPDocCreuset->m_IdxHypotheseCourant = m_pATPDocCreuset->m_IdxHypotheseActif;
			}
				
			if(i>0)
				m_pATPDocCreuset->m_AFFPlanMaitre = false;
			else
				m_pATPDocCreuset->m_AFFPlanMaitre = true;

		}
		// Update affichage insertions
		m_pATPDocCreuset->UpdateAllViews(UPDATE_SEL_HYPOTHESE);
	}
}

void CATPBtnHypothese::OnLedSel(LONG i, LONG j)
{
	if(m_pATPDocCreuset)
	{
		// On ne peut pas changer le led de l'hypothese actif
		if(m_pATPDocCreuset->m_IdxHypotheseActif == i)
			return;

		// Affichage hypothèse / plan master inactif
		if(i>=0 && i<=m_pATPDocCreuset->m_TblHypotheses.GetCount() && j==0)
			// Nouvelle hypothèse active
			m_pATPDocCreuset->m_MapAffHypothese.Invert(i);
		
		// Update affichage insertions
		m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_CREUSET);
	}

}

void CATPBtnHypothese::OnGetItemParams(LONG i, LONG j, BtnParams & Params)
{

	// on récupère l'état de l'élément
	Params.m_State = 0;
	Params.m_Style = JFCButtonList::BUT_SINGLE_SEL|JFCButtonList::LED_SEL;

	if(m_pATPDocCreuset)
	{

		if(i)
			Params.m_Text.Format("H%d",i); 
		else
			// Le plan de reference
			Params.m_Text.LoadString(IDS_CR_PM);

		// on teste si élmt est l'hypothese active
		if (m_pATPDocCreuset->m_IdxHypotheseActif == i)
			Params.m_State = JFCButtonList::BUT_DOWN;

		// Allumer les hypothèses pour l'affichage des courbes
		if(m_pATPDocCreuset->m_MapAffHypothese.GetState(i) == true)
			Params.m_State |= JFCButtonList::LED_ON ;
	}

	Params.m_Color_BtnDown	=	CATPColors::GetColorLight(CATPColors::COLORCREUSET);
	Params.m_Color_BtnHover	=	CATPColors::GetColorDark(CATPColors::COLORCREUSET);
	Params.m_Color_BtnUp	=	CATPColors::GetColorMedium(CATPColors::COLORCREUSET);
	Params.m_Color_LedOff	=	CATPColors::GetColorMedium(CATPColors::COLORCREUSET);

	if(m_pATPDocCreuset->m_MapAffHypothese.GetNbAff()>1)
		Params.m_Color_LedOn	=	CATPColors::GetColorGrph(i);
	else
		Params.m_Color_LedOn	=	CATPColors::GetColorDark(CATPColors::COLORCREUSET);

	Params.m_Color_Text		=	CATPColors::GetColorBlack();
}

bool CATPBtnHypothese::OnBeginDrawing(CDC & dc)
{
	if(m_pATPDocCreuset && m_pATPDocCreuset->m_TblHypotheses.GetCount())
		return true;
	return false;
}


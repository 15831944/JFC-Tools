// on inclut les d�finitions n�cessaires
#include "stdafx.h"
#include "ATPPupitre_A8.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////
// le constructeur

CATPPupitre_A8::CATPPupitre_A8()
{
	// on initialise le document
	m_pATPDocCreuset = 0;
}

///////////////////////////////////////
// la fonction pour d�finir le document

JVoid CATPPupitre_A8::SetDocument(JATPDocCreuset* pATPDocCreuset)
{
	// on m�morise le document
	m_pATPDocCreuset = pATPDocCreuset;

	// on met � jour le composant
	this->OnUpdate();
}

//////////////////////////////////////////////
// la fonction pour mettre � jour le composant

JVoid CATPPupitre_A8::Update()
{
	// on met � jour le composant
	this->OnUpdate();
}

/////////////////
// le destructeur

CATPPupitre_A8::~CATPPupitre_A8()
{
	// on ne fait rien
}

BEGIN_MESSAGE_MAP(CATPPupitre_A8, JFCButtonList)
	//{{AFX_MSG_MAP(CATPPupitre_A8)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// la fonction pour r�cup�rer l'�tat des �l�ments
void CATPPupitre_A8::OnGetItemParams(LONG i, LONG j, BtnParams & Params)
{
	// on r�cup�re l'�tat de l'�l�ment
	Params.m_State = 0;

	// Mode s�lection simple et led triangle vers le haut (asc vertical)
	Params.m_Style			=	(JFCButtonList::BUT_SINGLE_SEL|JFCButtonList::LED_SEL);
	Params.m_Color_BtnDown	=	CATPColors::GetColorLight(CATPColors::COLORCREUSET);
	Params.m_Color_BtnHover=	CATPColors::GetColorDark(CATPColors::COLORCREUSET);
	Params.m_Color_BtnUp	=	CATPColors::GetColorMedium(CATPColors::COLORCREUSET);
	// Params.m_Color_LedOff	=	CATPColors::GetColorLight(CATPColors::COLORCREUSET);
	Params.m_Color_LedOff	=	CATPColors::GetColorMedium(CATPColors::COLORCREUSET);
	Params.m_Color_LedOn	=	CATPColors::GetColorDark(CATPColors::COLORCREUSET);
	Params.m_Color_Text		=	CATPColors::GetColorBlack();
	Params.m_Design_Led     =   1; 
	Params.m_Text			=   "";  

}

void CATPPupitre_A8::OnButtonSel(LONG i, LONG j)
{
	// D�calage format affichage vers le haut
	if (m_pATPDocCreuset->m_DecalAffichFormat > 0) 
		m_pATPDocCreuset->m_DecalAffichFormat --;
	
	// Mise � jour vue pupitre formats
	m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_CREUSET);
}


void CATPPupitre_A8::OnLedSel(LONG i, LONG j)
{
	// D�calage format affichage vers le haut
	if (m_pATPDocCreuset->m_DecalAffichFormat > 0) 
		m_pATPDocCreuset->m_DecalAffichFormat --;
	
	// Mise � jour vue pupitre formats
	m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_CREUSET);
}

bool CATPPupitre_A8::OnBeginDrawing(CDC & dc)
{
	if(m_pATPDocCreuset && m_pATPDocCreuset->m_MapIdFormatsSel.GetCount())
		return true;
	return false;
}

//////////////////////////////////////////////
// la fonction pour appliquer les mises � jour

JVoid CATPPupitre_A8::OnUpdate()
{
	if(m_pATPDocCreuset)
	{
		if(m_pATPDocCreuset->m_MapIdFormatsSel.GetCount() > 1 )
			// au moins 2 formats
			SetVertItemRange(0,0,true);
		else
			// sinon rien
			SetVertItemRange(-1,-1,true);
	}
	// on redessine la fen�tre
	InvalidateRect(NULL, FALSE);
}

/////////////////////////////////////
// CATPPupitre_A8 message handlers

void CATPPupitre_A8::PreSubclassWindow() 
{
	// on appelle le gestionnaire de base
	JFCButtonList::PreSubclassWindow();

	// on initialise la taille des cellules
	SetHorzItemSize(19,0,0, FALSE);
	SetVertItemSize(22,0,0, FALSE);
}

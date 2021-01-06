// on inclut les définitions nécessaires
#include "stdafx.h"
#include "ATPPupitre_A7.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////
// le constructeur

CATPPupitre_A7::CATPPupitre_A7()
{
	// on initialise le document
	m_pATPDocCreuset = 0;
}

///////////////////////////////////////
// la fonction pour définir le document

JVoid CATPPupitre_A7::SetDocument(JATPDocCreuset* pATPDocCreuset)
{
	// on mémorise le document
	m_pATPDocCreuset = pATPDocCreuset;

	// on met à jour le composant
	this->OnUpdate();
}

//////////////////////////////////////////////
// la fonction pour mettre à jour le composant

JVoid CATPPupitre_A7::Update()
{
	// on met à jour le composant
	this->OnUpdate();
}

/////////////////
// le destructeur

CATPPupitre_A7::~CATPPupitre_A7()
{
	// on ne fait rien
}

BEGIN_MESSAGE_MAP(CATPPupitre_A7, JFCButtonList)
	//{{AFX_MSG_MAP(CATPPupitre_A7)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// la fonction pour récupérer l'état des éléments
void CATPPupitre_A7::OnGetItemParams(LONG i, LONG j, BtnParams & Params)
{
	// Etat du controle tous

	// Tous les formats sont-ils actifs
	//if (NbFormatActif  == m_pATPDocCreuset->m_MapIdFormatsSel.GetCount())
	//	Etat = 0x0003;


	// Mode simple sélection avec led forme carré
	Params.m_Color_BtnDown	=	CATPColors::GetColorLight(CATPColors::COLORCREUSET);
	Params.m_Color_BtnHover	=	CATPColors::GetColorDark(CATPColors::COLORCREUSET);
	Params.m_Color_BtnUp	=	CATPColors::GetColorMedium(CATPColors::COLORCREUSET);
	// Params.m_Color_LedOff	=	CATPColors::GetColorLight(CATPColors::COLORCREUSET);
	Params.m_Color_LedOff	=	CATPColors::GetColorMedium(CATPColors::COLORCREUSET);
	Params.m_Color_LedOn	=	CATPColors::GetColorDark(CATPColors::COLORCREUSET);
	Params.m_Design_Led     =   3; 
	Params.m_Color_Text		=	CATPColors::GetColorBlack();
	
	if(m_pATPDocCreuset->m_AFFCreuset.GetPlanActif())
	{
		// Texte "tous les formats"
		Params.m_Text			=   "Tous";  
		Params.m_Style			=	(JFCButtonList::BUT_SINGLE_SEL/*|JFCButtonList::LED_SEL*/);
		// on récupère l'état de l'élément
		JUnt32 Etat = 0;
	//	JUnt32 NbFormatActif     = 0;
		JUnt32 NbFormatAllume    = 0;
		for(m_pATPDocCreuset->m_MapIdFormatsSel.MoveFirst();
			m_pATPDocCreuset->m_MapIdFormatsSel.IsValid();
			m_pATPDocCreuset->m_MapIdFormatsSel.MoveNext())
		{
			// Etat du format
			JUnt32 & Etat = m_pATPDocCreuset->m_MapIdFormatsSel.GetItem();

			// est-il actif ?
			//if(Etat == 3 || Etat == 1)
			//{
			//	// Ident format en cours de sélection
			//	NbFormatActif ++; 
			//}
			// Est-il allumé
			if(Etat == 2 || Etat == 3)
			{
				// Ident format en cours de sélection
				NbFormatAllume ++; 
			}
		}

		// Tous les formats sont-ils allumés
		if (NbFormatAllume == m_pATPDocCreuset->m_MapIdFormatsSel.GetCount())
		//	Etat =  0x0002;
			Etat =  BUT_DOWN;
		Params.m_State = Etat;
	}
	else
	{
		// Texte "tous les formats"
		Params.m_Text			=   "Total";  
		Params.m_Style			=	(JFCButtonList::BUT_SINGLE_SEL);
		// on récupère l'état de l'élément
		Params.m_State = m_pATPDocCreuset->m_AFFColonneTotal ? BUT_DOWN : 0;
	}

}

void CATPPupitre_A7::OnButtonSel(LONG i, LONG j)
{
	if(m_pATPDocCreuset->m_AFFCreuset.GetPlanActif())
	{
	//// Rappel sélection
	//// Etat 0  : non sélectionné
	//// Etat 1  : sélectionné pour mode insertion
	//// Etat 2  : affichage uniquement
	//// Etat 3  : mode insertion et affiché

	//// Voir nb formats déjà sélectionnés
	//JUnt32 NbFormatActif     = 0;
	//for(m_pATPDocCreuset->m_MapIdFormatsSel.MoveFirst();
	//	m_pATPDocCreuset->m_MapIdFormatsSel.IsValid();
	//	m_pATPDocCreuset->m_MapIdFormatsSel.MoveNext())
	//{
	//	JUnt32 & Etat = m_pATPDocCreuset->m_MapIdFormatsSel.GetItem();
	//	if(Etat == 3 || Etat == 1)
	//	{
	//		// Ident format en cours de sélection
	//		NbFormatActif ++; 
	//	}
	//}

	//JUnt32 etat;
	//if (NbFormatActif == m_pATPDocCreuset->m_MapIdFormatsSel.GetCount())
	//	// Déselectionner tous les formats
	//	etat = 2;
	//else
	//	// Sélectionner tous les formats
	//	etat = 3;
	//
	//for(m_pATPDocCreuset->m_MapIdFormatsSel.MoveFirst();
	//	m_pATPDocCreuset->m_MapIdFormatsSel.IsValid();
	//	m_pATPDocCreuset->m_MapIdFormatsSel.MoveNext())
	//	{
	//		m_pATPDocCreuset->m_MapIdFormatsSel.GetItem() = etat;
	//	}

		JUnt32 Etat = 0;
		JUnt32 NbFormatAllume    = 0;
		for(m_pATPDocCreuset->m_MapIdFormatsSel.MoveFirst();
			m_pATPDocCreuset->m_MapIdFormatsSel.IsValid();
			m_pATPDocCreuset->m_MapIdFormatsSel.MoveNext())
		{
			// Etat du format
			JUnt32 & Etat = m_pATPDocCreuset->m_MapIdFormatsSel.GetItem();
			// Est-il allumé
			if(Etat == 2 || Etat == 3)
			{
				// Ident format en cours de sélection
				NbFormatAllume ++; 
			}
		}

		// Tous les formats sont-ils allumés
		if (NbFormatAllume == m_pATPDocCreuset->m_MapIdFormatsSel.GetCount())
		//	Etat =  0x0002;
			Etat =  BUT_DOWN;

		for(m_pATPDocCreuset->m_MapIdFormatsSel.MoveFirst();
			m_pATPDocCreuset->m_MapIdFormatsSel.IsValid();
			m_pATPDocCreuset->m_MapIdFormatsSel.MoveNext())
		{
			if(Etat == BUT_DOWN && m_pATPDocCreuset->m_MapIdFormatsSel.GetItem()!=3)
				m_pATPDocCreuset->m_MapIdFormatsSel.GetItem() = 0;
			if(Etat == 0 && m_pATPDocCreuset->m_MapIdFormatsSel.GetItem()!=3)
				m_pATPDocCreuset->m_MapIdFormatsSel.GetItem() = 2;
		}
		// Mise à jour bouton
		m_pATPDocCreuset->UpdateAllViews(UPDATE_SEL_FORMAT);	
	}
	else
	{
		m_pATPDocCreuset->m_AFFColonneTotal = !m_pATPDocCreuset->m_AFFColonneTotal;
		m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_CREUSET);
	}
}

void CATPPupitre_A7::OnLedSel(LONG i, LONG j)
{
/*	// Rappel sélection
	// Etat 0  : non sélectionné
	// Etat 1  : sélectionné pour mode insertion
	// Etat 2  : affichage uniquement
	// Etat 3  : mode insertion et affiché

	JUnt32 etatVoyant;
	BtnParams Params;
	OnGetItemParams(i, j,  Params);

	// Repositionne état led bouton tous
	switch(Params.m_State)
	{
		case 0:
			// Allume toutes les leds
			etatVoyant = 2; break;
		case 1:
			// Allume toutes les leds et active tous les formats
			etatVoyant = 3; break;
		case 2:
			// Eteint tous les formats
			etatVoyant = 0; break;
		case 3:
			// Eteint tous les formats, desactive tous les formats actifs restent
			etatVoyant = 0; break;
	}
		
	// Repositionne les led formats sélectionnés
	for(m_pATPDocCreuset->m_MapIdFormatsSel.MoveFirst();
		m_pATPDocCreuset->m_MapIdFormatsSel.IsValid();
		m_pATPDocCreuset->m_MapIdFormatsSel.MoveNext())
	{
		JUnt32 & Etat = m_pATPDocCreuset->m_MapIdFormatsSel.GetItem();
		if (etatVoyant & 0x0002)
		{
			// on allume toutes les leds format
			if (Etat == 0)
				Etat = 2;
			else if (Etat == 1)
				Etat = 3;
		}
		else
		{
			// on éteint toutes les leds format
			if (Etat == 2)
				Etat = 0;
			else if (Etat == 3)
				Etat = 0;
		}
	}

	// Mise à jour bouton
	m_pATPDocCreuset->UpdateAllViews(UPDATE_SEL_FORMAT);	
*/}


bool CATPPupitre_A7::OnBeginDrawing(CDC & dc)
{
	if(m_pATPDocCreuset && m_pATPDocCreuset->m_MapIdFormatsSel.GetCount())
		return true;
	return false;
}

//////////////////////////////////////////////
// la fonction pour appliquer les mises à jour
JVoid CATPPupitre_A7::OnUpdate()
{
	if(m_pATPDocCreuset)
	{
		if(m_pATPDocCreuset->m_MapIdFormatsSel.GetCount() > 1)
			// au moins 2 formats
			SetVertItemRange(0,0,true);
		else
			SetVertItemRange(-1,-1,true);
	}

	// on redessine la fenêtre
	InvalidateRect(NULL, FALSE);
}

/////////////////////////////////////
// CATPPupitre_A7 message handlers

void CATPPupitre_A7::PreSubclassWindow() 
{
	// on appelle le gestionnaire de base
	JFCButtonList::PreSubclassWindow();

	// on initialise la taille des cellules
	SetHorzItemSize(91,0,0, FALSE);  // 60
	SetVertItemSize(22,0,0, FALSE);
}

// on inclut les définitions nécessaires
#include "stdafx.h"
#include "ATPPupitre_A6.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////
// le constructeur

CATPPupitre_A6::CATPPupitre_A6()
{
	// on initialise le document
	m_pATPDocCreuset = 0;
}

///////////////////////////////////////
// la fonction pour définir le document

JVoid CATPPupitre_A6::SetDocument(JATPDocCreuset* pATPDocCreuset)
{
	// on mémorise le document
	m_pATPDocCreuset = pATPDocCreuset;

	// on met à jour le composant
	this->OnUpdate();
}

//////////////////////////////////////////////
// la fonction pour mettre à jour le composant

JVoid CATPPupitre_A6::Update()
{
	// on met à jour le composant
	this->OnUpdate();
}

/////////////////
// le destructeur

CATPPupitre_A6::~CATPPupitre_A6()
{
	// on ne fait rien
}

BEGIN_MESSAGE_MAP(CATPPupitre_A6, JFCButtonList)
	//{{AFX_MSG_MAP(CATPPupitre_A6)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// la fonction pour récupérer l'état des éléments
void CATPPupitre_A6::OnGetItemParams(LONG i, LONG j, BtnParams & Params)
{
	// on récupère l'état de l'élément
	Params.m_State = 0;

	// Indice format (tiens compte position 1er elmt format via ascenseur)
	j = j + m_pATPDocCreuset->m_DecalAffichFormat;

	// Test si indice Ok
	if (j >= m_pATPDocCreuset->m_MapIdFormatsSel.GetCount())
		j = m_pATPDocCreuset->m_MapIdFormatsSel.GetCount()-1;

	m_pATPDocCreuset->m_MapIdFormatsSel.MoveFirst();
	for(JInt32 inc = 0; inc < j ; inc++)
		m_pATPDocCreuset->m_MapIdFormatsSel.MoveNext();

	// Récup Table des formats
	const JSRCPresse  & srcpresse = JSRCPresse::GetInstance();
	
	srcpresse.m_TBLFormats.MoveTo(m_pATPDocCreuset->m_MapIdFormatsSel.GetKey()); // Récup item correspondant à l'indice
	if(srcpresse.m_TBLFormats.IsValid())
	{
		IElem *pElem = srcpresse.m_TBLFormats.GetItem();
		if(pElem)
			Params.m_Text = pElem->GetLabel().AsJCharPtr();  
	}

	// Vérifie si le format est valide
	if(m_pATPDocCreuset->m_MapIdFormatsSel.IsValid())
	{
		if(m_pATPDocCreuset->m_AFFCreuset.GetPlanActif())
			Params.m_State = m_pATPDocCreuset->m_MapIdFormatsSel.GetItem();
		else
			if(m_pATPDocCreuset->m_MapIdFormatsSel.GetItem()>=2)
				Params.m_State = BUT_DOWN;
	}

	if(m_pATPDocCreuset->m_AFFCreuset.GetPlanActif())
		Params.m_Style = (JFCButtonList::BUT_MULTI_SEL|JFCButtonList::LED_SEL);
	else
		Params.m_Style = (JFCButtonList::BUT_SINGLE_SEL);

	// Multisélection avec led forme ronde
	//Params.m_Style			=	(JFCButtonList::BUT_MULTI_SEL|JFCButtonList::LED_SEL);
	Params.m_Color_BtnDown	=	CATPColors::GetColorLight(CATPColors::COLORCREUSET);
	Params.m_Color_BtnHover	=	CATPColors::GetColorDark(CATPColors::COLORCREUSET);
	Params.m_Color_BtnUp	=	CATPColors::GetColorMedium(CATPColors::COLORCREUSET);
	//Params.m_Color_LedOff	=	CATPColors::GetColorMedium(CATPColors::COLORCREUSET);
	//Params.m_Color_LedOn	=	CATPColors::GetColorDark(CATPColors::COLORCREUSET);
	Params.m_Color_LedBorder=	CATPColors::GetColorSelect(CATPColors::COLORCREUSET);
	Params.m_Color_LedOff	=	CATPColors::GetColorMedium(CATPColors::COLORCREUSET);
	Params.m_Color_LedOn	=	CATPColors::GetColorLight(CATPColors::COLORCREUSET);
	Params.m_Design_Led     =   4;
	Params.m_Color_Text		=	CATPColors::GetColorBlack();
}

void CATPPupitre_A6::OnButtonSel(LONG i, LONG j)
{
	// Rappel sélection
	// Etat 0  : non sélectionné
	// Etat 1  : rien
	// Etat 2  : affichage uniquement
	// Etat 3  : mode insertion et affiché

	// Indice format
	j = j + m_pATPDocCreuset->m_DecalAffichFormat;
	// Test si indice Ok
	if (j >= m_pATPDocCreuset->m_MapIdFormatsSel.GetCount())
		j = m_pATPDocCreuset->m_MapIdFormatsSel.GetCount()-1;

	m_pATPDocCreuset->m_MapIdFormatsSel.MoveFirst();
	for(JInt32 inc = 0; inc < j; inc++)
		m_pATPDocCreuset->m_MapIdFormatsSel.MoveNext();
	// Vérifie si le format est valide
	if(m_pATPDocCreuset->m_MapIdFormatsSel.IsValid())
	{
		JUnt32 key = m_pATPDocCreuset->m_MapIdFormatsSel.GetKey();
		//JUnt32 & Etat = m_pATPDocCreuset->m_MapIdFormatsSel.GetItem();


		if(m_pATPDocCreuset->m_AFFCreuset.GetPlanActif())
		{
			//On déselectionne le format actif
			int oldEtat = m_pATPDocCreuset->m_MapIdFormatsSel.GetItem();

			for(m_pATPDocCreuset->m_MapIdFormatsSel.MoveFirst();
				m_pATPDocCreuset->m_MapIdFormatsSel.IsValid();
				m_pATPDocCreuset->m_MapIdFormatsSel.MoveNext())
			{
				if(m_pATPDocCreuset->m_MapIdFormatsSel.GetItem()==3)
					m_pATPDocCreuset->m_MapIdFormatsSel.GetItem() = 2;
			}
			//On selectionne le nouveau
			m_pATPDocCreuset->m_MapIdFormatsSel.MoveTo(key);
			if(oldEtat != 3)
				m_pATPDocCreuset->m_MapIdFormatsSel.GetItem() = 3;
		}
		else
		{
			if(m_pATPDocCreuset->m_MapIdFormatsSel.GetItem()>=2)
				m_pATPDocCreuset->m_MapIdFormatsSel.GetItem() = 0;
			else
				//On rend le format visible
				m_pATPDocCreuset->m_MapIdFormatsSel.GetItem() = 2;
		}
        m_pATPDocCreuset->UpdateAllViews(UPDATE_SEL_FORMAT);
	}

	
	
/*	// Rappel sélection
	// Etat 0  : non sélectionné
	// Etat 1  : sélectionné pour mode insertion
	// Etat 2  : affichage uniquement
	// Etat 3  : mode insertion et affiché
	

	// Mode sélection (multi sélection si touche CONTROL ou SHIFT enfoncé)
	short StateControl = GetKeyState(VK_CONTROL);
	short StateShift   = GetKeyState(VK_SHIFT);
	short State		   = 1;	
	if (StateControl < 0 || StateShift < 0) 
		State = -1;

	// Voir si déjà un format sélectionné
	JInt32 NbFormatActif     = 0;
	for(m_pATPDocCreuset->m_MapIdFormatsSel.MoveFirst();
		m_pATPDocCreuset->m_MapIdFormatsSel.IsValid();
		m_pATPDocCreuset->m_MapIdFormatsSel.MoveNext())
	{
		JUnt32 & Etat = m_pATPDocCreuset->m_MapIdFormatsSel.GetItem();
		if(Etat == 3 || Etat == 1)
		{
			// Ident format en cours de sélection
			NbFormatActif ++; 
		}
	}


	// Indice format
	j = j + m_pATPDocCreuset->m_DecalAffichFormat;
	// Test si indice Ok
	if (j >= m_pATPDocCreuset->m_MapIdFormatsSel.GetCount())
		j = m_pATPDocCreuset->m_MapIdFormatsSel.GetCount()-1;

	m_pATPDocCreuset->m_MapIdFormatsSel.MoveFirst();
	for(JInt32 inc = 0; inc < j; inc++)
		m_pATPDocCreuset->m_MapIdFormatsSel.MoveNext();

	// Vérifie si le format est valide
	if(m_pATPDocCreuset->m_MapIdFormatsSel.IsValid())
	{
		JUnt32 & Etat = m_pATPDocCreuset->m_MapIdFormatsSel.GetItem();
		switch(Etat)
		{
			case 0:
				if (NbFormatActif == 0 || State < 0)
				{
					// Si aucun format actif ou mode multi-sélection
					Etat = 3; 
				}
				if (NbFormatActif > 0 && State >= 0)
				// déselectionne tous les autres
				{
					Etat = 3;
					JUnt32 ifFormat = m_pATPDocCreuset->m_MapIdFormatsSel.GetKey();
					for(m_pATPDocCreuset->m_MapIdFormatsSel.MoveFirst();
					m_pATPDocCreuset->m_MapIdFormatsSel.IsValid();
					m_pATPDocCreuset->m_MapIdFormatsSel.MoveNext())
					{
						if (m_pATPDocCreuset->m_MapIdFormatsSel.GetKey() != ifFormat)
						{
							if (m_pATPDocCreuset->m_MapIdFormatsSel.GetItem() == 2 || m_pATPDocCreuset->m_MapIdFormatsSel.GetItem() == 3)
								m_pATPDocCreuset->m_MapIdFormatsSel.GetItem() = 0;
						}
					}

				}
				break;

			case 1:
				Etat = 0; break;

			case 2:
				if (NbFormatActif == 0 || State < 0)
				{
					// Si aucun format actif ou mode multi-sélection
					Etat = 3; 
				}
				if (NbFormatActif > 0 && State >= 0)
				// déselectionne tous les autres
				{
					Etat = 3;
					JUnt32 ifFormat = m_pATPDocCreuset->m_MapIdFormatsSel.GetKey();
					for(m_pATPDocCreuset->m_MapIdFormatsSel.MoveFirst();
					m_pATPDocCreuset->m_MapIdFormatsSel.IsValid();
					m_pATPDocCreuset->m_MapIdFormatsSel.MoveNext())
					{
						if (m_pATPDocCreuset->m_MapIdFormatsSel.GetKey() != ifFormat)
						{
							if (m_pATPDocCreuset->m_MapIdFormatsSel.GetItem() == 2 || m_pATPDocCreuset->m_MapIdFormatsSel.GetItem() == 3)
								m_pATPDocCreuset->m_MapIdFormatsSel.GetItem() = 0;
						}
					}

				}
				break;

			case 3:
				Etat = 0; break;
				//Etat = 2; break;
		}
	}

	// Mise à jour du bouton Tous // Voir nb formats déjà sélectionnés
	NbFormatActif     = 0;
	for(m_pATPDocCreuset->m_MapIdFormatsSel.MoveFirst();
		m_pATPDocCreuset->m_MapIdFormatsSel.IsValid();
		m_pATPDocCreuset->m_MapIdFormatsSel.MoveNext())
	{
		JUnt32 & Etat = m_pATPDocCreuset->m_MapIdFormatsSel.GetItem();
		if(Etat & 0x0001)
		{
			// Ident format en cours de sélection
			NbFormatActif ++; 
		}
	}
	
	// Rafraichissement des formats
	if(m_pATPDocCreuset->m_MapIdFormatsSel.GetCount()>1)
        m_pATPDocCreuset->UpdateAllViews(UPDATE_SEL_FORMAT);
*/	
}

void CATPPupitre_A6::OnLedSel(LONG i, LONG j)
{
	// Indice format
	j = j + m_pATPDocCreuset->m_DecalAffichFormat;
	// Test si indice Ok
	if (j >= m_pATPDocCreuset->m_MapIdFormatsSel.GetCount())
		j = m_pATPDocCreuset->m_MapIdFormatsSel.GetCount()-1;

	m_pATPDocCreuset->m_MapIdFormatsSel.MoveFirst();
	for(JInt32 inc = 0; inc < j ; inc++)
		m_pATPDocCreuset->m_MapIdFormatsSel.MoveNext();

	// Vérifie si le format est valide
	if(m_pATPDocCreuset->m_MapIdFormatsSel.IsValid())
	{
		JUnt32 & Etat = m_pATPDocCreuset->m_MapIdFormatsSel.GetItem();
		switch(Etat)
		{
			case 0:
				Etat = 2; break;
			//case 1:
			//	Etat = 3; break;
			case 2:
				Etat = 0; break;
			//case 3:
			//	Etat = 0; break;
		}
	}

	//// Mise à jour du bouton Tous // Voir nb formats déjà allumés
	//JUnt32 NbFormatLedAllume     = 0;
	//for(m_pATPDocCreuset->m_MapIdFormatsSel.MoveFirst();
	//	m_pATPDocCreuset->m_MapIdFormatsSel.IsValid();
	//	m_pATPDocCreuset->m_MapIdFormatsSel.MoveNext())
	//{
	//	JUnt32 & Etat = m_pATPDocCreuset->m_MapIdFormatsSel.GetItem();
	//	if(Etat & 0x0002)
	//	{
	//		// Ident format en cours de sélection
	//		NbFormatLedAllume ++; 
	//	}
	//}


	// Raffraichissement fenêtre format	
	m_pATPDocCreuset->UpdateAllViews(UPDATE_SEL_FORMAT);
}


bool CATPPupitre_A6::OnBeginDrawing(CDC & dc)
{
	if(m_pATPDocCreuset && m_pATPDocCreuset->m_MapIdFormatsSel.GetCount())
		return true;
	return false;
}

//////////////////////////////////////////////
// la fonction pour appliquer les mises à jour

JVoid CATPPupitre_A6::OnUpdate()
{

	// Si aucun format sélectionné, on sélectionne le 1er
/*	JBool AucunFormatSel = true;
	for (m_pATPDocCreuset->m_MapIdFormatsSel.MoveFirst();
		 m_pATPDocCreuset->m_MapIdFormatsSel.IsValid();
		 m_pATPDocCreuset->m_MapIdFormatsSel.MoveNext())
	{
		// récupère état du format
		JUnt32 & Etat = m_pATPDocCreuset->m_MapIdFormatsSel.GetItem();
		if (Etat & 0x0001)
		{
			// format actif
			AucunFormatSel = false;
			break;
		}
	}

	if (AucunFormatSel && m_pATPDocCreuset->m_MapIdFormatsSel.GetCount() > 0)
	{
		// On se positionne sur le 1er format de l'offre
		m_pATPDocCreuset->m_Offre.m_TBLIdFormats.MoveFirst();
		
		// Récup id format
		JUnt32 Id = m_pATPDocCreuset->m_Offre.m_TBLIdFormats.GetItem();
		if(Id)
		{
			// Positionnement dans le map des formats
			m_pATPDocCreuset->m_MapIdFormatsSel.MoveTo(Id);

			// On le sélectionne
			JUnt32 & Etat = m_pATPDocCreuset->m_MapIdFormatsSel.GetItem();
			Etat = 3;

			// Rafraichissement des formats
			m_pATPDocCreuset->UpdateAllViews(UPDATE_SEL_FORMAT);
		}
	}
*/
	// Nombre d'élément à afficher selon position ascenseur
	JUnt32 NbElmtAff = m_pATPDocCreuset->m_MapIdFormatsSel.GetCount();
	NbElmtAff -= m_pATPDocCreuset->m_DecalAffichFormat;

	// Initialisation item range
	if(m_pATPDocCreuset)
	{
		if(NbElmtAff)
			SetVertItemRange(0,NbElmtAff-1,true);
		else
			SetVertItemRange(-1,-1,true);
	}
	SetIcons(IDI_CR_LEDON, IDI_CR_LEDNEUTRE);
	
	// on redessine la fenêtre
	InvalidateRect(NULL, FALSE);
}

/////////////////////////////////////
// CATPPupitre_A6 message handlers

void CATPPupitre_A6::PreSubclassWindow() 
{
	// on appelle le gestionnaire de base
	JFCButtonList::PreSubclassWindow();

	// on initialise la taille des cellules
	SetHorzItemSize(133,2,2, FALSE);
	SetVertItemSize(24,2,2, FALSE);
	//SetDefaultColors(CATPColors::GetColorMedium(CATPColors::COLORCREUSET),CATPColors::GetColorBlack());   
}
